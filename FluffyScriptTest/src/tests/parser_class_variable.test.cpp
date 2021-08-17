#include <memory>
#include "gtest/gtest.h"

#include "fl_parser.h"
#include "fl_exceptions.h"
#include "parser_objects/fl_parser_objects.h"


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

		// Sets up the test fixture.
		virtual void SetUp()
		{
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

	TEST_F(ParserClassVariableTest, TestVariableI32)
	{
		parser->loadSource("class Foo { let Foo: i32; }");
		parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

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
		parser->loadSource("class Foo { let ref Foo: i32; }");
		parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

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
		parser->loadSource("class Foo { const Foo: i32 = a.getValue(); }");
		parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

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
		parser->loadSource("class Foo { const ref Foo: i32 = 5 * 9 + super.getValue() * this.value; }");
		parser->nextToken();

		auto classDecl = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classDecl->variableList.size(), 1);
		EXPECT_EQ(classDecl->variableList[0]->type, ClassMemberType_e::Variable);
		EXPECT_EQ(classDecl->variableList[0]->identifier, "Foo");
		EXPECT_EQ(classDecl->variableList[0]->isConst, true);
		EXPECT_EQ(classDecl->variableList[0]->isReference, true);
		EXPECT_EQ(classDecl->variableList[0]->isStatic, false);
		EXPECT_EQ(classDecl->variableList[0]->accessModifier, ClassMemberAccessModifier_e::Private);
		EXPECT_EQ(classDecl->variableList[0]->typeDecl->typeID, TypeDeclID_e::I32);
	}
} }