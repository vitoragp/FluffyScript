#include <memory>
#include "gtest\gtest.h"

#include "parser\fl_parser.h"
#include "ast\fl_ast_decl.h"
#include "fl_buffer.h"

namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * ParserIncludeTest
	 */

	struct ParserIncludeTest : public ::testing::Test
	{
		std::unique_ptr<Parser> parser;
		fluffy::parser::ParserContext_s ctx{ true, false, false };

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

	TEST_F(ParserIncludeTest, TestIncludeDecl_All)
	{
		parser->loadSource("include { * } from std;");

		auto inc = parser->parseInclude(ctx);

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

		auto inc = parser->parseInclude(ctx);

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

		auto inc = parser->parseInclude(ctx);

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

		auto inc = parser->parseInclude(ctx);

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