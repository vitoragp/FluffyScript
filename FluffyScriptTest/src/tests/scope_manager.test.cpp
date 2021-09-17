#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_buffer.h"
#include "ast\fl_ast_decl.h"
#include "parser\fl_parser.h"
#include "scope\fl_scope_manager.h"
#include "utils\fl_scope_utils.h"
#include "attributes\fl_included_scope.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "fl_compiler.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {

	/**
	 * ScopeManagerTest
	 */

	struct ScopeManagerTest : public ::testing::Test
	{
		std::unique_ptr<fluffy::Compiler> compiler;
		std::unique_ptr<parser::Parser> parser;
		parser::ParserContext_s ctx = { false };

		// Antes de cada test
		virtual void SetUp() override {
			parser = std::make_unique<parser::Parser>(new DirectBuffer());
			compiler = std::make_unique<fluffy::Compiler>();
		}
	};

	/**
	 * Testing
	 */

	TEST_F(ScopeManagerTest, TestSimpleScope)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					// Testa informacoes do root: codeUnit
					{
						auto root = scopeManager->getRootScope();

						auto appIdentifier = TString("app");
						auto appNode = root.findNodeById(appIdentifier);

						EXPECT_EQ(appNode.foundResult, true);

						EXPECT_EQ(appNode.nodeList[0]->identifier, appIdentifier);
						EXPECT_EQ(appNode.nodeList[0]->nodeType, AstNodeType_e::NamespaceDecl);

						// Testa informacoes do namespace app
						{
							auto appChildren = utils::ScopeUtils::getPositionalSimplifiedNodeChildrenMap(scopeManager, root.getNode(), appNode.nodeList[0]);

							EXPECT_EQ(appChildren.size(), 1);

							auto fooIdentifier = TString("Foo");
							auto fooNode = appChildren.find(fooIdentifier);

							EXPECT_NE(fooNode, appChildren.end());

							EXPECT_EQ(fooNode->first, fooIdentifier);
							EXPECT_EQ(fooNode->second->nodeType, AstNodeType_e::ClassDecl);

							// Testa informacoes do namespace Foo
							{
								auto fooChildren = utils::ScopeUtils::getPositionalSimplifiedNodeChildrenMap(scopeManager, appNode.nodeList[0], fooNode->second);

								EXPECT_EQ(fooChildren.size(), 1);

								auto mainIdentifier = TString("main");
								auto mainNode = fooChildren.find(mainIdentifier);

								EXPECT_NE(mainNode, fooChildren.end());

								EXPECT_EQ(mainNode->first, mainIdentifier);
								EXPECT_EQ(mainNode->second->nodeType, AstNodeType_e::ClassFunctionDecl);

								// Testa informacoes do parent node: fn main()
								{
									auto mainChildren = utils::ScopeUtils::getPositionalSimplifiedNodeChildrenMap(scopeManager, fooNode->second, mainNode->second);

									EXPECT_EQ(mainChildren.size(), 1);

									auto aNode = mainChildren.find(node->identifier);

									EXPECT_NE(aNode, mainChildren.end());

									EXPECT_EQ(aNode->first, node->identifier);
									EXPECT_EQ(aNode->second, node);
								}
							}
						}
					}
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main() { \n"
						"let a = 0; \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeWithParameter)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					EXPECT_EQ(aNodeList.foundResult, true);

					EXPECT_EQ(aNodeList.nodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[0]->nodeType, AstNodeType_e::FunctionParameterDecl);

					EXPECT_EQ(aNodeList.nodeList[1]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[1]->nodeType, AstNodeType_e::StmtVariable);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main(a: i32, b: i32) { \n"
						"let a = 0; \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeWithPatternParameter)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					EXPECT_EQ(aNodeList.foundResult, true);

					EXPECT_EQ(aNodeList.nodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[0]->nodeType, AstNodeType_e::FunctionParameterDecl);

					EXPECT_EQ(aNodeList.nodeList[1]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[1]->nodeType, AstNodeType_e::StructureItemPatternDecl);

					EXPECT_EQ(aNodeList.nodeList[2]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[2]->nodeType, AstNodeType_e::LiteralPattern);

					EXPECT_EQ(aNodeList.nodeList[3]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[3]->nodeType, AstNodeType_e::StmtVariable);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main(a: i32, { a, b: (a) }: Loo) { \n"
						"let a = 0; \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeWithPatternParameterAndPatternVariable)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					EXPECT_EQ(aNodeList.foundResult, true);

					EXPECT_EQ(aNodeList.nodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[0]->nodeType, AstNodeType_e::FunctionParameterDecl);

					EXPECT_EQ(aNodeList.nodeList[1]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[1]->nodeType, AstNodeType_e::StructureItemPatternDecl);

					EXPECT_EQ(aNodeList.nodeList[2]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[2]->nodeType, AstNodeType_e::LiteralPattern);

					EXPECT_EQ(aNodeList.nodeList[3]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[3]->nodeType, AstNodeType_e::StructureItemPatternDecl);

					EXPECT_EQ(aNodeList.nodeList[4]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[4]->nodeType, AstNodeType_e::LiteralPattern);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main(a: i32, { a, b: (a) }: Loo) { \n"
						"let { a, b: (a) } = foo; \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeIfElseOne)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aNode = scope.findNodeById(TString("a"));
					auto bNode = scope.findNodeById(TString("b"));

					ASSERT_TRUE(
						(aNode.foundResult == true && bNode.foundResult == false) ||
						(aNode.foundResult == false && bNode.foundResult == true)
					);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main(a: i32, { a, b: (a) }: Loo) { \n"
						"if a { let a = 0; } else { let b = 0; } \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeFor)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aNode = scope.findNodeById(TString("a"));

					ASSERT_TRUE(aNode.foundResult);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main(a: i32, { a, b: (a) }: Loo) { \n"
						"for a = 0; a < 10; a++ { let a = 0; } \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeForInit)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					ASSERT_TRUE(aNodeList.foundResult);

					EXPECT_EQ(aNodeList.nodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[0]->nodeType, AstNodeType_e::StmtForInitDecl);

					EXPECT_EQ(aNodeList.nodeList[1]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[1]->nodeType, AstNodeType_e::StmtVariable);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main(a: i32, { a, b: (a) }: Loo) { \n"
						"for let a = 0; a < 10; a++ { let a = 0; } \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeForInitExpr)
	{
		class Validate : public scope::NodeProcessor
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					if (node->identifier == "a")
					{
						auto scope = scopeManager->getParentScope();

						auto aIdentifier = TString("a");
						auto aNodeList = scope.findNodeById(aIdentifier);

						ASSERT_TRUE(aNodeList.foundResult);

						EXPECT_EQ(aNodeList.nodeList[0]->identifier, aIdentifier);
						EXPECT_EQ(aNodeList.nodeList[0]->nodeType, AstNodeType_e::StmtForInitDecl);

						EXPECT_EQ(aNodeList.nodeList[1]->identifier, aIdentifier);
						EXPECT_EQ(aNodeList.nodeList[1]->nodeType, AstNodeType_e::StmtVariable);
					}
				}

				if (node->nodeType == AstNodeType_e::FunctionParameterDeclExpr)
				{
					ast::safe_cast<ast::expr::ExpressionFunctionParameterDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					ASSERT_TRUE(aNodeList.foundResult);

					EXPECT_EQ(aNodeList.nodeList[0], node);

					EXPECT_EQ(aNodeList.nodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList.nodeList[0]->nodeType, AstNodeType_e::FunctionParameterDeclExpr);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		parser->loadSource(
			"namespace app { \n"
				"class Foo { \n"
					"fn main(a: i32, { a, b: (a) }: Loo) { \n"
						"let b = |a| { for let a = 0; a < 10; a++ { let a = 0; } }; \n"
					"} \n"
				"} \n"
			"}"
		);

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<scope::ScopeManager> scopeManager(new scope::ScopeManager());
		scopeManager->insertCodeUnit(codeUnit.get());

		auto validation = Validate();
		scopeManager->processCodeUnit(codeUnit.get(), &validation);
	}

	TEST_F(ScopeManagerTest, TestScopeWithIncludeOne)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult() {}
			~CheckResult() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (scopeManager->getCodeUnitName() == "source1" && node->identifier == "app")
				{
					auto scope = scopeManager->getRootScope();
					auto scopedItems = scope.toMap();				

					U32 foundedItems = 0;
					for (auto& it : scopedItems)
					{
						if (it.first == "app")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::NamespaceDecl);
							ASSERT_EQ(it.second->identifier, "app");

							foundedItems++;
						}

						if (it.first == "foo")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::NamespaceDecl);
							ASSERT_EQ(it.second->identifier, "foo");

							foundedItems++;
						}

						if (it.first == "FooOne")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::ClassDecl);
							ASSERT_EQ(it.second->identifier, "FooOne");

							foundedItems++;
						}
					}
					ASSERT_EQ(foundedItems, 3);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { loo::FooOne } in \"source2\";"
			"namespace app { \n"
				"class Foo { \n"
					"fn main() { \n"
					"} \n"
				"} \n"
			"} \n"
			"namespace foo {}"
		);

		compiler->addBlockToBuild("source2",
			"namespace loo { \n"
				"export class FooOne {}"
			"}"
		);

		compiler->build();
	}

	TEST_F(ScopeManagerTest, TestScopeWithIncludeTwo)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult() {}
			~CheckResult() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (scopeManager->getCodeUnitName() == "source1" && node->identifier == "app")
				{
					auto scope = scopeManager->getRootScope();
					auto scopedItems = scope.toMap();				

					U32 foundedItems = 0;
					for (auto& it : scopedItems)
					{
						if (it.first == "app")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::NamespaceDecl);
							ASSERT_EQ(it.second->identifier, "app");

							foundedItems++;
						}

						if (it.first == "foo")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::NamespaceDecl);
							ASSERT_EQ(it.second->identifier, "foo");

							foundedItems++;
						}

						if (it.first == "FooTwo")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::ClassDecl);
							ASSERT_EQ(it.second->identifier, "FooOne");

							foundedItems++;
						}
					}
					ASSERT_EQ(foundedItems, 3);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { loo::FooOne as FooTwo } in \"source2\";"
			"namespace app { \n"
				"class Foo { \n"
					"fn main() { \n"
					"} \n"
				"} \n"
			"} \n"
			"namespace foo {}"
		);

		compiler->addBlockToBuild("source2",
			"namespace loo { \n"
				"export class FooOne {}"
			"}"
		);

		compiler->build();
	}

	TEST_F(ScopeManagerTest, TestScopeWithIncludeThree)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult() {}
			~CheckResult() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (scopeManager->getCodeUnitName() == "source1" && node->identifier == "app")
				{
					auto scope = scopeManager->getRootScope();
					auto scopedItems = scope.toMap();

					U32 foundedItems = 0;
					for (auto& it : scopedItems)
					{
						if (it.first == "app")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::NamespaceDecl);
							ASSERT_EQ(it.second->identifier, "app");

							foundedItems++;
						}

						if (it.first == "foo")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::NamespaceDecl);
							ASSERT_EQ(it.second->identifier, "foo");

							foundedItems++;
						}

						if (it.first == "FooOne")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::ClassDecl);
							ASSERT_EQ(it.second->identifier, "FooOne");

							foundedItems++;
						}

						if (it.first == "FooTwo")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::ClassDecl);
							ASSERT_EQ(it.second->identifier, "FooTwo");

							foundedItems++;
						}
					}
					ASSERT_EQ(foundedItems, 4);
				}
			}
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { loo::* } in \"source2\";"
			"namespace app { \n"
				"class Foo { \n"
					"fn main() { \n"
					"} \n"
				"} \n"
			"} \n"
			"namespace foo {}"
		);

		compiler->addBlockToBuild("source2",
			"namespace loo { \n"
				"export class FooOne {}"
				"export class FooTwo {}"
				"class FooThree {}"
			"}"
		);

		compiler->build();
	}

	TEST_F(ScopeManagerTest, TestScopeWithIncludeFour)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult() {}
			~CheckResult() {}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
			{
				if (scopeManager->getCodeUnitName() == "source1" && node->identifier == "app")
				{
					auto scope = scopeManager->getRootScope();
					auto scopedItems = scope.toMap();

					U32 foundedItems = 0;
					for (auto& it : scopedItems)
					{
						if (it.first == "FooOne")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::ClassDecl);
							ASSERT_EQ(it.second->identifier, "FooOne");

							foundedItems++;
						}

						if (it.first == "FooTwo")
						{
							ASSERT_EQ(it.second->nodeType, AstNodeType_e::ClassDecl);
							ASSERT_EQ(it.second->identifier, "FooTwo");

							foundedItems++;
						}
					}
					ASSERT_EQ(foundedItems, 2);
				}
			}

			virtual void
			onEndProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{}
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include { loo::app2::*, loo::app2::FooOne } in \"source2\"; \n"
			"namespace app { \n"
				"class Foo { \n"
					"fn main() { \n"
					"} \n"
				"} \n"
			"} \n"
			"namespace foo {}"
		);

		compiler->addBlockToBuild("source2",
			"namespace loo { \n"
				"namespace app2 { \n"
					"export class FooOne {}"
					"export class FooTwo {}"
					"class FooThree {}"
				"} \n"
			"}"
		);

		compiler->build();
	}
} }