#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_buffer.h"
#include "parser\fl_parser.h"
#include "fl_exceptions.h"

namespace fluffy {
	namespace testing {

		/**
		 * ValidationTypesTest
		 */

		struct ValidationTypesTest : public ::testing::Test
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

		TEST_F(ValidationTypesTest, TestOne)
		{
			parser->loadSource("");
			parser->parseCodeUnit(ctx);
		}

	}
}