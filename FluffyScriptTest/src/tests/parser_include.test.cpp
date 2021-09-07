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

	TEST_F(ParserIncludeTest, TestIncludeDecl_All)
	{
		parser->loadSource("include { * } in std;");

		auto inc = parser->parseInclude(ctx);

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 0);

		EXPECT_NE(inc->inNamespace, nullptr);
		EXPECT_EQ(inc->inNamespace->identifier, "std");
		EXPECT_EQ(inc->inNamespace->startFromRoot, false);
		EXPECT_EQ(inc->inNamespace->referencedIdentifier, nullptr);
	}

	TEST_F(ParserIncludeTest, TestIncludeDecl_OneInc)
	{
		parser->loadSource("include { print } in std;");

		auto inc = parser->parseInclude(ctx);

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 1);
		EXPECT_EQ(inc->includedItemList[0]->identifier, "print");

		EXPECT_NE(inc->inNamespace, nullptr);
		EXPECT_EQ(inc->inNamespace->identifier, "std");
		EXPECT_EQ(inc->inNamespace->startFromRoot, false);
		EXPECT_EQ(inc->inNamespace->referencedIdentifier, nullptr);
	}

	TEST_F(ParserIncludeTest, TestIncludeDecl_TwoInc)
	{
		parser->loadSource("include { print, scan } in std;");

		auto inc = parser->parseInclude(ctx);

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 2);
		EXPECT_EQ(inc->includedItemList[0]->identifier, "print");
		EXPECT_EQ(inc->includedItemList[1]->identifier, "scan");

		EXPECT_NE(inc->inNamespace, nullptr);
		EXPECT_EQ(inc->inNamespace->identifier, "std");
		EXPECT_EQ(inc->inNamespace->startFromRoot, false);
		EXPECT_EQ(inc->inNamespace->referencedIdentifier, nullptr);
	}

	TEST_F(ParserIncludeTest, TestIncludeDecl_TwoInc_ScopedRoot)
	{
		parser->loadSource("include { print, scan } in ::std::next;");

		auto inc = parser->parseInclude(ctx);

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 2);
		EXPECT_EQ(inc->includedItemList[0]->identifier, "print");
		EXPECT_EQ(inc->includedItemList[1]->identifier, "scan");

		EXPECT_NE(inc->inNamespace, nullptr);
		EXPECT_EQ(inc->inNamespace->identifier, "std");
		EXPECT_EQ(inc->inNamespace->startFromRoot, true);
		EXPECT_EQ(inc->inNamespace->referencedIdentifier->identifier, "next");
	}
} }