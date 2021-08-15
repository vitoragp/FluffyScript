/*
 * lexer.test.cpp
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#include <memory>
#include <set>
#include "gtest/gtest.h"
#include "fl_lex.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"

namespace fluffy { namespace detail {
	static const std::tuple<String, TokenSubType_e> keywords[] = {
		std::make_tuple("include", 		TokenSubType_e::eTST_Include),		// ok
		std::make_tuple("from", 		TokenSubType_e::eTST_From),			// ok
		std::make_tuple("export", 		TokenSubType_e::eTST_Export),		// ok
		std::make_tuple("namespace",	TokenSubType_e::eTST_Namespace),	// ok
		std::make_tuple("class", 		TokenSubType_e::eTST_Class),		// ok
		std::make_tuple("extends", 		TokenSubType_e::eTST_Extends),		// ok
		std::make_tuple("implements",	TokenSubType_e::eTST_Implements),	// ok
		std::make_tuple("constructor",	TokenSubType_e::eTST_Constructor),	// ok
		std::make_tuple("destructor",	TokenSubType_e::eTST_Destructor),	// ok
		std::make_tuple("super",		TokenSubType_e::eTST_Super),		// ok
		std::make_tuple("public", 		TokenSubType_e::eTST_Public),		// ok
		std::make_tuple("protected",	TokenSubType_e::eTST_Protected),	// ok
		std::make_tuple("private", 		TokenSubType_e::eTST_Private),		// ok
		std::make_tuple("final",		TokenSubType_e::eTST_Final),		// ok
		std::make_tuple("override", 	TokenSubType_e::eTST_Override),		// ok
		std::make_tuple("abstract", 	TokenSubType_e::eTST_Abstract),		// ok
		std::make_tuple("virtual", 		TokenSubType_e::eTST_Virtual),		// ok
		std::make_tuple("interface",	TokenSubType_e::eTST_Interface),	// ok
		std::make_tuple("struct",		TokenSubType_e::eTST_Struct),		// ok
		std::make_tuple("enum", 		TokenSubType_e::eTST_Enum),			// ok
		std::make_tuple("match", 		TokenSubType_e::eTST_Match),		// ok
		std::make_tuple("when", 		TokenSubType_e::eTST_When),			// ok
		std::make_tuple("trait", 		TokenSubType_e::eTST_Trait),		// ok
		std::make_tuple("static", 		TokenSubType_e::eTST_Static),		// ok
		std::make_tuple("const", 		TokenSubType_e::eTST_Const),		// ok
		std::make_tuple("true", 		TokenSubType_e::eTST_True),			// ok
		std::make_tuple("false", 		TokenSubType_e::eTST_False),		// ok
		std::make_tuple("void", 		TokenSubType_e::eTST_Void),			// ok
		std::make_tuple("bool", 		TokenSubType_e::eTST_Bool),			// ok
		std::make_tuple("i8", 			TokenSubType_e::eTST_I8),			// ok
		std::make_tuple("u8", 			TokenSubType_e::eTST_U8),			// ok
		std::make_tuple("i16", 			TokenSubType_e::eTST_I16),			// ok
		std::make_tuple("u16", 			TokenSubType_e::eTST_U16),			// ok
		std::make_tuple("i32", 			TokenSubType_e::eTST_I32),			// ok
		std::make_tuple("u32", 			TokenSubType_e::eTST_U32),			// ok
		std::make_tuple("i64", 			TokenSubType_e::eTST_I64),			// ok
		std::make_tuple("u64", 			TokenSubType_e::eTST_U64),			// ok
		std::make_tuple("fp32",			TokenSubType_e::eTST_Fp32),			// ok
		std::make_tuple("fp64",			TokenSubType_e::eTST_Fp64),			// ok
		std::make_tuple("string", 		TokenSubType_e::eTST_String),		// ok
		std::make_tuple("vector", 		TokenSubType_e::eTST_Vector),		// ok
		std::make_tuple("set", 			TokenSubType_e::eTST_Set),			// ok
		std::make_tuple("map", 			TokenSubType_e::eTST_Map),			// ok
		std::make_tuple("object", 		TokenSubType_e::eTST_Object),		// ok
		std::make_tuple("fn", 			TokenSubType_e::eTST_Fn),			// ok
		std::make_tuple("let", 			TokenSubType_e::eTST_Let),			// ok
		std::make_tuple("this", 		TokenSubType_e::eTST_This),			// ok
		std::make_tuple("self", 		TokenSubType_e::eTST_Self),			// ok
		std::make_tuple("sizeof", 		TokenSubType_e::eTST_SizeOf),		// ok
		std::make_tuple("new", 			TokenSubType_e::eTST_New),			// ok
		std::make_tuple("as", 			TokenSubType_e::eTST_As),			// ok
		std::make_tuple("is", 			TokenSubType_e::eTST_Is),			// ok
		std::make_tuple("goto", 		TokenSubType_e::eTST_Goto),			// ok
		std::make_tuple("if", 			TokenSubType_e::eTST_If),			// ok
		std::make_tuple("else", 		TokenSubType_e::eTST_Else),			// ok
		std::make_tuple("continue", 	TokenSubType_e::eTST_Continue),		// ok
		std::make_tuple("break", 		TokenSubType_e::eTST_Break),		// ok
		std::make_tuple("for", 			TokenSubType_e::eTST_For),			// ok
		std::make_tuple("foreach", 		TokenSubType_e::eTST_Foreach),		// ok
		std::make_tuple("in",			TokenSubType_e::eTST_In),			// ok
		std::make_tuple("while", 		TokenSubType_e::eTST_While),		// ok
		std::make_tuple("do", 			TokenSubType_e::eTST_Do),			// ok
		std::make_tuple("try", 			TokenSubType_e::eTST_Try),			// ok
		std::make_tuple("catch",		TokenSubType_e::eTST_Catch),		// ok
		std::make_tuple("finally",		TokenSubType_e::eTST_Finally),		// ok
		std::make_tuple("panic", 		TokenSubType_e::eTST_Panic),		// ok
		std::make_tuple("return", 		TokenSubType_e::eTST_Return),		// ok
		std::make_tuple("null", 		TokenSubType_e::eTST_Null)			// ok
	};

	const U32 keywordsCount = sizeof(detail::keywords) / sizeof(std::tuple<String, TokenSubType_e>);
} }

namespace fluffy { namespace testing {
	using fluffy::lexer::Lexer;
	using fluffy::Token_s;
	using std::unique_ptr;

	/**
	 * LexerWithDirectBufferTest
	 */

	struct LexerWithDirectBufferTest : public ::testing::Test
	{
		unique_ptr<Lexer> 	lex;
		Token_s 			tok;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			lex = std::make_unique<Lexer>(new DirectBuffer());
		}
	};

	/**
	 * Testing
	 */

	TEST_F(LexerWithDirectBufferTest, TestParseKeywordInclude)
	{
		lex->loadSource("include");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "include");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Include);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseKeywordNamespace)
	{
		lex->loadSource("namespace");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "namespace");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Namespace);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseIdentifier)
	{
		lex->loadSource("test");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseMultiplesTokens)
	{
		lex->loadSource("void test");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "void");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Void);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 6);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseMultiplesLines)
	{
		lex->loadSource("void test\nu32 love");

		lex->parse(tok); // void
		lex->parse(tok); // test

		lex->parse(tok); // u32
		{
			EXPECT_EQ(tok.value, "u32");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_U32);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // love
		{
			EXPECT_EQ(tok.value, "love");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseFile)
	{
		const std::string filename = ".\\files\\lexer\\source_1.txt";

		lex->loadFromSource(filename);

		lex->parse(tok); // u64
		{
			EXPECT_EQ(tok.value, "u64");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_U64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // id
		{
			EXPECT_EQ(tok.value, "id");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // string
		{
			EXPECT_EQ(tok.value, "string");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_String);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // name
		{
			EXPECT_EQ(tok.value, "name");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, filename);
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseInvalidFile)
	{
		const std::string filename = ".\\files\\lexer\\source_not_exist.txt";

		try
		{
			lex->loadFromSource(filename);
		}
		catch (exceptions::file_not_found_exception& e)
		{
			EXPECT_STREQ(e.what(), "File not found: '.\\files\\lexer\\source_not_exist.txt'");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseInvalidToken)
	{
		try
		{
			lex->loadSource("#");
			lex->parse(tok);

			throw std::exception();
		}
		catch (exceptions::unexpected_token_exception& e)
		{
			EXPECT_STREQ(e.what(), "Unexpected token '#' at: line 1, column 1");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseSymbol)
	{
		lex->loadSource("> >= == +=");

		lex->parse(tok); // >
		{
			EXPECT_EQ(tok.value, ">");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_GreaterThan);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // >=
		{
			EXPECT_EQ(tok.value, ">=");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_GreaterThanOrEqual);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // ==
		{
			EXPECT_EQ(tok.value, "==");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Equal);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 6);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // +=
		{
			EXPECT_EQ(tok.value, "+=");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_PlusAssign);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 9);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestSkipComments)
	{
		lex->loadFromSource(".\\files\\lexer\\source_2.txt");

		lex->parse(tok); // void
		{
			EXPECT_EQ(tok.value, "void");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Void);
			EXPECT_EQ(tok.line, 7);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, ".\\files\\lexer\\source_2.txt");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestSkipCommentInvalid)
	{
		try
		{
			lex->loadFromSource(".\\files\\lexer\\source_3.txt");
			lex->parse(tok);

			throw std::exception();
		}
		catch (exceptions::unexpected_end_of_file_exception& e)
		{
			EXPECT_STREQ(e.what(), "Unexpected end of file");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestHexConstant)
	{
		lex->loadSource("0x0 0x10a");

		lex->parse(tok); // 0x0
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantHex);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "10a");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantHex);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestBinConstant)
	{
		lex->loadSource("0b0 0b101");

		lex->parse(tok); // 0x0
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantBin);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "101");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantBin);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestRealConstant)
	{
		lex->loadSource("0.0 0.5f 0.95F");

		lex->parse(tok); // 0.0
		{
			EXPECT_EQ(tok.value, "0.0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantFp64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.5f
		{
			EXPECT_EQ(tok.value, "0.5");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.95F
		{
			EXPECT_EQ(tok.value, "0.95");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 10);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestIntegerConstant)
	{
		lex->loadSource("0 0i8 0u8 150u64 350i64 12u16 65i32");

		lex->parse(tok); // 0
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0i8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0u8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantU8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 7);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 150u64
		{
			EXPECT_EQ(tok.value, "150");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantU64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 11);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 350i64
		{
			EXPECT_EQ(tok.value, "350");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 18);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 12u16
		{
			EXPECT_EQ(tok.value, "12");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantU16);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 25);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 65i32
		{
			EXPECT_EQ(tok.value, "65");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 31);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestCharacterConstant)
	{
		lex->loadSource("'a' 'b' '0'");

		lex->parse(tok); // 'a'
		{
			EXPECT_EQ(tok.value, "a");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'b'
		{
			EXPECT_EQ(tok.value, "b");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // '0'
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 9);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestStringConstant)
	{
		lex->loadSource("\"test\" \"bola\" \"\\n\\r\" \"'tonhudo'\"");

		lex->parse(tok); // test
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // bola
		{
			EXPECT_EQ(tok.value, "bola");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // \n\r
		{
			EXPECT_EQ(tok.value, "\\n\\r");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 15);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'tonhudo'
		{
			EXPECT_EQ(tok.value, "'tonhudo'");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 22);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestComplexScript)
	{
		lex->loadFromSource(".\\files\\lexer\\source_4.txt");

		int tokenCount = 0;
		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == TokenType_e::eTT_EOF) {
					break;
				}
			}
			tokenCount++;
		}
		EXPECT_EQ(tokenCount, 134);
	}

	TEST_F(LexerWithDirectBufferTest, TestAllKeywordsImpl)
	{
		std::set<TokenSubType_e> keywordsImplSet;

		lex->loadFromSource(".\\files\\lexer\\source_5.txt");

		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == TokenType_e::eTT_Keyword) {
					keywordsImplSet.insert(tok.subType);
				}
				if (tok.type == TokenType_e::eTT_EOF) {
					break;
				}
			}
		}

		for (int i = 0; i < static_cast<int>(detail::keywordsCount); i++)
		{
			auto keyword = keywordsImplSet.find(std::get<1>(detail::keywords[i]));

			if (keyword == keywordsImplSet.end())
			{
				std::cerr << "[          ] keyword = " << std::get<0>(detail::keywords[i]) << std::endl;
			}
		}

		ASSERT_EQ(detail::keywordsCount, keywordsImplSet.size());
	}

	TEST_F(LexerWithDirectBufferTest, TestAllSymbolsImpl)
	{
		std::set<U32> symbolsImplSet;

		lex->loadFromSource(".\\files\\lexer\\source_6.txt");

		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == TokenType_e::eTT_Symbol) {
					symbolsImplSet.insert(static_cast<U32>(tok.subType));
				}
				if (tok.type == TokenType_e::eTT_EOF) {
					break;
				}
			}
		}

		ASSERT_EQ(46, symbolsImplSet.size());
	}
} }

namespace fluffy { namespace testing {
	using fluffy::lexer::Lexer;
	using fluffy::Token_s;
	using std::unique_ptr;

	/**
	 * LexerWithLazyBufferTest
	 */

	struct LexerWithLazyBufferTest : public ::testing::Test
	{
		unique_ptr<Lexer> 	lex;
		Token_s 			tok;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			lex = std::make_unique<Lexer>(new LazyBuffer(5));
		}
	};

	/**
	 * Testing
	 */

	TEST_F(LexerWithLazyBufferTest, TestParseKeywordInclude)
	{
		lex->loadSource("include");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "include");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Include);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseKeywordNamespace)
	{
		lex->loadSource("namespace");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "namespace");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Namespace);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseIdentifier)
	{
		lex->loadSource("test");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseMultiplesTokens)
	{
		lex->loadSource("void test");
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "void");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Void);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 6);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseMultiplesLines)
	{
		lex->loadSource("void test\nu32 love");

		lex->parse(tok); // void
		lex->parse(tok); // test

		lex->parse(tok); // u32
		{
			EXPECT_EQ(tok.value, "u32");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_U32);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // love
		{
			EXPECT_EQ(tok.value, "love");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

	}

	TEST_F(LexerWithLazyBufferTest, TestParseFile)
	{
		const std::string filename = ".\\files\\lexer\\source_1.txt";

		lex->loadFromSource(filename);

		lex->parse(tok); // u64
		{
			EXPECT_EQ(tok.value, "u64");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_U64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // id
		{
			EXPECT_EQ(tok.value, "id");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // string
		{
			EXPECT_EQ(tok.value, "string");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_String);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // name
		{
			EXPECT_EQ(tok.value, "name");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, filename);
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseInvalidFile)
	{
		const std::string filename = ".\\files\\lexer\\source_not_exist.txt";

		try
		{
			lex->loadFromSource(filename);
		}
		catch (exceptions::file_not_found_exception& e)
		{
			EXPECT_STREQ(e.what(), "File not found: '.\\files\\lexer\\source_not_exist.txt'");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseInvalidToken)
	{
		try
		{
			lex->loadSource("#");
			lex->parse(tok);

			throw std::exception();
		}
		catch (exceptions::unexpected_token_exception& e)
		{
			EXPECT_STREQ(e.what(), "Unexpected token '#' at: line 1, column 1");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseSymbol)
	{
		lex->loadSource("> >= == +=");

		lex->parse(tok); // >
		{
			EXPECT_EQ(tok.value, ">");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_GreaterThan);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // >=
		{
			EXPECT_EQ(tok.value, ">=");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_GreaterThanOrEqual);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // ==
		{
			EXPECT_EQ(tok.value, "==");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Equal);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 6);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // +=
		{
			EXPECT_EQ(tok.value, "+=");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_PlusAssign);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 9);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestSkipComments)
	{
		lex->loadFromSource(".\\files\\lexer\\source_2.txt");

		lex->parse(tok); // void
		{
			EXPECT_EQ(tok.value, "void");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_Void);
			EXPECT_EQ(tok.line, 7);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, ".\\files\\lexer\\source_2.txt");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestSkipCommentInvalid)
	{
		try
		{
			lex->loadFromSource(".\\files\\lexer\\source_3.txt");
			lex->parse(tok);

			throw std::exception();
		}
		catch (exceptions::unexpected_end_of_file_exception& e)
		{
			EXPECT_STREQ(e.what(), "Unexpected end of file");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestHexConstant)
	{
		lex->loadSource("0x0 0x10a");

		lex->parse(tok); // 0x0
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantHex);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "10a");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantHex);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestBinConstant)
	{
		lex->loadSource("0b0 0b101");

		lex->parse(tok); // 0x0
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantBin);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "101");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantBin);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestRealConstant)
	{
		lex->loadSource("0.0 0.5f 0.95F");

		lex->parse(tok); // 0.0
		{
			EXPECT_EQ(tok.value, "0.0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantFp64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.5f
		{
			EXPECT_EQ(tok.value, "0.5");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.95F
		{
			EXPECT_EQ(tok.value, "0.95");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 10);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestIntegerConstant)
	{
		lex->loadSource("0 0i8 0u8 150u64 350i64 12u16 65i32");

		lex->parse(tok); // 0
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0i8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0u8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantU8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 7);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 150u64
		{
			EXPECT_EQ(tok.value, "150");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantU64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 11);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 350i64
		{
			EXPECT_EQ(tok.value, "350");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 18);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 12u16
		{
			EXPECT_EQ(tok.value, "12");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantU16);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 25);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 65i32
		{
			EXPECT_EQ(tok.value, "65");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantI32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 31);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestCharacterConstant)
	{
		lex->loadSource("'a' 'b' '0'");

		lex->parse(tok); // 'a'
		{
			EXPECT_EQ(tok.value, "a");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'b'
		{
			EXPECT_EQ(tok.value, "b");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // '0'
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 9);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestStringConstant)
	{
		lex->loadSource("\"test\" \"bola\" \"\\n\\r\" \"'tonhudo'\"");

		lex->parse(tok); // test
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // bola
		{
			EXPECT_EQ(tok.value, "bola");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // \n\r
		{
			EXPECT_EQ(tok.value, "\\n\\r");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 15);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'tonhudo'
		{
			EXPECT_EQ(tok.value, "'tonhudo'");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::TokenSubType_e::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 22);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestComplexScript)
	{
		lex->loadFromSource(".\\files\\lexer\\source_4.txt");

		int tokenCount = 0;
		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == TokenType_e::eTT_EOF) {
					break;
				}
			}
			tokenCount++;
		}
		EXPECT_EQ(tokenCount, 134);
	}

	TEST_F(LexerWithLazyBufferTest, TestAllKeywordsImpl)
	{
		std::set<U32> keywordsImplSet;

		lex->loadFromSource(".\\files\\lexer\\source_5.txt");

		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == TokenType_e::eTT_Keyword) {
					keywordsImplSet.insert(static_cast<U32>(tok.subType));
				}
				if (tok.type == TokenType_e::eTT_EOF) {
					break;
				}
			}
		}

		ASSERT_EQ(detail::keywordsCount, keywordsImplSet.size());
	}

	TEST_F(LexerWithLazyBufferTest, TestAllSymbolsImpl)
	{
		std::set<U32> symbolsImplSet;

		lex->loadFromSource(".\\files\\lexer\\source_6.txt");

		while (true)
		{
			try
			{
				lex->parse(tok);
				{
					if (tok.subType == TokenSubType_e::eTST_Dot) {
						std::cout << "," << std::endl;
					}
					if (tok.type == TokenType_e::eTT_Symbol) {
						symbolsImplSet.insert(static_cast<U32>(tok.subType));
					}
					if (tok.type == TokenType_e::eTT_EOF) {
						break;
					}
				}
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}

		ASSERT_EQ(46, symbolsImplSet.size());
	}
} }
