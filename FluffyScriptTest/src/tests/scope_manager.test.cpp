#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_buffer.h"
#include "parser\fl_parser.h"
#include "scope\fl_scope_manager.h"
#include "utils\fl_scope_utils.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {

	/**
		* ValidationDuplicationTest
		*/

	struct ValidationDuplicationTest : public ::testing::Test
	{
		std::unique_ptr<parser::Parser> parser;
		parser::ParserContext_s ctx;

		// Antes de cada test
		virtual void SetUp() override {
			parser = std::make_unique<parser::Parser>(new DirectBuffer());
		}
	};

	/**
	 * Testing
	 */

	TEST_F(ValidationDuplicationTest, TestSimpleScope)
	{
		class Validate : public scope::ProcessNode
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onBeginProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					// Testa informacoes do root: codeUnit
					{
						auto root = scopeManager->getRootScope();

						auto appIdentifier = TString("app");
						auto appNode = root.findNodeById(appIdentifier);

						EXPECT_EQ(appNode.size(), 1);

						EXPECT_EQ(appNode[0]->identifier, appIdentifier);
						EXPECT_EQ(appNode[0]->nodeType, AstNodeType_e::NamespaceDecl);

						// Testa informacoes do namespace app
						{
							auto appChildren = utils::ScopeUtils::getPositionalSimplifiedNodeChildrenMap(root.getNode(), appNode[0]);

							EXPECT_EQ(appChildren.size(), 1);

							auto fooIdentifier = TString("Foo");
							auto fooNode = appChildren.find(fooIdentifier);

							EXPECT_NE(fooNode, appChildren.end());

							EXPECT_EQ(fooNode->first, fooIdentifier);
							EXPECT_EQ(fooNode->second->nodeType, AstNodeType_e::ClassDecl);

							// Testa informacoes do namespace Foo
							{
								auto fooChildren = utils::ScopeUtils::getPositionalSimplifiedNodeChildrenMap(appNode[0], fooNode->second);

								EXPECT_EQ(fooChildren.size(), 1);

								auto mainIdentifier = TString("main");
								auto mainNode = fooChildren.find(mainIdentifier);

								EXPECT_NE(mainNode, fooChildren.end());

								EXPECT_EQ(mainNode->first, mainIdentifier);
								EXPECT_EQ(mainNode->second->nodeType, AstNodeType_e::ClassFunctionDecl);

								// Testa informacoes do parent node: fn main()
								{
									auto mainChildren = utils::ScopeUtils::getPositionalSimplifiedNodeChildrenMap(fooNode->second, mainNode->second);

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
		scopeManager->applyOnTree(codeUnit.get(), &validation);
	}

	TEST_F(ValidationDuplicationTest, TestScopeWithParameter)
	{
		class Validate : public scope::ProcessNode
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onBeginProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					EXPECT_EQ(aNodeList.size(), 2);

					EXPECT_EQ(aNodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[0]->nodeType, AstNodeType_e::StmtVariable);

					EXPECT_EQ(aNodeList[1]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[1]->nodeType, AstNodeType_e::FunctionParameterDecl);
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
		scopeManager->applyOnTree(codeUnit.get(), &validation);
	}

	TEST_F(ValidationDuplicationTest, TestScopeWithPatternParameter)
	{
		class Validate : public scope::ProcessNode
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onBeginProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					EXPECT_EQ(aNodeList.size(), 4);

					EXPECT_EQ(aNodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[0]->nodeType, AstNodeType_e::StmtVariable);

					EXPECT_EQ(aNodeList[1]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[1]->nodeType, AstNodeType_e::LiteralPattern);

					EXPECT_EQ(aNodeList[2]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[2]->nodeType, AstNodeType_e::StructureItemPatternDecl);

					EXPECT_EQ(aNodeList[3]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[3]->nodeType, AstNodeType_e::FunctionParameterDecl);
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
		scopeManager->applyOnTree(codeUnit.get(), &validation);
	}

	TEST_F(ValidationDuplicationTest, TestScopeWithPatternParameterAndPatternVariable)
	{
		class Validate : public scope::ProcessNode
		{
		public:
			Validate() {}
			~Validate() {}

			virtual void
			onBeginProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::StmtVariable)
				{
					ast::safe_cast<ast::stmt::StmtVariableDecl>(node);

					auto scope = scopeManager->getParentScope();

					auto aIdentifier = TString("a");
					auto aNodeList = scope.findNodeById(aIdentifier);

					EXPECT_EQ(aNodeList.size(), 5);

					EXPECT_EQ(aNodeList[0]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[0]->nodeType, AstNodeType_e::LiteralPattern);

					EXPECT_EQ(aNodeList[1]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[1]->nodeType, AstNodeType_e::StructureItemPatternDecl);

					EXPECT_EQ(aNodeList[2]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[2]->nodeType, AstNodeType_e::LiteralPattern);

					EXPECT_EQ(aNodeList[3]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[3]->nodeType, AstNodeType_e::StructureItemPatternDecl);

					EXPECT_EQ(aNodeList[4]->identifier, aIdentifier);
					EXPECT_EQ(aNodeList[4]->nodeType, AstNodeType_e::FunctionParameterDecl);
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
		scopeManager->applyOnTree(codeUnit.get(), &validation);
	}
} }
