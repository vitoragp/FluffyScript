#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "ast\fl_ast_type.h"
#include "parser\fl_parser.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"


namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * ParserClassFunctionTest
	 */

	struct ParserClassFunctionTest : public ::testing::Test
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

	TEST_F(ParserClassFunctionTest, TestFunctionNoModNoParamsNoRet)
	{
		parser->loadSource("class Foo { fn Foo() {} }");
		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 0);

		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::Void);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithOnlyOneParamNoRet)
	{
		parser->loadSource("class Foo { fn Foo(test: i32) {} }");
		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 1);

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[0].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		}

		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::Void);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsNoRet)
	{
		parser->loadSource("class Foo { fn Foo(test: i32, test2: string) {} }");
		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[0].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		}

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[1].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test2");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::String);
		}

		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::Void);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsWithRet)
	{
		parser->loadSource("class Foo { fn Foo(test: i32, test2: string) -> i32 {} }");
		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[0].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		}

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[1].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test2");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::String);
		}

		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsWithRetWithOverride)
	{
		parser->loadSource("class Foo { override fn Foo(test: i32, test2: string) -> i32 {} }");
		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, true);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[0].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		}

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[1].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test2");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::String);
		}

		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestFunctionNoModWithMultiplesParamsWithRetWithFinal)
	{
		parser->loadSource("class Foo { override fn Foo(test: i32, test2: string) -> i32 final {} }");
		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Private);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, true);
		EXPECT_EQ(classObject->functionList[0]->isOverride, true);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[0].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		}

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[1].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test2");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::String);
		}

		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestVirtualFunctionWithModWithMultiplesParamsWithRetWithFinal)
	{
		parser->loadSource("class Foo { protected override fn Foo(test: i32, test2: string) -> i32 final {} }");
		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);		

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Protected);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, false);
		EXPECT_EQ(classObject->functionList[0]->isFinal, true);
		EXPECT_EQ(classObject->functionList[0]->isOverride, true);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[0].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		}

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[1].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test2");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::String);
		}

		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestAbstractFunctionWithModWithMultiplesParamsWithRetWithFinal)
	{
		parser->loadSource("class Foo { protected abstract fn Foo(test: i32, test2: string) -> i32; }");		

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->functionList.size(), 1);

		EXPECT_EQ(classObject->functionList[0]->accessModifier, ClassMemberAccessModifier_e::Protected);

		EXPECT_EQ(classObject->functionList[0]->identifier, "Foo");

		EXPECT_EQ(classObject->functionList[0]->isStatic, false);
		EXPECT_EQ(classObject->functionList[0]->isAbstract, true);
		EXPECT_EQ(classObject->functionList[0]->isFinal, false);
		EXPECT_EQ(classObject->functionList[0]->isOverride, false);

		EXPECT_EQ(classObject->functionList[0]->genericDecl, nullptr);

		EXPECT_EQ(classObject->functionList[0]->parameterList.size(), 2);
		
		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[0].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		}

		{
			auto param = dynamic_cast<ast::FunctionParameterDecl*>(classObject->functionList[0]->parameterList[1].get());

			ASSERT_TRUE(param != nullptr);

			EXPECT_EQ(param->identifier, "test2");
			EXPECT_EQ(param->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
			EXPECT_EQ(param->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::String);
		}
		
		EXPECT_EQ(classObject->functionList[0]->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classObject->functionList[0]->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassFunctionTest, TestInvalidStaticFunctionWithVirtual)
	{
		parser->loadSource("class Foo { static override fn Foo(test: i32, test2: string) -> i32 {} }");

		try
		{
			auto classObject = parser->parseClass(ctx, false);
			throw std::exception();
		}
		catch (exceptions::custom_exception& e)
		{
			e.what();
		}
	}

	TEST_F(ParserClassFunctionTest, TestInvalidStaticFunctionWithAbstact)
	{
		parser->loadSource("class Foo { static abstract fn Foo(test: i32, test2: string) -> i32 {} }");

		try
		{
			auto classObject = parser->parseClass(ctx, false);
			throw std::exception();
		}
		catch (exceptions::custom_exception& e)
		{
			e.what();
		}
	}

	TEST_F(ParserClassFunctionTest, TestInvalidAbstractFunction)
	{
		parser->loadSource("class Foo { protected abstract fn Foo(test: i32, test2: string) -> i32 {} }");

		try
		{
			auto classObject = parser->parseClass(ctx, false);
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
		String file = getProjectFilePath(".\\files\\parser\\source_2.txt");
		parser->loadSourceFromFile(file.c_str());
		parser->parseClass(ctx, false);
	}
} }