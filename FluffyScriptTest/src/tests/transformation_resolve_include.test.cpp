#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "ast\fl_ast_decl.h"
#include "attributes\fl_included_scope.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "scope\fl_scope_manager.h"
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
			onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::CodeUnit && event == scope::NodeProcessorEvent_e::onBegin)
				{
					auto includedScope = node->getAttribute<attributes::IncludedScope>();

					if (node->identifier == "source1") {
						auto includeItem = includedScope->includeList()[0];
						ASSERT_EQ(includeItem.node->identifier, "FooOne");
						ASSERT_EQ(includeItem.node->nodeType, AstNodeType_e::ClassDecl);
						ASSERT_EQ(includeItem.scope->identifier, "foo");
						ASSERT_EQ(includeItem.scope->nodeType, AstNodeType_e::NamespaceDecl);
						passes++;
					}

					if (node->identifier == "source2") {
						auto includeItem = includedScope->includeList()[0];
						ASSERT_EQ(includeItem.node->identifier, "FooThree");
						ASSERT_EQ(includeItem.node->nodeType, AstNodeType_e::ClassDecl);
						ASSERT_EQ(includeItem.scope->identifier, "loo");
						ASSERT_EQ(includeItem.scope->nodeType, AstNodeType_e::NamespaceDecl);
						passes++;
					}
				}
			}

			U32 passes = false;
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

		EXPECT_EQ(checkResult->passes, 2);
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
			ASSERT_STREQ(e.what(), "source1 error: Trying to attempt include a unimportable element 'FooOne' at: line 1, column 11");
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

	TEST_F(TransformationResolveInclude, TestIncludeFour)
	{
		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		compiler->addBlockToBuild("source1",
			"include { foo::FooTwo::p } in \"source2\"; \n"
			"namespace app { \n"
				"class Foo {}"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"namespace foo { \n"
				"export class FooOne {}"
				"export class FooTwo { static fn p<T>() { let a = 0; } }"
				"class FooThree { }"
			"} \n"
		);

		compiler->build();
	}

	TEST_F(TransformationResolveInclude, TestIncludeFive)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult()
			{}

			virtual ~CheckResult()
			{}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::CodeUnit && event == scope::NodeProcessorEvent_e::onBegin)
				{
					auto includedScope = node->getAttribute<attributes::IncludedScope>();

					if (node->identifier == "source1") {
						auto weakList = includedScope->weakIncludeList();
						ASSERT_EQ(weakList.size(), 2);

						auto includeItem1 = weakList[0];
						ASSERT_EQ(includeItem1.node->identifier, "FooOne");
						ASSERT_EQ(includeItem1.node->nodeType, AstNodeType_e::ClassDecl);
						ASSERT_EQ(includeItem1.scope->identifier, "foo");
						ASSERT_EQ(includeItem1.scope->nodeType, AstNodeType_e::NamespaceDecl);

						auto includeItem2 = weakList[1];
						ASSERT_EQ(includeItem2.node->identifier, "FooTwo");
						ASSERT_EQ(includeItem2.node->nodeType, AstNodeType_e::ClassDecl);
						ASSERT_EQ(includeItem2.scope->identifier, "foo");
						ASSERT_EQ(includeItem2.scope->nodeType, AstNodeType_e::NamespaceDecl);
						passes++;

						auto list = includedScope->includeList();

						ASSERT_EQ(list.size(), 1);

						auto includeItem3 = list[0];
						ASSERT_EQ(includeItem3.node->identifier, "p");
						ASSERT_EQ(includeItem3.node->nodeType, AstNodeType_e::ClassVariableDecl);
						ASSERT_EQ(includeItem3.scope->identifier, "FooOne");
						ASSERT_EQ(includeItem3.scope->nodeType, AstNodeType_e::ClassDecl);
					}

					if (node->identifier == "source2") {
						auto includeItem = includedScope->includeList()[0];
						ASSERT_EQ(includeItem.node->identifier, "FooFour");
						ASSERT_EQ(includeItem.node->nodeType, AstNodeType_e::ClassDecl);
						ASSERT_EQ(includeItem.scope->identifier, "loo");
						ASSERT_EQ(includeItem.scope->nodeType, AstNodeType_e::NamespaceDecl);
						passes++;
					}
				}
			}

			U32 passes = false;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { foo::*, foo::FooOne::p } in \"source2\"; \n"
			"namespace app { \n"
				"class Foo {}"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"include { loo::FooFour } in \"source3\"; \n"
			"namespace foo { \n"
				"export class FooOne { static let p = 0; }"
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

		EXPECT_EQ(checkResult->passes, 2);
	}
} }