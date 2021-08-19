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

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericNoExtendsNoImplements)
	{
		ctx.parser->loadSource("class Foo {}");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->identifier, "Foo");
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithOnlyOneGenericNoExtendsNoImplements)
	{
		ctx.parser->loadSource("class Foo<T> {}");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDeclList.size(), 1);
		EXPECT_EQ(classObject->genericDeclList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDeclList[0]->whereTypeList.size(), 0);

		EXPECT_EQ(classObject->baseClass, nullptr);
		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsWithExtendsNoImplements)
	{
		ctx.parser->loadSource("class Foo<T, R> extends ::Window {}");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

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
		ctx.parser->loadSource("class Foo extends ::Window {}");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->baseClass->typeID, TypeDeclID_e::Named);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->baseClass.get());

		EXPECT_EQ(namedTypeDecl->identifier, "Window");
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsNoExtendsTwoInterfaces)
	{
		ctx.parser->loadSource("class Foo<T, R> implements ::Clickable<T>, UI::Viewable<R> {}");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

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
		ctx.parser->loadSource("class Foo implements ::Clickable, UI::Viewable {}");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

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
		ctx.parser->loadSource("class Foo<T, R> extends ::Window implements ::Clickable, UI::Viewable {}");
		ctx.parser->nextToken();

		auto classObject = parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);

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
		ctx.parser->loadSource("<T>");
		ctx.parser->nextToken();

		auto genericDeclList = parser_objects::ParserObjectGenericDecl::parse(&ctx);

		EXPECT_EQ(genericDeclList.size(), 1);
		EXPECT_EQ(genericDeclList[0]->identifier, "T");
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithTwoDeclaration)
	{
		ctx.parser->loadSource("<T, R>");
		ctx.parser->nextToken();

		auto genericDeclList = parser_objects::ParserObjectGenericDecl::parse(&ctx);

		EXPECT_EQ(genericDeclList.size(), 2);

		EXPECT_EQ(genericDeclList[0]->identifier, "T");
		EXPECT_EQ(genericDeclList[1]->identifier, "R");
	}

	TEST_F(ParserClassTest, TestFullClassDeclaration)
	{
		ctx.parser->loadSourceFromFile(".\\files\\parser\\source_4.txt");
		ctx.parser->nextToken();

		while (!ctx.parser->isEof())
		{
			parser_objects::ParserObjectClassDecl::parse(&ctx, false, false);
		}
	}
} }