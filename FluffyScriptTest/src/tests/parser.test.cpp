#include <memory>
#include "gtest/gtest.h"
#include "test.h"

#include "parser\fl_parser.h"
#include "validation\fl_validate.h"
#include "utils\fl_info_util.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * ParserTest
	 */

	struct ParserTest : public ::testing::Test
	{
		std::unique_ptr<Parser> parser;
		fluffy::parser::ParserContext_s ctx{ false };

		// Sets up the test fixture.
		virtual void SetUp()
		{
			parser = std::make_unique<Parser>(
				new DirectBuffer()
			);
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserTest, TestParseEmpty)
	{
		parser->loadSource("");
		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeOnlyOneIdentifier)
	{
		parser->loadSource("include { print } in std;");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includedItemList[0]->identifier, "print");

		EXPECT_EQ(treeAst->inNamespace->identifier, "std");
		EXPECT_EQ(treeAst->inNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->inNamespace->referencedIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeTwoIdentifier)
	{
		parser->loadSource("include { print, scan } in std;");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);
		
		EXPECT_EQ(treeAst->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includedItemList[0]->identifier, "print");
		EXPECT_EQ(treeAst->includedItemList[1]->identifier, "scan");

		EXPECT_EQ(treeAst->inNamespace->identifier, "std");
		EXPECT_EQ(treeAst->inNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->inNamespace->referencedIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeAll)
	{
		parser->loadSource("include { * } in std::math;");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->inNamespace->identifier, "std");
		EXPECT_EQ(treeAst->inNamespace->startFromRoot, false);
	
		EXPECT_EQ(treeAst->inNamespace->referencedIdentifier->identifier, "math");
		EXPECT_EQ(treeAst->inNamespace->referencedIdentifier->startFromRoot, false);
		EXPECT_EQ(treeAst->inNamespace->referencedIdentifier->referencedIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeTwoIdentifierWithGeneric)
	{
		parser->loadSource("include { print, scan } in std;\ninclude { Window } in std::UI;");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);
		
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->identifier, "print");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1]->identifier, "scan");

		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->referencedIdentifier, nullptr);

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0]->identifier, "Window");

		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->startFromRoot, false);

		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->referencedIdentifier->identifier, "UI");
		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->referencedIdentifier->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->referencedIdentifier->referencedIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeAll)
	{
		parser->loadSource("include { * } in std;\ninclude { * } in UI;");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);

		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 0);
				
		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->referencedIdentifier, nullptr);

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->identifier, "UI");
		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->referencedIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseMultiplesInclude)
	{
		parser->loadSource("include { print2, scan } in std;\ninclude { * } in UI;");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);

		
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->identifier, "print2");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1]->identifier, "scan");

		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->inNamespace->referencedIdentifier, nullptr);

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->identifier, "UI");
		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[1]->inNamespace->referencedIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceEmpty)
	{
		parser->loadSource("namespace application { }");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceEmpty)
	{
		parser->loadSource("namespace application { }\nnamespace testing {}");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);

		EXPECT_EQ(treeAst->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->namespaceDeclList[1]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");

		EXPECT_EQ(treeAst->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMixedIncludeAndNamespace)
	{
		parser->loadSource("include { print } in std;\nnamespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 2);

		// Include
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->identifier, "print");

		// Namespace
		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");

		EXPECT_EQ(treeAst->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestDestructuringGetChildren)
	{
		parser->loadSource(
			"Test;\n"
			"123;\n"
			"{ Test };\n"
			"(Test, Test, 0);\n"
			"{ Test: Test2 };\n"
			"{ Test: { Test2 } };\n"
			"{ Test: { Test: Test2 } };\n"
			"({ Test: { Test: Test1 } }, Test2, Test3)"
		);

		auto pattern1 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern2 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern3 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern4 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern5 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern6 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern7 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern8 = parser->parsePattern(ctx);
	}

	class ValidateTest : public validations::Validation
	{
	public:
		ValidateTest()
			: m_nodeCount(0)
		{}

		virtual ~ValidateTest()
		{}

		U32
		getNodeCount()
		{
			return m_nodeCount;
		}

	protected:
		virtual void
		beginValidation(ast::AstNode* const decl) override
		{
			m_nodeCount++;
		}

		virtual void
		endValidation(ast::AstNode* const decl) override
		{}

	private:
		U32 m_nodeCount;

	};

	TEST_F(ParserTest, TestValidation)
	{
		utils::InfoUtil::resetNodeCount();

		parser->loadSourceFromFile(getProjectFilePath("files\\compiler\\main.txt").c_str());

		auto codeUnit = parser->parseCodeUnit(ctx);

		std::unique_ptr<ValidateTest> validate = std::make_unique<ValidateTest>();
		validate->validate(codeUnit.get());

		EXPECT_EQ(utils::InfoUtil::getNodeCount(), validate->getNodeCount());
	}
} }