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
		parser->loadSource("include { print } from \"std\";");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 1);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->identifierList.size(), 1);
		EXPECT_EQ(treeAst->includeList[0]->identifierList[0], "print");

		EXPECT_EQ(treeAst->includeList[0]->from, "std");
	}

	TEST_F(ParserTest, TestParseIncludeTwoIdentifier)
	{
		parser->loadSource("include { print, scan } from \"std\";");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 1);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->identifierList.size(), 2);
		EXPECT_EQ(treeAst->includeList[0]->identifierList[0], "print");
		EXPECT_EQ(treeAst->includeList[0]->identifierList[1], "scan");

		EXPECT_EQ(treeAst->includeList[0]->from, "std");
	}

	TEST_F(ParserTest, TestParseIncludeAll)
	{
		parser->loadSource("include { * } from \"std\";");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 1);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[0]->identifierList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->from, "std");
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeTwoIdentifier)
	{
		parser->loadSource("include { print, scan } from \"std\";\ninclude { Window } from \"UI\";");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 2);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->identifierList.size(), 2);
		EXPECT_EQ(treeAst->includeList[0]->identifierList[0], "print");
		EXPECT_EQ(treeAst->includeList[0]->identifierList[1], "scan");

		EXPECT_EQ(treeAst->includeList[0]->from, "std");

		EXPECT_EQ(treeAst->includeList[1]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[1]->identifierList.size(), 1);
		EXPECT_EQ(treeAst->includeList[1]->identifierList[0], "Window");

		EXPECT_EQ(treeAst->includeList[1]->from, "UI");
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeAll)
	{
		parser->loadSource("include { * } from \"std\";\ninclude { * } from \"UI\";");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 2);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[0]->identifierList.size(), 0);

		EXPECT_EQ(treeAst->includeList[0]->from, "std");

		EXPECT_EQ(treeAst->includeList[1]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[1]->identifierList.size(), 0);

		EXPECT_EQ(treeAst->includeList[1]->from, "UI");
	}

	TEST_F(ParserTest, TestParseMultiplesInclude)
	{
		parser->loadSource("include { print2, scan } from \"std\";\ninclude { * } from \"UI\";");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeList.size(), 2);

		EXPECT_EQ(treeAst->includeList[0]->allFlag, false);
		EXPECT_EQ(treeAst->includeList[0]->identifierList.size(), 2);
		EXPECT_EQ(treeAst->includeList[0]->identifierList[0], "print2");
		EXPECT_EQ(treeAst->includeList[0]->identifierList[1], "scan");

		EXPECT_EQ(treeAst->includeList[0]->from, "std");

		EXPECT_EQ(treeAst->includeList[1]->allFlag, true);
		EXPECT_EQ(treeAst->includeList[1]->identifierList.size(), 0);

		EXPECT_EQ(treeAst->includeList[1]->from, "UI");
	}
} }