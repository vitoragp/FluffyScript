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
	 * ParserTypesTest
	 */

	struct ParserTypesTest : public ::testing::Test
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

	TEST_F(ParserTypesTest, TestNullableVoidType)
	{
		parser->loadSource("void?");

		try
		{
			auto typeVoidDecl = parser->parseType(ctx);			
			throw std::exception();
		}
		catch (exceptions::custom_exception& e)
		{
			EXPECT_STREQ(e.what(), "'void' type can't be nullable at: line 1, column 1");
		}
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveType)
	{
		parser->loadSource("i32?");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(typeI32NullableDecl->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
	}

	TEST_F(ParserTypesTest, TestPrimitiveUnsizedArrayType)
	{
		parser->loadSource("i32[]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::ArrayType);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(arrayTypeDecl->valueType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveUnsizedArrayType)
	{
		parser->loadSource("i32?[]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::ArrayType);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, true);
		EXPECT_EQ(arrayTypeDecl->valueType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(arrayTypeDecl->valueType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveUnsizedNullableArrayType)
	{
		parser->loadSource("i32[]?");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::ArrayType);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(arrayTypeDecl->valueType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveUnsizedNullableArrayType)
	{
		parser->loadSource("i32?[]?");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::ArrayType);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, true);
		EXPECT_EQ(arrayTypeDecl->valueType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(arrayTypeDecl->valueType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveMultiplesUnsizedLevelsArrayType)
	{
		parser->loadSource("i32[][]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::ArrayType);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(arrayTypeDecl->valueType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 2);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[1]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveSizedArrayType)
	{
		parser->loadSource("i32[2]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::ArrayType);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(arrayTypeDecl->valueType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Sized);
		
		ast::SizedArrayDecl* sizedArrayDecl = reinterpret_cast<ast::SizedArrayDecl*>(arrayTypeDecl->arrayDeclList[0].get());

		ASSERT_TRUE(sizedArrayDecl != nullptr);

		EXPECT_EQ(sizedArrayDecl->size, 2);
	}

	TEST_F(ParserTypesTest, TestPrimitiveMultiplesMixedLevelsArrayType)
	{
		parser->loadSource("i32[][2]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->nodeType, AstNodeType_e::ArrayType);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(arrayTypeDecl->valueType->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 2);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[1]->arrayType, ArrayType_e::Sized);
		
		ast::SizedArrayDecl* sizedArrayDecl = reinterpret_cast<ast::SizedArrayDecl*>(arrayTypeDecl->arrayDeclList[1].get());

		ASSERT_TRUE(sizedArrayDecl != nullptr);

		EXPECT_EQ(sizedArrayDecl->size, 2);
	}

	TEST_F(ParserTypesTest, TestFunctionType)
	{
		parser->loadSource("fn(i32, string, vector<i8>? -> fp32)");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->nodeType, AstNodeType_e::FunctionType);
		EXPECT_EQ(typeDecl->nullable, false);

		auto functionTypeDecl = reinterpret_cast<ast::TypeDeclFunction*>(typeDecl.get());

		ASSERT_TRUE(functionTypeDecl != nullptr);

		EXPECT_EQ(functionTypeDecl->returnType->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(functionTypeDecl->returnType->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList.size(), 3);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[0]->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[0]->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[0]->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList[1]->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[1]->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::String);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[1]->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList[2]->nodeType, AstNodeType_e::NamedType);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[2]->nullable, true);

		auto vectorTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(functionTypeDecl->parameterTypeList[2].get());

		ASSERT_TRUE(vectorTypeDecl != nullptr);
	}

	TEST_F(ParserTypesTest, TestNamedType)
	{
		parser->loadSource("Foo");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->nodeType, AstNodeType_e::NamedType);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopePath, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNamedFromRootType)
	{
		parser->loadSource("::Foo");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->nodeType, AstNodeType_e::NamedType);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopePath, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);
	}

	TEST_F(ParserTypesTest, TestArrayNamedType)
	{
		parser->loadSource("Foo[]");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->nodeType, AstNodeType_e::ArrayType);
		EXPECT_EQ(typeDecl->nullable, false);

		auto arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(arrayTypeDecl->valueType.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopePath, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNullableArrayNamedType)
	{
		parser->loadSource("Foo[]?");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->nodeType, AstNodeType_e::ArrayType);
		EXPECT_EQ(typeDecl->nullable, true);

		auto arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(arrayTypeDecl->valueType.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopePath, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNamedTypeWithGeneric)
	{
		parser->loadSource("Foo<T, i32?>");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->nodeType, AstNodeType_e::NamedType);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopePath, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 2);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);

		EXPECT_EQ(namedTypeDecl->genericDefinitionList[0]->nodeType, AstNodeType_e::NamedType);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList[0]->nullable, false);

		auto namedParamType = reinterpret_cast<ast::TypeDeclNamed*>(namedTypeDecl->genericDefinitionList[0].get());

		ASSERT_TRUE(namedParamType != nullptr);

		EXPECT_EQ(namedParamType->identifier, "T");
		EXPECT_EQ(namedParamType->scopePath, nullptr);
		EXPECT_EQ(namedParamType->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedParamType->nullable, false);
		EXPECT_EQ(namedParamType->startFromRoot, false);

		EXPECT_EQ(namedTypeDecl->genericDefinitionList[1]->nodeType, AstNodeType_e::PrimitiveType);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList[1]->to<ast::TypeDeclPrimitive>()->primitiveType, PrimitiveTypeID_e::I32);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList[1]->nullable, true);
	}

	TEST_F(ParserTypesTest, TestManyProdutionsPass)
	{
		String file = getProjectFilePath("files\\parser\\source_1.txt");
		parser->loadSourceFromFile(file.c_str());

		std::vector<std::unique_ptr<ast::TypeDecl>> typeList;
		int typeCount = 0;
		while (true)
		{
			if (parser->finished())
			{
				break;
			}
			typeList.push_back(parser->parseType(ctx));
			typeCount++;
		}
		ASSERT_EQ(typeCount, 14);
	}
} }