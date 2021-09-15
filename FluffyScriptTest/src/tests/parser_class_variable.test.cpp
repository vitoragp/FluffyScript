#include <memory>
#include "gtest/gtest.h"

#include "ast\fl_ast_type.h"
#include "parser\fl_parser.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"


namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * ParserClassVariableTest
	 */

	struct ParserClassVariableTest : public ::testing::Test
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

	TEST_F(ParserClassVariableTest, TestVariableI32)
	{
		parser->loadSource("class Foo { let Foo: i32; }");

		auto classDecl = parser->parseClass(ctx, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, false);
		EXPECT_EQ(classDecl->variableList[0]->isReference, false);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestVariableRefI32)
	{
		parser->loadSource("class Foo { let ref Foo: i32; }");

		auto classDecl = parser->parseClass(ctx, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, false);
		EXPECT_EQ(classDecl->variableList[0]->isReference, true);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestConstI32)
	{
		parser->loadSource("class Foo { const Foo: i32 = a.getValue(); }");

		auto classDecl = parser->parseClass(ctx, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, true);
		EXPECT_EQ(classDecl->variableList[0]->isReference, false);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestConstRefI32)
	{
		parser->loadSource("class Foo { const ref Foo: i32 = 5 * 9 + super.getValue() * this.value; }");

		auto classDecl = parser->parseClass(ctx, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, true);
		EXPECT_EQ(classDecl->variableList[0]->isReference, true);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestConstFunctionTypeWithFunctionExpr)
	{
		parser->loadSource("class Foo { const ref Foo: fn(i32 -> i32) = |a| { }; }");

		auto classDecl = parser->parseClass(ctx, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, true);
		EXPECT_EQ(classDecl->variableList[0]->isReference, true);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->nodeType, AstNodeType_e::FunctionType);

		auto functionType = classDecl->variableList[0]->typeDecl->to<ast::TypeDeclFunction>();

		EXPECT_EQ(functionType->parameterTypeList[0]->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(functionType->parameterTypeList[0]->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(functionType->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(functionType->returnType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}
} }