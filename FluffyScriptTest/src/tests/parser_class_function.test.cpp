#include <memory>
#include "gtest/gtest.h"

#include "parser\fl_parser.h"
#include "parser\parser_objects/fl_parser_objects.h"
#include "lexer\fl_lexer.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"


namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserClassFunctionTest
	 */

	struct ParserClassFunctionTest : public ::testing::Test
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

	TEST_F(ParserClassFunctionTest, TestFunctionNoModNoParamsNoRet)
	{
		ctx.parser->loadSource("class Foo { fn Foo() {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::Void);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithOnlyOneParamNoRet)
	{
		ctx.parser->loadSource("class Foo { fn Foo(test: i32) {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::Void);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsNoRet)
	{
		ctx.parser->loadSource("class Foo { fn Foo(test: i32, test2: string) {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::Void);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsWithRet)
	{
		ctx.parser->loadSource("class Foo { fn Foo(test: i32, test2: string) -> i32 {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsWithRetWithOverride)
	{
		ctx.parser->loadSource("class Foo { virtual fn Foo(test: i32, test2: string) -> i32 override {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, true);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, true);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsWithRetWithFinal)
	{
		ctx.parser->loadSource("class Foo { virtual fn Foo(test: i32, test2: string) -> i32 final {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, true);
		EXPECT_EQ(classObject->functionList[0]->isFinal, true);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestVirtualFunctionWithModWithMultiplesParamsWithRetWithFinal)
	{
		ctx.parser->loadSource("class Foo { protected virtual fn Foo(test: i32, test2: string) -> i32 final {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Protected);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, true);
		EXPECT_EQ(classObject->functionList[0]->isFinal, true);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestAbstractFunctionWithModWithMultiplesParamsWithRetWithFinal)
	{
		ctx.parser->loadSource("class Foo { protected abstract fn Foo(test: i32, test2: string) -> i32; }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Protected);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, true);
		EXPECT_EQ(classObject->functionList[0]->isVirtual, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->functionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->functionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->functionList[0]->returnType->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestStaticFunctionNoModWithMultiplesParamsWithRet)
	{
		ctx.parser->loadSource("class Foo { static fn Foo(test: i32, test2: string) -> i32 {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 0);
		EXPECT_EQ(classObject->staticFunctionList.size(), 1);

		EXPECT_EQ(classObject->staticFunctionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->staticFunctionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->staticFunctionList[0]->isStatic, true);
		EXPECT_EQ(classObject->staticFunctionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->staticFunctionList[0]->isVirtual, false);
		EXPECT_EQ(classObject->staticFunctionList[0]->isFinal, false);
		EXPECT_EQ(classObject->staticFunctionList[0]->isOverride, false);

		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->staticFunctionList[0]->returnType->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestStaticFunctionNoModWithMultiplesParamsWithRetWithGeneric)
	{
		ctx.parser->loadSource("class Foo { static fn Foo<T: where T is i32 | fp32>(test: i32, test2: string) -> i32 {} }");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->functionList.size(), 0);
		EXPECT_EQ(classObject->staticFunctionList.size(), 1);

		EXPECT_EQ(classObject->staticFunctionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->staticFunctionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->staticFunctionList[0]->isStatic, true);
		EXPECT_EQ(classObject->staticFunctionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->staticFunctionList[0]->isVirtual, false);
		EXPECT_EQ(classObject->staticFunctionList[0]->isFinal, false);
		EXPECT_EQ(classObject->staticFunctionList[0]->isOverride, false);

		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList.size(), 1);
		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList[0]->identifier, "T");
		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList[0]->whereTypeList.size(), 2);
		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList[0]->whereTypeList[0]->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList[0]->whereTypeList[0]->nullable, false);
		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList[0]->whereTypeList[1]->typeID, TypeDeclID_e::Fp32);
		EXPECT_EQ(classObject->staticFunctionList[0]->genericDeclList[0]->whereTypeList[1]->nullable, false);

		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList.size(), 2);

		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[0]->identifier, "test");
		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[0]->typeDecl->typeID, TypeDeclID_e::I32);

		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[1]->identifier, "test2");
		EXPECT_EQ(classObject->staticFunctionList[0]->parameterList[1]->typeDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(classObject->staticFunctionList[0]->returnType->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestInvalidStaticFunctionWithVirtual)
	{
		ctx.parser->loadSource("class Foo { static virtual fn Foo(test: i32, test2: string) -> i32 {} }");
		ctx.parser->nextToken();

		try
		{
			auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);
			throw std::exception();
		}
		catch (exceptions::custom_exception& e)
		{
			e.what();
		}
	}

	TEST_F(ParserClassFunctionTest, TestInvalidStaticFunctionWithAbstact)
	{
		ctx.parser->loadSource("class Foo { static abstract fn Foo(test: i32, test2: string) -> i32 {} }");
		ctx.parser->nextToken();

		try
		{
			auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);
			throw std::exception();
		}
		catch (exceptions::custom_exception& e)
		{
			e.what();
		}
	}

	TEST_F(ParserClassFunctionTest, TestInvalidAbstractFunction)
	{
		ctx.parser->loadSource("class Foo { protected abstract fn Foo(test: i32, test2: string) -> i32 {} }");
		ctx.parser->nextToken();

		try
		{
			auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);
			throw std::exception();
		}
		catch (exceptions::unexpected_token_exception& e)
		{
			e.what();
		}
		catch (exceptions::custom_exception& e)
		{
			e.what();
		}
	}

	TEST_F(ParserClassFunctionTest, TestStaticFunctionAndFunctionPass)
	{
		ctx.parser->loadSourceFromFile(".\\files\\parser\\source_2.txt");
		ctx.parser->nextToken();

		parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);
	}
} }