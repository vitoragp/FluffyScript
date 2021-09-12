#include <memory>
#include "test.h"
#include "gtest/gtest.h"

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

	TEST_F(ParserTypesTest, TestPrimitiveTypes)
	{
		parser->loadSource("void i8 u8 i16 u16 i32 u32 i64 u64 fp32 fp64 string object");

		auto typeVoidDecl = parser->parseType(ctx);

		auto typeI8Decl = parser->parseType(ctx);
		auto typeU8Decl = parser->parseType(ctx);
		auto typeI16Decl = parser->parseType(ctx);
		auto typeU16Decl = parser->parseType(ctx);
		auto typeI32Decl = parser->parseType(ctx);
		auto typeU32Decl = parser->parseType(ctx);
		auto typeI64Decl = parser->parseType(ctx);
		auto typeU64Decl = parser->parseType(ctx);

		auto typeFp32Decl = parser->parseType(ctx);
		auto typeFp64Decl = parser->parseType(ctx);

		auto typeStringDecl = parser->parseType(ctx);
		auto typeObjectDecl = parser->parseType(ctx);

		EXPECT_EQ(typeVoidDecl->nullable, false);
		EXPECT_EQ(typeVoidDecl->typeID, TypeDeclID_e::Void);

		EXPECT_EQ(typeI8Decl->nullable, false);
		EXPECT_EQ(typeI8Decl->typeID, TypeDeclID_e::I8);
		EXPECT_EQ(typeU8Decl->nullable, false);
		EXPECT_EQ(typeU8Decl->typeID, TypeDeclID_e::U8);

		EXPECT_EQ(typeI16Decl->nullable, false);
		EXPECT_EQ(typeI16Decl->typeID, TypeDeclID_e::I16);
		EXPECT_EQ(typeU16Decl->nullable, false);
		EXPECT_EQ(typeU16Decl->typeID, TypeDeclID_e::U16);

		EXPECT_EQ(typeI32Decl->nullable, false);
		EXPECT_EQ(typeI32Decl->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(typeU32Decl->nullable, false);
		EXPECT_EQ(typeU32Decl->typeID, TypeDeclID_e::U32);

		EXPECT_EQ(typeI64Decl->nullable, false);
		EXPECT_EQ(typeI64Decl->typeID, TypeDeclID_e::I64);
		EXPECT_EQ(typeU64Decl->nullable, false);
		EXPECT_EQ(typeU64Decl->typeID, TypeDeclID_e::U64);

		EXPECT_EQ(typeFp32Decl->nullable, false);
		EXPECT_EQ(typeFp32Decl->typeID, TypeDeclID_e::Fp32);
		EXPECT_EQ(typeFp64Decl->nullable, false);
		EXPECT_EQ(typeFp64Decl->typeID, TypeDeclID_e::Fp64);

		EXPECT_EQ(typeStringDecl->nullable, false);
		EXPECT_EQ(typeStringDecl->typeID, TypeDeclID_e::String);

		EXPECT_EQ(typeObjectDecl->nullable, false);
		EXPECT_EQ(typeObjectDecl->typeID, TypeDeclID_e::Object);
	}

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
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::I32);
	}

	TEST_F(ParserTypesTest, TestPrimitiveUnsizedArrayType)
	{
		parser->loadSource("i32[]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveUnsizedArrayType)
	{
		parser->loadSource("i32?[]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, true);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveUnsizedNullableArrayType)
	{
		parser->loadSource("i32[]?");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveUnsizedNullableArrayType)
	{
		parser->loadSource("i32?[]?");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, true);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveMultiplesUnsizedLevelsArrayType)
	{
		parser->loadSource("i32[][]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 2);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[1]->arrayType, ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveSizedArrayType)
	{
		parser->loadSource("i32[2]");

		auto typeI32NullableDecl = parser->parseType(ctx);

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, TypeDeclID_e::I32);
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
		EXPECT_EQ(typeI32NullableDecl->typeID, TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, TypeDeclID_e::I32);
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

		EXPECT_EQ(typeDecl->typeID, TypeDeclID_e::Function);
		EXPECT_EQ(typeDecl->nullable, false);

		auto functionTypeDecl = reinterpret_cast<ast::TypeDeclFunction*>(typeDecl.get());

		ASSERT_TRUE(functionTypeDecl != nullptr);

		EXPECT_EQ(functionTypeDecl->returnType->typeID, TypeDeclID_e::Fp32);
		EXPECT_EQ(functionTypeDecl->returnType->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList.size(), 3);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[0]->typeID, TypeDeclID_e::I32);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[0]->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList[1]->typeID, TypeDeclID_e::String);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[1]->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList[2]->typeID, TypeDeclID_e::Named);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[2]->nullable, true);

		auto vectorTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(functionTypeDecl->parameterTypeList[2].get());

		ASSERT_TRUE(vectorTypeDecl != nullptr);
	}

	TEST_F(ParserTypesTest, TestNamedType)
	{
		parser->loadSource("Foo");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->typeID, TypeDeclID_e::Named);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopedReferenceDecl, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNamedFromRootType)
	{
		parser->loadSource("::Foo");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->typeID, TypeDeclID_e::Named);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopedReferenceDecl, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);
	}

	TEST_F(ParserTypesTest, TestArrayNamedType)
	{
		parser->loadSource("Foo[]");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->typeID, TypeDeclID_e::Array);
		EXPECT_EQ(typeDecl->nullable, false);

		auto arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(arrayTypeDecl->valueType.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopedReferenceDecl, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNullableArrayNamedType)
	{
		parser->loadSource("Foo[]?");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->typeID, TypeDeclID_e::Array);
		EXPECT_EQ(typeDecl->nullable, true);

		auto arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ArrayType_e::Unsized);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(arrayTypeDecl->valueType.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopedReferenceDecl, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNamedTypeWithGeneric)
	{
		parser->loadSource("Foo<T, i32?>");

		auto typeDecl = parser->parseType(ctx);

		EXPECT_EQ(typeDecl->typeID, TypeDeclID_e::Named);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->scopedReferenceDecl, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList.size(), 2);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);

		EXPECT_EQ(namedTypeDecl->genericDefinitionList[0]->typeID, TypeDeclID_e::Named);
		EXPECT_EQ(namedTypeDecl->genericDefinitionList[0]->nullable, false);

		auto namedParamType = reinterpret_cast<ast::TypeDeclNamed*>(namedTypeDecl->genericDefinitionList[0].get());

		ASSERT_TRUE(namedParamType != nullptr);

		EXPECT_EQ(namedParamType->identifier, "T");
		EXPECT_EQ(namedParamType->scopedReferenceDecl, nullptr);
		EXPECT_EQ(namedParamType->genericDefinitionList.size(), 0);
		EXPECT_EQ(namedParamType->nullable, false);
		EXPECT_EQ(namedParamType->startFromRoot, false);

		EXPECT_EQ(namedTypeDecl->genericDefinitionList[1]->typeID, TypeDeclID_e::I32);
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