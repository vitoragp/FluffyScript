#include <memory>
#include <filesystem>
#include "gtest/gtest.h"

#include "fl_parser.h"
#include "fl_exceptions.h"
#include "parser_objects/fl_parser_objects.h"


namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserObjectTest
	 */

	struct ParserObjectTest : public ::testing::Test
	{
		std::unique_ptr<Lexer> lexer;
		std::unique_ptr<Parser> parser;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			if (std::filesystem::exists(".\\s_cache"))
			{
				std::filesystem::remove(".\\s_cache");
			}

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

	TEST_F(ParserObjectTest, TestParseClassEmptyNoExportNoGenericNoExtendsNoImplements)
	{
		parser->loadSource("class Foo {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->name, "Foo");
	}

	TEST_F(ParserObjectTest, TestParseClassEmptyNoExportWithOnlyOneGenericNoExtendsNoImplements)
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

	TEST_F(ParserObjectTest, TestGenericTemplateWithOnlyOneDeclaration)
	{
		parser->loadSource("<T>");
		parser->nextToken();

		auto genericTemplateList = parser_objects::ParserObjectGenericTemplateDecl::parse(parser.get());

		EXPECT_EQ(genericTemplateList.size(), 1);
		EXPECT_EQ(genericTemplateList[0], "T");
	}

	TEST_F(ParserObjectTest, TestGenericTemplateWithTwoDeclaration)
	{
		parser->loadSource("<T, R>");
		parser->nextToken();

		auto genericTemplateList = parser_objects::ParserObjectGenericTemplateDecl::parse(parser.get());

		EXPECT_EQ(genericTemplateList.size(), 2);

		EXPECT_EQ(genericTemplateList[0], "T");
		EXPECT_EQ(genericTemplateList[1], "R");
	}

	TEST_F(ParserObjectTest, TestClassExtendsDeclaration)
	{
		parser->loadSource("extends ::Window");
		parser->nextToken();

		auto extendsClass = parser_objects::ParserObjectClassExtendsDecl::parse(parser.get());

		EXPECT_EQ(extendsClass->identifiers.size(), 1);
		EXPECT_EQ(extendsClass->identifiers[0], "Window");
		EXPECT_EQ(extendsClass->startFromRoot, true);
	}

	TEST_F(ParserObjectTest, TestClassImplementsWithOnlyOneDeclaration)
	{
		parser->loadSource("implements ::Clickable");
		parser->nextToken();

		auto interfaceList = parser_objects::ParserObjectClassImplementsDecl::parse(parser.get());

		EXPECT_EQ(interfaceList.size(), 1);
		EXPECT_EQ(interfaceList[0]->identifiers.size(), 1);
		EXPECT_EQ(interfaceList[0]->identifiers[0], "Clickable");
		EXPECT_EQ(interfaceList[0]->startFromRoot, true);
	}

	TEST_F(ParserObjectTest, TestClassImplementsWithTwoDeclarations)
	{
		parser->loadSource("implements ::Clickable, UI::Viewable");
		parser->nextToken();

		auto interfaceList = parser_objects::ParserObjectClassImplementsDecl::parse(parser.get());

		EXPECT_EQ(interfaceList.size(), 2);

		EXPECT_EQ(interfaceList[0]->identifiers.size(), 1);
		EXPECT_EQ(interfaceList[0]->identifiers[0], "Clickable");
		EXPECT_EQ(interfaceList[0]->startFromRoot, true);

		EXPECT_EQ(interfaceList[1]->identifiers.size(), 2);
		EXPECT_EQ(interfaceList[1]->identifiers[0], "UI");
		EXPECT_EQ(interfaceList[1]->identifiers[1], "Viewable");
		EXPECT_EQ(interfaceList[1]->startFromRoot, false);
	}
} }