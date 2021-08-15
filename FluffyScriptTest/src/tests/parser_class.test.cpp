#include <memory>
#include "gtest/gtest.h"

#include "fl_parser.h"
#include "fl_exceptions.h"
#include "parser_objects/fl_parser_objects.h"


namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserClassTest
	 */

	struct ParserClassTest : public ::testing::Test
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

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericNoExtendsNoImplements)
	{
		parser->loadSource("class Foo {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithOnlyOneGenericNoExtendsNoImplements)
	{
		parser->loadSource("class Foo<T> {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 1);
		EXPECT_EQ(classObject->genericTemplateList[0], "T");

		EXPECT_EQ(classObject->baseClass, nullptr);
		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsWithExtendsNoImplements)
	{
		parser->loadSource("class Foo<T, R> extends ::Window {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 2);
		EXPECT_EQ(classObject->genericTemplateList[0], "T");
		EXPECT_EQ(classObject->genericTemplateList[1], "R");
				
		EXPECT_EQ(classObject->baseClass->identifier, "Window");
		EXPECT_EQ(classObject->baseClass->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericsWithExtendsNoImplements)
	{
		parser->loadSource("class Foo extends ::Window {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 0);

		EXPECT_EQ(classObject->baseClass->identifier, "Window");
		EXPECT_EQ(classObject->baseClass->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsNoExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo<T, R> implements ::Clickable<T>, UI::Viewable<R> {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 2);
		EXPECT_EQ(classObject->genericTemplateList[0], "T");
		EXPECT_EQ(classObject->genericTemplateList[1], "R");

		EXPECT_EQ(classObject->baseClass, nullptr);

		EXPECT_EQ(classObject->interfaceList.size(), 2);

		EXPECT_EQ(classObject->interfaceList[0]->identifier, "Clickable");
		EXPECT_EQ(classObject->interfaceList[0]->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList[1]->identifier, "UI");
		EXPECT_EQ(classObject->interfaceList[1]->startFromRoot, false);

		EXPECT_EQ(classObject->interfaceList[1]->tailIdentifier->identifier, "Viewable");
		EXPECT_EQ(classObject->interfaceList[1]->tailIdentifier->startFromRoot, false);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericsNoExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo implements ::Clickable, UI::Viewable {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 0);

		EXPECT_EQ(classObject->baseClass, nullptr);

		EXPECT_EQ(classObject->interfaceList.size(), 2);
				
		EXPECT_EQ(classObject->interfaceList[0]->identifier, "Clickable");
		EXPECT_EQ(classObject->interfaceList[0]->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList[1]->identifier, "UI");
		EXPECT_EQ(classObject->interfaceList[1]->startFromRoot, false);

		EXPECT_EQ(classObject->interfaceList[1]->tailIdentifier->identifier, "Viewable");
		EXPECT_EQ(classObject->interfaceList[1]->tailIdentifier->startFromRoot, false);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsWithExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo<T, R> extends ::Window implements ::Clickable, UI::Viewable {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 2);
		EXPECT_EQ(classObject->genericTemplateList[0], "T");
		EXPECT_EQ(classObject->genericTemplateList[1], "R");
				
		EXPECT_EQ(classObject->baseClass->identifier, "Window");
		EXPECT_EQ(classObject->baseClass->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 2);

		EXPECT_EQ(classObject->interfaceList[0]->identifier, "Clickable");
		EXPECT_EQ(classObject->interfaceList[0]->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList[1]->identifier, "UI");
		EXPECT_EQ(classObject->interfaceList[1]->startFromRoot, false);

		EXPECT_EQ(classObject->interfaceList[1]->tailIdentifier->identifier, "Viewable");
		EXPECT_EQ(classObject->interfaceList[1]->tailIdentifier->startFromRoot, false);
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithOnlyOneDeclaration)
	{
		parser->loadSource("<T>");
		parser->nextToken();

		auto genericTemplateList = parser_objects::ParserObjectGenericTemplateDecl::parse(parser.get());

		EXPECT_EQ(genericTemplateList.size(), 1);
		EXPECT_EQ(genericTemplateList[0], "T");
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithTwoDeclaration)
	{
		parser->loadSource("<T, R>");
		parser->nextToken();

		auto genericTemplateList = parser_objects::ParserObjectGenericTemplateDecl::parse(parser.get());

		EXPECT_EQ(genericTemplateList.size(), 2);

		EXPECT_EQ(genericTemplateList[0], "T");
		EXPECT_EQ(genericTemplateList[1], "R");
	}

	TEST_F(ParserClassTest, TestClassExtendsDeclaration)
	{
		parser->loadSource("extends ::Window");
		parser->nextToken();

		auto extendsClass = parser_objects::ParserObjectClassExtendsDecl::parse(parser.get());

		EXPECT_EQ(extendsClass->identifier, "Window");
		EXPECT_EQ(extendsClass->startFromRoot, true);
	}

	TEST_F(ParserClassTest, TestClassImplementsWithOnlyOneDeclaration)
	{
		parser->loadSource("implements ::Clickable");
		parser->nextToken();

		auto interfaceList = parser_objects::ParserObjectClassImplementsDecl::parse(parser.get());

		EXPECT_EQ(interfaceList.size(), 1);
		EXPECT_EQ(interfaceList[0]->identifier, "Clickable");
		EXPECT_EQ(interfaceList[0]->startFromRoot, true);
	}

	TEST_F(ParserClassTest, TestClassImplementsWithTwoDeclarations)
	{
		parser->loadSource("implements ::Clickable, UI::Viewable");
		parser->nextToken();

		auto interfaceList = parser_objects::ParserObjectClassImplementsDecl::parse(parser.get());

		EXPECT_EQ(interfaceList.size(), 2);
				
		EXPECT_EQ(interfaceList[0]->identifier, "Clickable");
		EXPECT_EQ(interfaceList[0]->startFromRoot, true);

		EXPECT_EQ(interfaceList[1]->identifier, "UI");
		EXPECT_EQ(interfaceList[1]->startFromRoot, false);

		EXPECT_EQ(interfaceList[1]->tailIdentifier->identifier, "Viewable");
		EXPECT_EQ(interfaceList[1]->tailIdentifier->startFromRoot, false);
	}
} }