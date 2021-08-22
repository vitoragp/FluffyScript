#include <memory>
#include "gtest\gtest.h"

#include "parser\fl_parser.h"
#include "lexer\fl_lexer.h"
#include "parser\fl_ast_decl.h"
#include "parser\parser_objects\fl_parser_objects.h"
#include "fl_buffer.h"

namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserIncludeTest
	 */

	struct ParserIncludeTest : public ::testing::Test
	{
		std::unique_ptr<Lexer> lexer;
		std::unique_ptr<Parser> parser;

		// Sets up the test fixture.
		virtual void SetUp()
		{
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

	TEST_F(ParserIncludeTest, TestIncludeDecl_All)
	{
		parser->loadSource("include { * } from std;");
		parser->nextToken();

		auto inc = parser_objects::ParserObjectIncludeDecl::parse(parser.get());

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 0);

		EXPECT_NE(inc->fromNamespace, nullptr);
		EXPECT_EQ(inc->fromNamespace->identifier, "std");
		EXPECT_EQ(inc->fromNamespace->startFromRoot, false);
		EXPECT_EQ(inc->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserIncludeTest, TestIncludeDecl_OneInc)
	{
		parser->loadSource("include { print } from std;");
		parser->nextToken();

		auto inc = parser_objects::ParserObjectIncludeDecl::parse(parser.get());

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 1);
		EXPECT_EQ(inc->includedItemList[0], "print");

		EXPECT_NE(inc->fromNamespace, nullptr);
		EXPECT_EQ(inc->fromNamespace->identifier, "std");
		EXPECT_EQ(inc->fromNamespace->startFromRoot, false);
		EXPECT_EQ(inc->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserIncludeTest, TestIncludeDecl_TwoInc)
	{
		parser->loadSource("include { print, scan } from std;");
		parser->nextToken();

		auto inc = parser_objects::ParserObjectIncludeDecl::parse(parser.get());

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 2);
		EXPECT_EQ(inc->includedItemList[0], "print");
		EXPECT_EQ(inc->includedItemList[1], "scan");

		EXPECT_NE(inc->fromNamespace, nullptr);
		EXPECT_EQ(inc->fromNamespace->identifier, "std");
		EXPECT_EQ(inc->fromNamespace->startFromRoot, false);
		EXPECT_EQ(inc->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserIncludeTest, TestIncludeDecl_TwoInc_ScopedRoot)
	{
		parser->loadSource("include { print, scan } from ::std::next;");
		parser->nextToken();

		auto inc = parser_objects::ParserObjectIncludeDecl::parse(parser.get());

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 2);
		EXPECT_EQ(inc->includedItemList[0], "print");
		EXPECT_EQ(inc->includedItemList[1], "scan");

		EXPECT_NE(inc->fromNamespace, nullptr);
		EXPECT_EQ(inc->fromNamespace->identifier, "std");
		EXPECT_EQ(inc->fromNamespace->startFromRoot, true);
		EXPECT_EQ(inc->fromNamespace->tailIdentifier->identifier, "next");
	}
} }