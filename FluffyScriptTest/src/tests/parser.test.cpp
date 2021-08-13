#include <memory>
#include <filesystem>
#include "gtest/gtest.h"

#include "fl_parser.h"
#include "fl_exceptions.h"


namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserTest
	 */

	struct ParserTest : public ::testing::Test
	{
		std::unique_ptr<Lexer> lexer;
		std::unique_ptr<Parser> parser;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			if (std::filesystem::exists(".\\s_cache"))
			{
				std::filesystem::remove(".\\s_cache");
			}

			parser = std::make_unique<Parser>(
				new Lexer(
					new DirectBuffer()
				)
			);
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserTest, TestParseEmpty)
	{
		parser->loadSource("");
		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeOnlyOneIdentifier)
	{
		parser->loadSource("include { print } from std;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 1);
		EXPECT_EQ(treeAst->namespaceList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[0], "print");

		EXPECT_EQ(treeAst->includeList[0]->fromNamespace->identifier, "std");
	}

	TEST_F(ParserTest, TestParseIncludeTwoIdentifier)
	{
		parser->loadSource("include { print, scan } from std;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 1);
		EXPECT_EQ(treeAst->namespaceList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[0], "print");
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[1], "scan");

		EXPECT_EQ(treeAst->includeList[0]->fromNamespace->identifier, "std");
	}

	TEST_F(ParserTest, TestParseIncludeAll)
	{
		parser->loadSource("include { * } from std::math;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 1);
		EXPECT_EQ(treeAst->namespaceList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeList[0]->fromNamespace->tailDecl->identifier, "math");
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeTwoIdentifier)
	{
		parser->loadSource("include { print, scan } from std;\ninclude { Window } from std::UI;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 2);
		EXPECT_EQ(treeAst->namespaceList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[0], "print");
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[1], "scan");

		EXPECT_EQ(treeAst->includeList[0]->fromNamespace->identifier, "std");

		EXPECT_EQ(treeAst->includeList[1]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[1]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeList[1]->includedItemList[0], "Window");

		EXPECT_EQ(treeAst->includeList[1]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeList[1]->fromNamespace->tailDecl->identifier, "UI");
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeAll)
	{
		parser->loadSource("include { * } from std;\ninclude { * } from UI;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 2);
		EXPECT_EQ(treeAst->namespaceList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->fromNamespace->identifier, "std");

		EXPECT_EQ(treeAst->includeList[1]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[1]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeList[1]->fromNamespace->identifier, "UI");
	}

	TEST_F(ParserTest, TestParseMultiplesInclude)
	{
		parser->loadSource("include { print2, scan } from std;\ninclude { * } from UI;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 2);
		EXPECT_EQ(treeAst->namespaceList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[0], "print2");
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[1], "scan");

		EXPECT_EQ(treeAst->includeList[0]->fromNamespace->identifier, "std");

		EXPECT_EQ(treeAst->includeList[1]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[1]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeList[1]->fromNamespace->identifier, "UI");
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceEmpty)
	{
		parser->loadSource("namespace application { }");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList.size(), 1);

		EXPECT_EQ(treeAst->namespaceList[0]->name, "application");
		EXPECT_EQ(treeAst->namespaceList[0]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceEmpty)
	{
		parser->loadSource("namespace application { }\nnamespace testing {}");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList.size(), 2);

		EXPECT_EQ(treeAst->namespaceList[0]->name, "application");
		EXPECT_EQ(treeAst->namespaceList[0]->generalDeclList.size(), 0);

		EXPECT_EQ(treeAst->namespaceList[1]->name, "testing");
		EXPECT_EQ(treeAst->namespaceList[1]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList.size(), 1);

		EXPECT_EQ(treeAst->namespaceList[0]->name, "application");
		EXPECT_EQ(treeAst->namespaceList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList[0]->namespaceList.size(), 1);

		EXPECT_EQ(treeAst->namespaceList[0]->namespaceList[0]->name, "detail");
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList.size(), 2);

		EXPECT_EQ(treeAst->namespaceList[0]->name, "application");
		EXPECT_EQ(treeAst->namespaceList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList[0]->namespaceList.size(), 1);

		EXPECT_EQ(treeAst->namespaceList[0]->namespaceList[0]->name, "detail");

		EXPECT_EQ(treeAst->namespaceList[1]->name, "testing");
		EXPECT_EQ(treeAst->namespaceList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList[1]->namespaceList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMixedIncludeAndNamespace)
	{
		parser->loadSource("include { print } from std;\nnamespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 1);
		EXPECT_EQ(treeAst->namespaceList.size(), 2);

		// Include
		EXPECT_EQ(treeAst->includeList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeList[0]->includedItemList[0], "print");

		// Namespace
		EXPECT_EQ(treeAst->namespaceList[0]->name, "application");
		EXPECT_EQ(treeAst->namespaceList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList[0]->namespaceList.size(), 1);

		EXPECT_EQ(treeAst->namespaceList[0]->namespaceList[0]->name, "detail");

		EXPECT_EQ(treeAst->namespaceList[1]->name, "testing");
		EXPECT_EQ(treeAst->namespaceList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceList[1]->namespaceList.size(), 0);
	}
} }