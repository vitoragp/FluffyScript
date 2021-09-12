#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_buffer.h"
#include "parser\fl_parser.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {

	/**
	 * ValidationDuplicationTest
	 */

	struct ValidationDuplicationTest : public ::testing::Test
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
	/*
	TEST_F(ValidationDuplicationTest, TestDuplicatedNamespace)
	{
		parser->loadSource(
			"namespace app {}\n"
			"namespace app {}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'app' at: line 2, column 1");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameter)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, a: u32) {} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 2, column 17");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternOne)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, { a }: Foo) {} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 2, column 19");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternTwo)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, { a: a }: Foo) {} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 2, column 22");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternThree)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, { a: (a) }: Foo) {} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 2, column 23");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternFour)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, { a: ({ a }) }: Foo) {} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 2, column 25");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternFive)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, { a: ({ b: a }) }: Foo) {} \n"
				"class Foo {}\n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 2, column 28");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternSix)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, { a: ({ a: b }) }: Foo) { \n"
					"let a = 0; \n"
				"} \n"
				"class Foo {} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 3, column 1");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternSeven)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main(a: u32, { a: ({ a: b }) }: Foo) { \n"
					"let { a } = foo;"
				"} \n"
				"class Foo {}\n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 3, column 7");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternEight)
	{
		parser->loadSource(
			"namespace app { \n"
				"class Foo {}\n"
				"fn main(a: u32, { a: ({ a: b }) }: Foo) { \n"
					"let { a: { a: (a)} } = foo;"
				"} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 4, column 16");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedFunction)
	{
		parser->loadSource(
			"namespace app { \n"
				"class Foo {} \n"
				"class Tonho {} \n"
				"fn main(a: u32, { a: ({ a: b }) }: Foo) {}\n"
				"fn main(a: u32, b: bool) {}\n"
				"fn main(a: u32, b: i8) {}\n"
				"fn main(a: u32, b: u8) {}\n"
				"fn main(a: u32, b: Tonho) {}\n"
				"fn main(a: u32, b: app::Foo) {}\n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'main' at: line 9, column 1");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}

	TEST_F(ValidationDuplicationTest, TestLambdaOne)
	{
		parser->loadSource(
			"namespace app { \n"
				"fn main() { \n"
					"let a = |a, a| {}; \n"
				"} \n"
			"}"
		);
		auto codeUnit = parser->parseCodeUnit(ctx);

		try
		{
			NodeMultiMap map;
			map.emplace(codeUnit->name.c_str(), codeUnit.get());

			auto validation = validations::DuplicatedValidation(map);
			validation.validate(codeUnit.get());

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "anom_block: error duplicated identifier 'a' at: line 3, column 13");
		}
		catch (std::exception&)
		{
			ASSERT_ANY_THROW("Unexpected exception");
		}
	}
	*/
} }