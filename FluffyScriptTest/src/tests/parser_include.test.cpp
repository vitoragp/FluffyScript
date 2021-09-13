#include <memory>
#include "gtest\gtest.h"

#include "parser\fl_parser.h"
#include "ast\fl_ast_decl.h"
#include "fl_buffer.h"

namespace fluffy { namespace testing {

	/**
	 * ParserIncludeTest
	 */

	struct ParserIncludeTest : public ::testing::Test
	{
		std::unique_ptr<fluffy::parser::Parser> parser;
		fluffy::parser::ParserContext_s ctx{ false };

		// Sets up the test fixture.
		virtual void SetUp()
		{
			parser = std::make_unique<fluffy::parser::Parser>(
				new DirectBuffer()
			);
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserIncludeTest, TestIncludeDecl_All)
	{
		parser->loadSource("include { io::* } in \"\\system\";");

		auto inc = parser->parseInclude(ctx);

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 1);
		EXPECT_EQ(((ast::IncludeItemDecl*)inc->includedItemList[0].get())->includeAll, true);
		EXPECT_EQ(((ast::IncludeItemDecl*)inc->includedItemList[0].get())->scopePath->identifier, "io");

		EXPECT_EQ(inc->inFile, "\\system");
	}

	TEST_F(ParserIncludeTest, TestIncludeDecl_TwoInc)
	{
		parser->loadSource("include { io::*, ui::* } in \"\\system\";");

		auto inc = parser->parseInclude(ctx);

		ASSERT_TRUE(inc != nullptr);

		EXPECT_EQ(inc->includedItemList.size(), 2);
		EXPECT_EQ(((ast::IncludeItemDecl*)inc->includedItemList[0].get())->includeAll, true);
		EXPECT_EQ(((ast::IncludeItemDecl*)inc->includedItemList[0].get())->scopePath->identifier, "io");
		EXPECT_EQ(((ast::IncludeItemDecl*)inc->includedItemList[1].get())->includeAll, true);
		EXPECT_EQ(((ast::IncludeItemDecl*)inc->includedItemList[1].get())->scopePath->identifier, "ui");

		EXPECT_EQ(inc->inFile, "\\system");
	}
} }