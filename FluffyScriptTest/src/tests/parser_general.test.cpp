#include <memory>
#include "gtest\gtest.h"

#include "parser\fl_parser.h"
#include "parser\parser_objects/fl_parser_objects.h"
#include "lexer\fl_lexer.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserGeneralTest
	 */

	struct ParserGeneralTest : public ::testing::Test
	{
		std::unique_ptr<Lexer> lexer;
		std::unique_ptr<Parser> parser;

		CompilationContext_t ctx;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			parser = std::make_unique<Parser>(
				new Lexer(
					new DirectBuffer()
				)
				);

			ctx.parser = parser.get();
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserGeneralTest, TestClassDeclaration)
	{
		ctx.parser->loadSource("class Foo {}");
		ctx.parser->nextToken();

		parser_objects::ParserObjectGeneralDecl::parse(&ctx);
	}
} }