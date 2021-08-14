#include <memory>
#include <filesystem>
#include "gtest/gtest.h"

#include "fl_parser.h"
#include "fl_exceptions.h"
#include "parser_objects/fl_parser_objects.h"


namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserObjectTest
	 */

	struct ParserObjectTest : public ::testing::Test
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

	TEST_F(ParserObjectTest, TestParseClassEmptyNoExportNoGenericNoExtendsNoImplements)
	{
		parser->loadSource("class Foo {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClass::parse(parser.get(), false, false);

		ASSERT_FALSE(classObject == nullptr);

		EXPECT_EQ(classObject->name, "Foo");
	}
} }