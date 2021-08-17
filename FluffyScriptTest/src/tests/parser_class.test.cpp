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
		EXPECT_EQ(classObject->genericTemplateList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericTemplateList[0]->whereTypeList.size(), 0);

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
		EXPECT_EQ(classObject->genericTemplateList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericTemplateList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericTemplateList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericTemplateList[1]->whereTypeList.size(), 0);
		
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

		EXPECT_EQ(classObject->name, "Foo");

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

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 2);
		EXPECT_EQ(classObject->genericTemplateList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericTemplateList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericTemplateList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericTemplateList[1]->whereTypeList.size(), 0);

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

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 0);

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

		EXPECT_EQ(classObject->name, "Foo");

		EXPECT_EQ(classObject->genericTemplateList.size(), 2);
		EXPECT_EQ(classObject->genericTemplateList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericTemplateList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericTemplateList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericTemplateList[1]->whereTypeList.size(), 0);
				
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

		auto genericTemplateList = parser_objects::ParserObjectGenericTemplateDecl::parse(parser.get());

		EXPECT_EQ(genericTemplateList.size(), 1);
		EXPECT_EQ(genericTemplateList[0]->identifier, "T");
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithTwoDeclaration)
	{
		parser->loadSource("<T, R>");
		parser->nextToken();

		auto genericTemplateList = parser_objects::ParserObjectGenericTemplateDecl::parse(parser.get());

		EXPECT_EQ(genericTemplateList.size(), 2);

		EXPECT_EQ(genericTemplateList[0]->identifier, "T");
		EXPECT_EQ(genericTemplateList[1]->identifier, "R");
	}
} }