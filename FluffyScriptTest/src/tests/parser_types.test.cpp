#include <memory>
#include "gtest/gtest.h"

#include "fl_parser.h"
#include "fl_exceptions.h"
#include "parser_objects/fl_parser_objects.h"

namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserTypesTest
	 */

	struct ParserTypesTest : public ::testing::Test
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

	TEST_F(ParserTypesTest, TestPrimitiveTypes)
	{
		using namespace parser_objects;

		parser->loadSource("void i8 u8 i16 u16 i32 u32 i64 u64 fp32 fp64 string object");
		parser->nextToken();

		auto typeVoidDecl = ParserObjectTypeDecl::parse(parser.get());

		auto typeI8Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeU8Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeI16Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeU16Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeI32Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeU32Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeI64Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeU64Decl = ParserObjectTypeDecl::parse(parser.get());

		auto typeFp32Decl = ParserObjectTypeDecl::parse(parser.get());
		auto typeFp64Decl = ParserObjectTypeDecl::parse(parser.get());

		auto typeStringDecl = ParserObjectTypeDecl::parse(parser.get());
		auto typeObjectDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeVoidDecl->nullable, false);
		EXPECT_EQ(typeVoidDecl->typeID, ast::TypeDecl::TypeDeclID_e::Void);

		EXPECT_EQ(typeI8Decl->nullable, false);
		EXPECT_EQ(typeI8Decl->typeID, ast::TypeDecl::TypeDeclID_e::I8);
		EXPECT_EQ(typeU8Decl->nullable, false);
		EXPECT_EQ(typeU8Decl->typeID, ast::TypeDecl::TypeDeclID_e::U8);

		EXPECT_EQ(typeI16Decl->nullable, false);
		EXPECT_EQ(typeI16Decl->typeID, ast::TypeDecl::TypeDeclID_e::I16);
		EXPECT_EQ(typeU16Decl->nullable, false);
		EXPECT_EQ(typeU16Decl->typeID, ast::TypeDecl::TypeDeclID_e::U16);

		EXPECT_EQ(typeI32Decl->nullable, false);
		EXPECT_EQ(typeI32Decl->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(typeU32Decl->nullable, false);
		EXPECT_EQ(typeU32Decl->typeID, ast::TypeDecl::TypeDeclID_e::U32);

		EXPECT_EQ(typeI64Decl->nullable, false);
		EXPECT_EQ(typeI64Decl->typeID, ast::TypeDecl::TypeDeclID_e::I64);
		EXPECT_EQ(typeU64Decl->nullable, false);
		EXPECT_EQ(typeU64Decl->typeID, ast::TypeDecl::TypeDeclID_e::U64);

		EXPECT_EQ(typeFp32Decl->nullable, false);
		EXPECT_EQ(typeFp32Decl->typeID, ast::TypeDecl::TypeDeclID_e::Fp32);
		EXPECT_EQ(typeFp64Decl->nullable, false);
		EXPECT_EQ(typeFp64Decl->typeID, ast::TypeDecl::TypeDeclID_e::Fp64);

		EXPECT_EQ(typeStringDecl->nullable, false);
		EXPECT_EQ(typeStringDecl->typeID, ast::TypeDecl::TypeDeclID_e::String);

		EXPECT_EQ(typeObjectDecl->nullable, false);
		EXPECT_EQ(typeObjectDecl->typeID, ast::TypeDecl::TypeDeclID_e::Object);
	}

	TEST_F(ParserTypesTest, TestNullableVoidType)
	{
		using namespace parser_objects;

		parser->loadSource("void?");
		parser->nextToken();

		try
		{
			auto typeVoidDecl = ParserObjectTypeDecl::parse(parser.get());			
			throw std::exception();
		}
		catch (exceptions::custom_exception& e)
		{
			EXPECT_STREQ(e.what(), "'void' type can't be nullable at: line 1, column 1");
		}
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveType)
	{
		using namespace parser_objects;

		parser->loadSource("i32?");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::I32);
	}

	TEST_F(ParserTypesTest, TestPrimitiveUnsizedArrayType)
	{
		using namespace parser_objects;

		parser->loadSource("i32[]");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveUnsizedArrayType)
	{
		using namespace parser_objects;

		parser->loadSource("i32?[]");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, true);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveUnsizedNullableArrayType)
	{
		using namespace parser_objects;

		parser->loadSource("i32[]?");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestNullablePrimitiveUnsizedNullableArrayType)
	{
		using namespace parser_objects;

		parser->loadSource("i32?[]?");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, true);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, true);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveMultiplesUnsizedLevelsArrayType)
	{
		using namespace parser_objects;

		parser->loadSource("i32[][]");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 2);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[1]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);
	}

	TEST_F(ParserTypesTest, TestPrimitiveSizedArrayType)
	{
		using namespace parser_objects;

		parser->loadSource("i32[2]");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Sized);
		
		ast::SizedArrayDecl* sizedArrayDecl = reinterpret_cast<ast::SizedArrayDecl*>(arrayTypeDecl->arrayDeclList[0].get());

		ASSERT_TRUE(sizedArrayDecl != nullptr);

		EXPECT_EQ(sizedArrayDecl->size, 2);
	}

	TEST_F(ParserTypesTest, TestPrimitiveMultiplesMixedLevelsArrayType)
	{
		using namespace parser_objects;

		parser->loadSource("i32[][2]");
		parser->nextToken();

		auto typeI32NullableDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeI32NullableDecl->nullable, false);
		EXPECT_EQ(typeI32NullableDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);

		ast::TypeDeclArray* arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeI32NullableDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->valueType->nullable, false);
		EXPECT_EQ(arrayTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 2);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[1]->arrayType, ast::ArrayDecl::ArrayType_e::Sized);
		
		ast::SizedArrayDecl* sizedArrayDecl = reinterpret_cast<ast::SizedArrayDecl*>(arrayTypeDecl->arrayDeclList[1].get());

		ASSERT_TRUE(sizedArrayDecl != nullptr);

		EXPECT_EQ(sizedArrayDecl->size, 2);
	}

	TEST_F(ParserTypesTest, TestVectorType)
	{
		using namespace parser_objects;

		parser->loadSource("vector<i32>");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Vector);
		EXPECT_EQ(typeDecl->nullable, false);

		auto vectorTypeDecl = reinterpret_cast<ast::TypeDeclVector*>(typeDecl.get());

		ASSERT_TRUE(vectorTypeDecl != nullptr);

		EXPECT_EQ(vectorTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(vectorTypeDecl->valueType->nullable, false);
	}

	TEST_F(ParserTypesTest, TestSetType)
	{
		using namespace parser_objects;

		parser->loadSource("set<i32>");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Set);
		EXPECT_EQ(typeDecl->nullable, false);

		auto setTypeDecl = reinterpret_cast<ast::TypeDeclSet*>(typeDecl.get());

		ASSERT_TRUE(setTypeDecl != nullptr);

		EXPECT_EQ(setTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(setTypeDecl->valueType->nullable, false);
	}

	TEST_F(ParserTypesTest, TestMapType)
	{
		using namespace parser_objects;

		parser->loadSource("map<i32, string>");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Map);
		EXPECT_EQ(typeDecl->nullable, false);

		auto mapTypeDecl = reinterpret_cast<ast::TypeDeclMap*>(typeDecl.get());

		ASSERT_TRUE(mapTypeDecl != nullptr);

		EXPECT_EQ(mapTypeDecl->keyType->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(mapTypeDecl->keyType->nullable, false);

		EXPECT_EQ(mapTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::String);
		EXPECT_EQ(mapTypeDecl->valueType->nullable, false);
	}

	TEST_F(ParserTypesTest, TestFunctionType)
	{
		using namespace parser_objects;

		parser->loadSource("fn(i32, string, vector<i8>? -> fp32)");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Function);
		EXPECT_EQ(typeDecl->nullable, false);

		auto functionTypeDecl = reinterpret_cast<ast::TypeDeclFunction*>(typeDecl.get());

		ASSERT_TRUE(functionTypeDecl != nullptr);

		EXPECT_EQ(functionTypeDecl->returnType->typeID, ast::TypeDecl::TypeDeclID_e::Fp32);
		EXPECT_EQ(functionTypeDecl->returnType->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList.size(), 3);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[0]->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[0]->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList[1]->typeID, ast::TypeDecl::TypeDeclID_e::String);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[1]->nullable, false);

		EXPECT_EQ(functionTypeDecl->parameterTypeList[2]->typeID, ast::TypeDecl::TypeDeclID_e::Vector);
		EXPECT_EQ(functionTypeDecl->parameterTypeList[2]->nullable, true);

		auto vectorTypeDecl = reinterpret_cast<ast::TypeDeclVector*>(functionTypeDecl->parameterTypeList[2].get());

		ASSERT_TRUE(vectorTypeDecl != nullptr);

		EXPECT_EQ(vectorTypeDecl->valueType->typeID, ast::TypeDecl::TypeDeclID_e::I8);
	}

	TEST_F(ParserTypesTest, TestNamedType)
	{
		using namespace parser_objects;

		parser->loadSource("Foo");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Named);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->internalIdentifier, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNamedFromRootType)
	{
		using namespace parser_objects;

		parser->loadSource("::Foo");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Named);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->internalIdentifier, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, true);
	}

	TEST_F(ParserTypesTest, TestArrayNamedType)
	{
		using namespace parser_objects;

		parser->loadSource("Foo[]");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);
		EXPECT_EQ(typeDecl->nullable, false);

		auto arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(arrayTypeDecl->valueType.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->internalIdentifier, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNullableArrayNamedType)
	{
		using namespace parser_objects;

		parser->loadSource("Foo[]?");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Array);
		EXPECT_EQ(typeDecl->nullable, true);

		auto arrayTypeDecl = reinterpret_cast<ast::TypeDeclArray*>(typeDecl.get());

		ASSERT_TRUE(arrayTypeDecl != nullptr);

		EXPECT_EQ(arrayTypeDecl->arrayDeclList.size(), 1);
		EXPECT_EQ(arrayTypeDecl->arrayDeclList[0]->arrayType, ast::ArrayDecl::ArrayType_e::Unsized);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(arrayTypeDecl->valueType.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->internalIdentifier, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefList.size(), 0);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);
	}

	TEST_F(ParserTypesTest, TestNamedTypeWithGeneric)
	{
		using namespace parser_objects;

		parser->loadSource("Foo<T, i32?>");
		parser->nextToken();

		auto typeDecl = ParserObjectTypeDecl::parse(parser.get());

		EXPECT_EQ(typeDecl->typeID, ast::TypeDecl::TypeDeclID_e::Named);
		EXPECT_EQ(typeDecl->nullable, false);

		auto namedTypeDecl = reinterpret_cast<ast::TypeDeclNamed*>(typeDecl.get());

		ASSERT_TRUE(namedTypeDecl != nullptr);

		EXPECT_EQ(namedTypeDecl->identifier, "Foo");
		EXPECT_EQ(namedTypeDecl->internalIdentifier, nullptr);
		EXPECT_EQ(namedTypeDecl->genericDefList.size(), 2);
		EXPECT_EQ(namedTypeDecl->nullable, false);
		EXPECT_EQ(namedTypeDecl->startFromRoot, false);

		EXPECT_EQ(namedTypeDecl->genericDefList[0]->typeID, ast::TypeDecl::TypeDeclID_e::Named);
		EXPECT_EQ(namedTypeDecl->genericDefList[0]->nullable, false);

		auto namedParamType = reinterpret_cast<ast::TypeDeclNamed*>(namedTypeDecl->genericDefList[0].get());

		ASSERT_TRUE(namedParamType != nullptr);

		EXPECT_EQ(namedParamType->identifier, "T");
		EXPECT_EQ(namedParamType->internalIdentifier, nullptr);
		EXPECT_EQ(namedParamType->genericDefList.size(), 0);
		EXPECT_EQ(namedParamType->nullable, false);
		EXPECT_EQ(namedParamType->startFromRoot, false);

		EXPECT_EQ(namedTypeDecl->genericDefList[1]->typeID, ast::TypeDecl::TypeDeclID_e::I32);
		EXPECT_EQ(namedTypeDecl->genericDefList[1]->nullable, true);
	}

	TEST_F(ParserTypesTest, TestManyProdutionsPass)
	{
		using namespace parser_objects;

		parser->loadSourceFromFile("./files/parser/source_1.txt");
		parser->nextToken();

		std::vector<std::unique_ptr<ast::TypeDecl>> typeList;
		int typeCount = 0;
		while (true)
		{
			if (parser->isEof())
			{
				break;
			}
			typeList.push_back(ParserObjectTypeDecl::parse(parser.get()));
			typeCount++;
		}
		ASSERT_EQ(typeCount, 13);
	}
} }