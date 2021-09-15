#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "parser/fl_parser.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"


namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * ParserClassTest
	 */

	struct ParserClassTest : public ::testing::Test
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

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericNoExtendsNoImplements)
	{
		parser->loadSource("class Foo {}");

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->identifier, "Foo");
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithOnlyOneGenericNoExtendsNoImplements)
	{
		parser->loadSource("class Foo<T> {}");

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDecl->genericDeclItemList.size(), 1);
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->whereTypeList.size(), 0);

		EXPECT_EQ(classObject->baseClass, nullptr);
		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsWithExtendsNoImplements)
	{
		parser->loadSource("class Foo<T, R> extends ::Window {}");

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDecl->genericDeclItemList.size(), 2);
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[1]->whereTypeList.size(), 0);
		
		EXPECT_EQ(classObject->baseClass->nodeType, AstNodeType_e::NamedType);

		auto namedTypeDecl = classObject->baseClass->to<ast::TypeDeclNamed>();

		EXPECT_EQ(namedTypeDecl->identifier, "Window");
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericsWithExtendsNoImplements)
	{
		parser->loadSource("class Foo extends ::Window {}");

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->baseClass->nodeType, AstNodeType_e::NamedType);

		auto namedTypeDecl = classObject->baseClass->to<ast::TypeDeclNamed>();

		EXPECT_EQ(namedTypeDecl->identifier, "Window");
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 0);
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsNoExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo<T, R> implements ::Clickable<T>, UI::Viewable<R> {}");

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDecl->genericDeclItemList.size(), 2);
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[1]->whereTypeList.size(), 0);

		EXPECT_EQ(classObject->baseClass, nullptr);

		EXPECT_EQ(classObject->interfaceList.size(), 2);

		// Clickable
		{
			EXPECT_EQ(classObject->interfaceList[0]->nodeType, AstNodeType_e::NamedType);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[0].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Clickable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, true);
			EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 1);

			EXPECT_EQ(namedTypeDecl->genericDefinitionList[0]->nodeType, AstNodeType_e::NamedType);

			auto genericTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(namedTypeDecl->genericDefinitionList[0].get());

			EXPECT_EQ(genericTypeDecl->identifier, "T");
			EXPECT_EQ(genericTypeDecl->startFromRoot, false);
			EXPECT_EQ(genericTypeDecl->genericDefinitionList.size(), 0);
		}

		// UI
		{
			EXPECT_EQ(classObject->interfaceList[1]->nodeType, AstNodeType_e::NamedType);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[1].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Viewable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, false);
			EXPECT_EQ(namedTypeDecl->genericDefinitionList[0]->nodeType, AstNodeType_e::NamedType);
			EXPECT_EQ(namedTypeDecl->startFromRoot, false);

			EXPECT_EQ(namedTypeDecl->scopePath->identifier, "UI");

			auto genericTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(namedTypeDecl->genericDefinitionList[0].get());

			EXPECT_EQ(genericTypeDecl->identifier, "R");
			EXPECT_EQ(genericTypeDecl->startFromRoot, false);
			EXPECT_EQ(genericTypeDecl->genericDefinitionList.size(), 0);
		}
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportNoGenericsNoExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo implements ::Clickable, UI::Viewable {}");

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDecl, nullptr);

		EXPECT_EQ(classObject->baseClass, nullptr);

		EXPECT_EQ(classObject->interfaceList.size(), 2);
				
		EXPECT_EQ(classObject->interfaceList.size(), 2);

		// Clickable
		{
			EXPECT_EQ(classObject->interfaceList[0]->nodeType, AstNodeType_e::NamedType);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[0].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Clickable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, true);
			EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		}

		// UI
		{
			EXPECT_EQ(classObject->interfaceList[1]->nodeType, AstNodeType_e::NamedType);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[1].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Viewable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, false);

			EXPECT_EQ(namedTypeDecl->scopePath->identifier, "UI");
		}
	}

	TEST_F(ParserClassTest, TestParseClassEmptyNoExportWithTwoGenericsWithExtendsTwoInterfaces)
	{
		parser->loadSource("class Foo<T, R> extends ::Window implements ::Clickable, UI::Viewable {}");

		auto classObject = parser->parseClass(ctx, false);

		EXPECT_EQ(classObject->identifier, "Foo");

		EXPECT_EQ(classObject->genericDecl->genericDeclItemList.size(), 2);
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->identifier, "T");
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[0]->whereTypeList.size(), 0);
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[1]->identifier, "R");
		EXPECT_EQ(classObject->genericDecl->genericDeclItemList[1]->whereTypeList.size(), 0);
				
		EXPECT_EQ(classObject->baseClass->nodeType, AstNodeType_e::NamedType);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->baseClass.get());

		EXPECT_EQ(namedTypeDecl->identifier, "Window");
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);

		EXPECT_EQ(classObject->interfaceList.size(), 2);

		// Clickable
		{
			EXPECT_EQ(classObject->interfaceList[0]->nodeType, AstNodeType_e::NamedType);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[0].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Clickable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, true);
			EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		}

		// UI
		{
			EXPECT_EQ(classObject->interfaceList[1]->nodeType, AstNodeType_e::NamedType);

			auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(classObject->interfaceList[1].get());

			EXPECT_EQ(namedTypeDecl->identifier, "Viewable");
			EXPECT_EQ(namedTypeDecl->startFromRoot, false);

			EXPECT_EQ(namedTypeDecl->scopePath->identifier, "UI");
		}
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithOnlyOneDeclaration)
	{
		parser->loadSource("<T>");

		auto genericDecl = parser->parseGenericDecl(ctx);

		EXPECT_EQ(genericDecl->genericDeclItemList.size(), 1);
		EXPECT_EQ(genericDecl->genericDeclItemList[0]->identifier, "T");
	}

	TEST_F(ParserClassTest, TestGenericTemplateWithTwoDeclaration)
	{
		parser->loadSource("<T, R>");

		auto genericDecl = parser->parseGenericDecl(ctx);

		EXPECT_EQ(genericDecl->genericDeclItemList.size(), 2);

		EXPECT_EQ(genericDecl->genericDeclItemList[0]->identifier, "T");
		EXPECT_EQ(genericDecl->genericDeclItemList[1]->identifier, "R");
	}

	TEST_F(ParserClassTest, TestFullClassDeclaration)
	{
		String file = getProjectFilePath("files\\parser\\source_4.txt");
		parser->loadSourceFromFile(file.c_str());
		parser->parseClass(ctx, false);
	}

	TEST_F(ParserClassTest, TestFullClassDeclaration2)
	{
		String file = getProjectFilePath("files\\parser\\source_5.txt");
		parser->loadSourceFromFile(file.c_str());

		auto codeUnit = parser->parseCodeUnit(ctx);

		ASSERT_TRUE(codeUnit != nullptr);
	}

	TEST_F(ParserClassTest, TestStatement)
	{
		parser->loadSource("a<i32>()");

		auto expr = parser->parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		ASSERT_TRUE(expr != nullptr);
	}
} }