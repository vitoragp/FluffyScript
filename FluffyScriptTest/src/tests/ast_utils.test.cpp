#include <memory>
#include "gtest/gtest.h"

#include "ast\fl_ast_decl.h"
#include "parser\fl_parser.h"
#include "utils\fl_ast_utils.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * AstUtilsTest
	 */

	struct AstUtilsTest : public ::testing::Test
	{
		std::unique_ptr<Parser> parser;
		parser::ParserContext_s ctx { false };

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

	TEST_F(AstUtilsTest, TestCompareTypedArrayOne)
	{
		parser->loadSource("u32[][1] u32[][1] u32[][2]");

		auto typeA = parser->parseType(ctx);
		auto typeB = parser->parseType(ctx);
		auto typeC = parser->parseType(ctx);

		ASSERT_TRUE(utils::AstUtils::equals(typeA.get(), typeB.get()));
		ASSERT_FALSE(utils::AstUtils::equals(typeA.get(), typeC.get()));
		ASSERT_FALSE(utils::AstUtils::equals(typeB.get(), typeC.get()));
	}

	TEST_F(AstUtilsTest, TestCompareNamedOne)
	{
		parser->loadSource("::Test::ID, Test::ID, ::Test::ID");

		auto typeA = parser->parseType(ctx); parser->skipToken();
		auto typeB = parser->parseType(ctx); parser->skipToken();
		auto typeC = parser->parseType(ctx);

		ASSERT_FALSE(utils::AstUtils::equals(typeA.get(), typeB.get()));
		ASSERT_FALSE(utils::AstUtils::equals(typeB.get(), typeC.get()));
		ASSERT_TRUE(utils::AstUtils::equals(typeA.get(), typeC.get()));
	}

	TEST_F(AstUtilsTest, TestCompareTraitOne)
	{
		parser->loadSource(
			"trait Foo {}"
			"trait Foo for Simple {}"
			"trait Foo for i32 {}"
			"trait Foo for i32 {}"
			"trait Foo for Simple {}"
		);

		auto traitA = parser->parseTrait(ctx, false);
		auto traitB = parser->parseTrait(ctx, false);
		auto traitC = parser->parseTrait(ctx, false);
		auto traitD = parser->parseTrait(ctx, false);
		auto traitE = parser->parseTrait(ctx, false);

		ASSERT_FALSE(utils::AstUtils::equals(traitA.get(), traitB.get()));
		ASSERT_FALSE(utils::AstUtils::equals(traitA.get(), traitC.get()));
		ASSERT_FALSE(utils::AstUtils::equals(traitB.get(), traitC.get()));

		ASSERT_TRUE(utils::AstUtils::equals(traitC.get(), traitD.get()));
		ASSERT_TRUE(utils::AstUtils::equals(traitB.get(), traitE.get()));
	}

	TEST_F(AstUtilsTest, TestCompareFunctionOne)
	{
		parser->loadSource(
			"fn main(a: i32) {}"
			"fn main(a: i32, b: i32) {}"
			"fn main(a: fp32) {}"
			"fn main(a: fp32, b: i32) {}"
			"fn main(a: i32) {}"
			"fn main(a: fp32, b: i32) {}"
		);

		auto traitA = parser->parseFunction(ctx, false);
		auto traitB = parser->parseFunction(ctx, false);
		auto traitC = parser->parseFunction(ctx, false);
		auto traitD = parser->parseFunction(ctx, false);
		auto traitE = parser->parseFunction(ctx, false);
		auto traitF = parser->parseFunction(ctx, false);

		ASSERT_FALSE(utils::AstUtils::equals(traitA.get(), traitB.get()));
		ASSERT_FALSE(utils::AstUtils::equals(traitA.get(), traitC.get()));
		ASSERT_FALSE(utils::AstUtils::equals(traitB.get(), traitC.get()));
		ASSERT_FALSE(utils::AstUtils::equals(traitC.get(), traitD.get()));

		ASSERT_TRUE(utils::AstUtils::equals(traitA.get(), traitE.get()));
		ASSERT_TRUE(utils::AstUtils::equals(traitD.get(), traitF.get()));
	}

	TEST_F(AstUtilsTest, TestFunctionGetChildrenValidation)
	{
		parser->loadSource(
			"fn main(a: i32, { Test: ({ Test: (Test1, { Test: Test2 } ) }, Test3), Test4 }: Foo) {}"
		);

		auto funcA = parser->parseFunction(ctx, false);
		auto identifiers = utils::AstUtils::extractFunctionIdentifiers(funcA.get());
		
		EXPECT_EQ(identifiers.size(), 5);
		EXPECT_EQ(std::get<0>(identifiers[0]), "a");
		EXPECT_EQ(std::get<0>(identifiers[1]), "Test1");
		EXPECT_EQ(std::get<0>(identifiers[2]), "Test2");
		EXPECT_EQ(std::get<0>(identifiers[3]), "Test3");
		EXPECT_EQ(std::get<0>(identifiers[4]), "Test4");
	}

	TEST_F(AstUtilsTest, TestFunctionStaticAndNonStatic)
	{
		parser->loadSource(
			"class Foo { \n"
				"fn main(a: i32) {} \n"
				"static fn main(a: i32) {} \n"
			"}"
		);

		auto classA = parser->parseClass(ctx, false);

		ASSERT_FALSE(utils::AstUtils::equals(classA->functionList[0].get(), classA->functionList[1].get()));
	}
} }