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
	 * ParserClassVariableTest
	 */

	struct ParserClassVariableTest : public ::testing::Test
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

	TEST_F(ParserClassVariableTest, TestVariableI32)
	{
		ctx.parser->loadSource("class Foo { let Foo: i32; }");
		ctx.parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, false);
		EXPECT_EQ(classDecl->variableList[0]->isReference, false);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestVariableRefI32)
	{
		ctx.parser->loadSource("class Foo { let ref Foo: i32; }");
		ctx.parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, false);
		EXPECT_EQ(classDecl->variableList[0]->isReference, true);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestConstI32)
	{
		ctx.parser->loadSource("class Foo { const Foo: i32 = a.getValue(); }");
		ctx.parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, true);
		EXPECT_EQ(classDecl->variableList[0]->isReference, false);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestConstRefI32)
	{
		ctx.parser->loadSource("class Foo { const ref Foo: i32 = 5 * 9 + super.getValue() * this.value; }");
		ctx.parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, true);
		EXPECT_EQ(classDecl->variableList[0]->isReference, true);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserClassVariableTest, TestConstFunctionTypeWithFunctionExpr)
	{
		ctx.parser->loadSource("class Foo { const ref Foo: fn(i32 -> i32) = fn (a) { }; }");
		ctx.parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, true);
		EXPECT_EQ(classDecl->variableList[0]->isReference, true);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->typeID, TypeDeclID_e::Function);
	}
} }