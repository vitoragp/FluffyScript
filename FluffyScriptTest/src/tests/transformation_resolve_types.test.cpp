#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "ast\fl_ast_decl.h"
#include "attributes\fl_reference.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "transformation\fl_transformation_resolve_types.h"
#include "fl_compiler.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {

	/**
	 * TransformationResolveTypes
	 */

	struct TransformationResolveTypes : public ::testing::Test
	{
		std::unique_ptr<fluffy::Compiler> compiler;

		// Antes de cada test
		virtual void SetUp() override {
			compiler = std::make_unique<fluffy::Compiler>();
		}
	};

	void
	validateReference(attributes::Reference* const reference, TString scopeId, TString nodeId, AstNodeType_e scopeType, AstNodeType_e nodeType)
	{
		ASSERT_TRUE(reference != nullptr);

		ASSERT_EQ(reference->referencedScope->identifier, scopeId);
		ASSERT_EQ(reference->referencedScope->nodeType, scopeType);
		ASSERT_EQ(reference->referencedNode->identifier, nodeId);
		ASSERT_EQ(reference->referencedNode->nodeType, nodeType);
	}

	/**
	 * Testing
	 */

	TEST_F(TransformationResolveTypes, TestResolveTypesOne)
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
				if (node->nodeType == AstNodeType_e::NamedType)
				{
					auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

					if (namedType->identifier == TString("FooOne") && namedType->scopePath == nullptr)
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "foo", "FooOne", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
						validationPasses++;
					}
					
					if (namedType->identifier == TString("FooOne") && namedType->scopePath && namedType->scopePath->identifier == TString("subapp"))
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "subapp", "FooOne", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
						validationPasses++;
					}
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { foo::FooOne } in \"source2\"; \n"
			"namespace app { \n"
				"let a: FooOne; \n"
				"let b: subapp::FooOne; \n"
				"namespace subapp { \n"
					"class FooOne {} \n"
				"} \n"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"namespace foo { \n"
				"export class FooOne {}"
			"} \n"
		);

		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 2);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesTwo)
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
				if (node->nodeType == AstNodeType_e::NamedType)
				{
					auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

					if (namedType->identifier == TString("FooOne"))
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "subfoo", "FooOne", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
						validationPasses++;
					}
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { foo::subfoo::FooOne } in \"source2\"; \n"
			"namespace app { \n"
				"let p : FooOne;"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"namespace foo { namespace subfoo { \n"
				"export class FooOne {}"
			"} } \n"
		);

		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 1);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesThree)
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
				if (node->nodeType == AstNodeType_e::NamedType)
				{
					auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

					if (namedType->identifier == TString("FooTwo"))
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "foo", "FooTwo", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
						validationPasses++;
					}
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { foo::* } in \"source2\"; \n"
			"namespace app { \n"
				"let a : FooTwo;"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"namespace foo { \n"
				"export class FooOne {}"
				"export class FooTwo {}"
				"class FooThree {}"
			"} \n"
		);

		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 1);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesFour)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());

			compiler->addBlockToBuild("source1",
				"include { foo::* } in \"source2\"; \n"
				"namespace app { \n"
					"let a : FooThree;"
				"} \n"
			);

			compiler->addBlockToBuild("source2",
				"namespace foo { \n"
					"export class FooOne {}"
					"export class FooTwo {}"
					"class FooThree {}"
				"} \n"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Identifier 'FooThree' not found in scope at: line 3, column 9");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesFive)
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
				if (node->nodeType == AstNodeType_e::NamedType)
				{
					auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

					if (namedType->identifier == TString("FooTwo"))
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "foo", "FooTwo", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
						validationPasses++;
					}
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { foo::* } in \"source2\"; \n"
			"namespace app { \n"
				"let a : FooTwo;"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"namespace foo { \n"
				"export class FooOne {}"
				"export class FooTwo {}"
				"class FooThree {}"
			"} \n"
		);

		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 1);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesSix)
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
				if (node->nodeType == AstNodeType_e::NamedType)
				{
					auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

					if (namedType->identifier == TString("FooOne"))
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "foo2", "FooOne", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
						validationPasses++;
					}
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { foo::* } in \"source2\"; \n"
			"include { foo2::FooOne } in \"source3\"; \n"
			"namespace app { \n"
				"let a : FooOne; \n"
			"} \n"
		);

		compiler->addBlockToBuild("source2",
			"namespace foo { \n"
				"export class FooOne {} \n"
				"export class FooTwo {} \n"
				"class FooThree {} \n"
			"} \n"
		);

		compiler->addBlockToBuild("source3",
			"namespace foo2 { \n"
				"export class FooOne {} \n"
			"} \n"
		);

		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 1);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesGenericTypeOne)
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
				if (node->nodeType == AstNodeType_e::NamedType)
				{
					auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

					if (namedType->identifier == TString("T"))
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "Foo", "T", AstNodeType_e::ClassDecl, AstNodeType_e::GenericItemDecl);
						validationPasses++;
					}
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"class Foo<T> { \n"
					"let p: T; \n"
				"} \n"
			"} \n"
		);


		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 1);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesGenericTypeTwo)
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
				if (node->nodeType == AstNodeType_e::NamedType)
				{
					auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

					if (namedType->identifier == TString("T"))
					{
						validateReference(namedType->getAttribute<attributes::Reference>(), "Foo", "T", AstNodeType_e::ClassDecl, AstNodeType_e::GenericItemDecl);
						validationPasses++;
					}
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"class T {} \n"
				"class Foo<T> { \n"
					"let p: T; \n"
				"} \n"
			"} \n"
		);


		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 1);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesGenericTypeThree)
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
				if (node->nodeType == AstNodeType_e::ClassVariableDecl && node->identifier == "p")
				{
					auto var = ast::safe_cast<ast::ClassVariableDecl>(node);

					validateReference(var->typeDecl->getAttribute<attributes::Reference>(), "app", "T", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
					validationPasses++;
				}

				if (node->nodeType == AstNodeType_e::ClassVariableDecl && node->identifier == "q")
				{
					auto var = ast::safe_cast<ast::ClassVariableDecl>(node);

					validateReference(var->typeDecl->getAttribute<attributes::Reference>(), "Foo", "T", AstNodeType_e::ClassDecl, AstNodeType_e::GenericItemDecl);
					validationPasses++;
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"class T {} \n"
				"class Foo<T> { \n"
					"let p: ::app::T; \n"
					"let q: T; \n"
				"} \n"
			"} \n"
		);

		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 2);
	}

	TEST_F(TransformationResolveTypes, TestResolveTypesGenericTypeFour)
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
				if (node->nodeType == AstNodeType_e::ClassVariableDecl && node->identifier == "p")
				{
					auto var = ast::safe_cast<ast::ClassVariableDecl>(node);

					validateReference(var->typeDecl->getAttribute<attributes::Reference>(), "app", "T", AstNodeType_e::NamespaceDecl, AstNodeType_e::ClassDecl);
					validationPasses++;
				}

				if (node->nodeType == AstNodeType_e::ClassVariableDecl && node->identifier == "q")
				{
					auto var = ast::safe_cast<ast::ClassVariableDecl>(node);

					validateReference(var->typeDecl->getAttribute<attributes::Reference>(), "Foo", "T", AstNodeType_e::ClassDecl, AstNodeType_e::GenericItemDecl);
					validationPasses++;
				}

				if (node->nodeType == AstNodeType_e::ClassFunctionDecl && node->identifier == "main")
				{
					auto var = ast::safe_cast<ast::ClassFunctionDecl>(node);

					validateReference(
						var->parameterList[0]->typeDecl->getAttribute<attributes::Reference>(),
						"main", "T", AstNodeType_e::ClassFunctionDecl, AstNodeType_e::GenericItemDecl
					);
					validationPasses++;
				}
			}

			U32 validationPasses = 0;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"class T {} \n"
				"class Foo<T> { \n"
					"let p: ::app::T; \n"
					"let q: T; \n"
					"fn main<T, W>(a: T, b: W) {}"
				"} \n"
			"} \n"
		);

		compiler->build();

		EXPECT_EQ(checkResult->validationPasses, 3);
	}

} }