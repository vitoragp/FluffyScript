#include <memory>
#include "gtest/gtest.h"

#include "parser/fl_parser.h"
#include "parser/parser_objects/fl_parser_objects.h"
#include "lexer\fl_lexer.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"


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

		EXPECT_EQ(classObject->identifier, "Foo");
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithOnlyOneGenericNoExtendsNoImplements)
	{
		parser->loadSource("class Foo<T> {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDeclList.size(), 1);
		EXPECT_EQ(classObject->genericDeclList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDeclList[0]->whereTypeList.size(), 0);

		EXPECT_EQ(classObject->baseClass, nullptr);
		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsWithExtendsNoImplements)
	{
		parser->loadSource("class Foo<T, R> extends ::Window {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDeclList.size(), 2);
		EXPECT_EQ(classObject->genericDeclList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDeclList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericDeclList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericDeclList[1]->whereTypeList.size(), 0);
		
		EXPECT_EQ(classObject->baseClass->typeID, TypeDeclID_e::Named);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->baseClass.get());

		EXPECT_EQ(namedTypeDecl->identifier, "Window");
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericsWithExtendsNoImplements)
	{
		parser->loadSource("class Foo extends ::Window {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->baseClass->typeID, TypeDeclID_e::Named);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->baseClass.get());

		EXPECT_EQ(namedTypeDecl->identifier, "Window");
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsNoExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo<T, R> implements ::Clickable<T>, UI::Viewable<R> {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDeclList.size(), 2);
		EXPECT_EQ(classObject->genericDeclList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDeclList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericDeclList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericDeclList[1]->whereTypeList.size(), 0);

		EXPECT_EQ(classObject->baseClass, nullptr);

		EXPECT_EQ(classObject->interfaceList.size(), 2);

		// Clickable
		{
			EXPECT_EQ(classObject->interfaceList[0]->typeID, TypeDeclID_e::Named);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[0].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Clickable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, true);
			EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 1);

			EXPECT_EQ(namedTypeDecl->genericDefinitionList[0]->typeID, TypeDeclID_e::Named);

			auto genericTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(namedTypeDecl->genericDefinitionList[0].get());

			EXPECT_EQ(genericTypeDecl->identifier, "T");
			EXPECT_EQ(genericTypeDecl->startFromRoot, false);
			EXPECT_EQ(genericTypeDecl->genericDefinitionList.size(), 0);
		}

		// UI
		{
			EXPECT_EQ(classObject->interfaceList[1]->typeID, TypeDeclID_e::Named);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[1].get());

			EXPECT_EQ(namedTypeDecl->identifier, "UI");
			EXPECT_EQ(namedTypeDecl->startFromRoot, false);

			EXPECT_EQ(namedTypeDecl->internalIdentifier->identifier, "Viewable");
			EXPECT_EQ(namedTypeDecl->internalIdentifier->startFromRoot, false);
			EXPECT_EQ(namedTypeDecl->internalIdentifier->genericDefinitionList.size(), 1);

			EXPECT_EQ(namedTypeDecl->internalIdentifier->genericDefinitionList[0]->typeID, TypeDeclID_e::Named);

			auto genericTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(namedTypeDecl->internalIdentifier->genericDefinitionList[0].get());

			EXPECT_EQ(genericTypeDecl->identifier, "R");
			EXPECT_EQ(genericTypeDecl->startFromRoot, false);
			EXPECT_EQ(genericTypeDecl->genericDefinitionList.size(), 0);
		}
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericsNoExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo implements ::Clickable, UI::Viewable {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDeclList.size(), 0);

		EXPECT_EQ(classObject->baseClass, nullptr);

		EXPECT_EQ(classObject->interfaceList.size(), 2);
				
		EXPECT_EQ(classObject->interfaceList.size(), 2);

		// Clickable
		{
			EXPECT_EQ(classObject->interfaceList[0]->typeID, TypeDeclID_e::Named);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[0].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Clickable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, true);
			EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		}

		// UI
		{
			EXPECT_EQ(classObject->interfaceList[1]->typeID, TypeDeclID_e::Named);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[1].get());

			EXPECT_EQ(namedTypeDecl->identifier, "UI");
			EXPECT_EQ(namedTypeDecl->startFromRoot, false);

			EXPECT_EQ(namedTypeDecl->internalIdentifier->identifier, "Viewable");
			EXPECT_EQ(namedTypeDecl->internalIdentifier->startFromRoot, false);
			EXPECT_EQ(namedTypeDecl->internalIdentifier->genericDefinitionList.size(), 0);
		}
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsWithExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo<T, R> extends ::Window implements ::Clickable, UI::Viewable {}");
		parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDeclList.size(), 2);
		EXPECT_EQ(classObject->genericDeclList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDeclList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericDeclList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericDeclList[1]->whereTypeList.size(), 0);
				
		EXPECT_EQ(classObject->baseClass->typeID, TypeDeclID_e::Named);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->baseClass.get());

		EXPECT_EQ(namedTypeDecl->identifier, "Window");
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 2);

		// Clickable
		{
			EXPECT_EQ(classObject->interfaceList[0]->typeID, TypeDeclID_e::Named);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[0].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Clickable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, true);
			EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		}

		// UI
		{
			EXPECT_EQ(classObject->interfaceList[1]->typeID, TypeDeclID_e::Named);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[1].get());

			EXPECT_EQ(namedTypeDecl->identifier, "UI");
			EXPECT_EQ(namedTypeDecl->startFromRoot, false);

			EXPECT_EQ(namedTypeDecl->internalIdentifier->identifier, "Viewable");
			EXPECT_EQ(namedTypeDecl->internalIdentifier->startFromRoot, false);
			EXPECT_EQ(namedTypeDecl->internalIdentifier->genericDefinitionList.size(), 0);
		}
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithOnlyOneDeclaration)
	{
		parser->loadSource("<T>");
		parser->nextToken();

		auto genericDeclList = parser_objects::ParserObjectGenericDecl::parse(parser.get());

		EXPECT_EQ(genericDeclList.size(), 1);
		EXPECT_EQ(genericDeclList[0]->identifier, "T");
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithTwoDeclaration)
	{
		parser->loadSource("<T, R>");
		parser->nextToken();

		auto genericDeclList = parser_objects::ParserObjectGenericDecl::parse(parser.get());

		EXPECT_EQ(genericDeclList.size(), 2);

		EXPECT_EQ(genericDeclList[0]->identifier, "T");
		EXPECT_EQ(genericDeclList[1]->identifier, "R");
	}

	TEST_F(ParserClassTest, TestFullClassDeclaration)
	{
		parser->loadSourceFromFile(".\\files\\parser\\source_4.txt");
		parser->nextToken();

		while (!parser->isEof())
		{
			parser_objects::ParserObjectClassDecl::parse(parser.get(), false, false);
		}
	}

	TEST_F(ParserClassTest, TestFullClassDeclaration2)
	{
		parser->loadSourceFromFile(".\\files\\parser\\source_5.txt");

		auto program = parser->parse();

		ASSERT_TRUE(program != nullptr);
	}
} }