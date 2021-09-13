#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "ast\fl_ast_decl.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "fl_compiler.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {

	/**
	 * TransformationResolveInclude
	 */

	struct TransformationResolveInclude : public ::testing::Test
	{
		std::unique_ptr<fluffy::Compiler> compiler;

		// Antes de cada test
		virtual void SetUp() override {
			compiler = std::make_unique<fluffy::Compiler>();
		}
	};

	/**
	 * Testing
	 */

	TEST_F(TransformationResolveInclude, TestIncludeOne)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult()
			{}

			virtual ~CheckResult()
			{}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::IncludeItemDecl)
				{
					auto includeItem = ast::safe_cast<ast::IncludeItemDecl>(node);

					if (includeItem->identifier == TString("FooOne"))
					{
						ASSERT_TRUE(includeItem->hasBeenResolved);
						ASSERT_TRUE(includeItem->referencedScope != nullptr);
						ASSERT_TRUE(includeItem->referencedScope->identifier == TString("foo"));
						ASSERT_TRUE(includeItem->referencedScope->nodeType == AstNodeType_e::NamespaceDecl);
						ASSERT_TRUE(includeItem->referencedNodeList.size() == 1);
						ASSERT_TRUE(includeItem->referencedNodeList[0]->identifier == TString("FooOne"));
						ASSERT_TRUE(includeItem->referencedNodeList[0]->nodeType == AstNodeType_e::ClassDecl);

						foundFooOne = true;
					}
					
					if (includeItem->identifier == TString("FooThree"))
					{
						ASSERT_TRUE(includeItem->hasBeenResolved);
						ASSERT_TRUE(includeItem->referencedScope != nullptr);
						ASSERT_TRUE(includeItem->referencedScope->identifier == TString("loo"));
						ASSERT_TRUE(includeItem->referencedScope->nodeType == AstNodeType_e::NamespaceDecl);
						ASSERT_TRUE(includeItem->referencedNodeList.size() == 1);
						ASSERT_TRUE(includeItem->referencedNodeList[0]->identifier == TString("FooThree"));
						ASSERT_TRUE(includeItem->referencedNodeList[0]->nodeType == AstNodeType_e::ClassDecl);

						foundFooThree = true;
					}
				}
			}

			Bool foundFooOne = false;
			Bool foundFooThree = false;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { foo::FooOne } in \"source2\"; \n"
			"namespace app { \n"
				"class Foo {}"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"include { loo::FooThree } in \"source3\"; \n"
			"namespace foo { \n"
				"export class FooOne {}"
			"} \n"
		);

		compiler->addBlockToBuild("source3",
			"namespace loo { \n"
				"export class FooThree {}"
			"} \n"
		);

		compiler->build();

		EXPECT_TRUE(checkResult->foundFooOne && checkResult->foundFooThree);
	}

	TEST_F(TransformationResolveInclude, TestIncludeTwo)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());

			compiler->addBlockToBuild("source1",
				"include { foo::FooOne } in \"source2\"; \n"
				"namespace app { \n"
					"class Foo {}"
				"} \n"
			);

			compiler->addBlockToBuild("source2",
				"include { loo::FooThree } in \"source3\"; \n"
				"namespace foo { \n"
					"class FooOne {}"
				"} \n"
			);

			compiler->addBlockToBuild("source3",
				"namespace loo { \n"
					"export class FooThree {}"
				"} \n"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected ending");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Trying to attempt a unexportable element 'FooOne' at: line 1, column 11");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(TransformationResolveInclude, TestIncludeThree)
	{
		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		compiler->addBlockToBuild("source1",
			"include { foo::* } in \"source2\"; \n"
			"namespace app { \n"
				"class Foo {}"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"include { loo::FooFour } in \"source3\"; \n"
			"namespace foo { \n"
				"export class FooOne {}"
				"export class FooTwo {}"
				"class FooThree {}"
			"} \n"
		);

		compiler->addBlockToBuild("source3",
			"namespace loo { \n"
				"export class FooFour {}"
			"} \n"
		);

		compiler->build();
	}

} }