/*
 * lexer.test.cpp
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#include <memory>
#include <set>
#include <filesystem>
#include "gtest/gtest.h"
#include "fl_lex.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"

namespace fluffy { namespace detail {
	static const std::tuple<String, TokenSubType_e> keywords[] = {
		std::make_tuple("include", 		eTST_Include),		// ok
		std::make_tuple("from", 		eTST_From),			// ok
		std::make_tuple("namespace",	eTST_Namespace),	// ok
		std::make_tuple("class", 		eTST_Class),		// ok
		std::make_tuple("extends", 		eTST_Extends),		// ok
		std::make_tuple("implements",	eTST_Implements),	// ok
		std::make_tuple("constructor",	eTST_Constructor),	// ok
		std::make_tuple("destructor",	eTST_Destructor),	// ok
		std::make_tuple("super",		eTST_Super),		// ok
		std::make_tuple("public", 		eTST_Public),		// ok
		std::make_tuple("protected",	eTST_Protected),	// ok
		std::make_tuple("private", 		eTST_Private),		// ok
		std::make_tuple("final",		eTST_Final),		// ok
		std::make_tuple("override", 	eTST_Override),		// ok
		std::make_tuple("abstract", 	eTST_Abstract),		// ok
		std::make_tuple("virtual", 		eTST_Virtual),		// ok
		std::make_tuple("interface",	eTST_Interface),	// ok
		std::make_tuple("struct",		eTST_Struct),		// ok
		std::make_tuple("enum", 		eTST_Enum),			// ok
		std::make_tuple("match", 		eTST_Match),		// ok
		std::make_tuple("when", 		eTST_When),			// ok
		std::make_tuple("trait", 		eTST_Trait),		// ok
		std::make_tuple("static", 		eTST_Static),		// ok
		std::make_tuple("const", 		eTST_Const),		// ok
		std::make_tuple("true", 		eTST_True),			// ok
		std::make_tuple("false", 		eTST_False),		// ok
		std::make_tuple("void", 		eTST_Void),			// ok
		std::make_tuple("bool", 		eTST_Bool),			// ok
		std::make_tuple("i8", 			eTST_i8),			// ok
		std::make_tuple("u8", 			eTST_u8),			// ok
		std::make_tuple("i16", 			eTST_i16),			// ok
		std::make_tuple("u16", 			eTST_u16),			// ok
		std::make_tuple("i32", 			eTST_i32),			// ok
		std::make_tuple("u32", 			eTST_u32),			// ok
		std::make_tuple("i64", 			eTST_i64),			// ok
		std::make_tuple("u64", 			eTST_u64),			// ok
		std::make_tuple("fp32",			eTST_fp32),			// ok
		std::make_tuple("fp64",			eTST_fp64),			// ok
		std::make_tuple("string", 		eTST_String),		// ok
		std::make_tuple("map", 			eTST_Map),			// ok
		std::make_tuple("object", 		eTST_Object),		// ok
		std::make_tuple("fn", 			eTST_Fn),			// ok
		std::make_tuple("let", 			eTST_Let),			// ok
		std::make_tuple("this", 		eTST_This),			// ok
		std::make_tuple("self", 		eTST_Self),			// ok
		std::make_tuple("sizeof", 		eTST_SizeOf),		// ok
		std::make_tuple("new", 			eTST_New),			// ok
		std::make_tuple("as", 			eTST_As),			// ok
		std::make_tuple("is", 			eTST_Is),			// ok
		std::make_tuple("goto", 		eTST_Goto),			// ok
		std::make_tuple("if", 			eTST_If),			// ok
		std::make_tuple("else", 		eTST_Else),			// ok
		std::make_tuple("switch", 		eTST_Switch),		// ok
		std::make_tuple("case", 		eTST_Case),			// ok
		std::make_tuple("default", 		eTST_Default),		// ok
		std::make_tuple("continue", 	eTST_Continue),		// ok
		std::make_tuple("break", 		eTST_Break),		// ok
		std::make_tuple("for", 			eTST_For),			// ok
		std::make_tuple("foreach", 		eTST_Foreach),		// ok
		std::make_tuple("in",			eTST_In),			// ok
		std::make_tuple("while", 		eTST_While),		// ok
		std::make_tuple("do", 			eTST_Do),			// ok
		std::make_tuple("try", 			eTST_Try),			// ok
		std::make_tuple("catch",		eTST_Catch),		// ok
		std::make_tuple("finally",		eTST_Finally),		// ok
		std::make_tuple("panic", 		eTST_Panic),		// ok
		std::make_tuple("return", 		eTST_Return),		// ok
		std::make_tuple("null", 		eTST_Null)			// ok
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
			if (std::filesystem::exists(".\\s_cache"))
			{
				std::filesystem::remove(".\\s_cache");
			}
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Include);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Namespace);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Void);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_u32);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // love
		{
			EXPECT_EQ(tok.value, "love");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseFile)
	{
		const std::string filename = ".\\files\\source_1.txt";

		lex->loadFromSource(filename);

		lex->parse(tok); // u64
		{
			EXPECT_EQ(tok.value, "u64");
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_u64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // id
		{
			EXPECT_EQ(tok.value, "id");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // string
		{
			EXPECT_EQ(tok.value, "string");
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_String);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // name
		{
			EXPECT_EQ(tok.value, "name");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, filename);
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseInvalidFile)
	{
		const std::string filename = ".\\files\\source_not_exist.txt";

		try
		{
			lex->loadFromSource(filename);
		}
		catch (exceptions::file_not_found_exception& e)
		{
			EXPECT_STREQ(e.what(), "File not found: '.\\files\\source_not_exist.txt'");
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
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_GreaterThan);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // >=
		{
			EXPECT_EQ(tok.value, ">=");
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_GreaterThanOrEqual);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // ==
		{
			EXPECT_EQ(tok.value, "==");
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_Equal);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 6);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // +=
		{
			EXPECT_EQ(tok.value, "+=");
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_PlusAssign);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 9);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestSkipComments)
	{
		lex->loadFromSource(".\\files\\source_2.txt");

		lex->parse(tok); // void
		{
			EXPECT_EQ(tok.value, "void");
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Void);
			EXPECT_EQ(tok.line, 7);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, ".\\files\\source_2.txt");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestSkipCommentInvalid)
	{
		try
		{
			lex->loadFromSource(".\\files\\source_3.txt");
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantHex);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "10a");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantHex);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantBin);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "101");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantBin);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantFp64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.5f
		{
			EXPECT_EQ(tok.value, "0.5");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.95F
		{
			EXPECT_EQ(tok.value, "0.95");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantFp32);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0i8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0u8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantU8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 7);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 150u64
		{
			EXPECT_EQ(tok.value, "150");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantU64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 11);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 350i64
		{
			EXPECT_EQ(tok.value, "350");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 18);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 12u16
		{
			EXPECT_EQ(tok.value, "12");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantU16);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 25);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 65i32
		{
			EXPECT_EQ(tok.value, "65");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI32);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'b'
		{
			EXPECT_EQ(tok.value, "b");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // '0'
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantChar);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // bola
		{
			EXPECT_EQ(tok.value, "bola");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // \n\r
		{
			EXPECT_EQ(tok.value, "\\n\\r");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 15);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'tonhudo'
		{
			EXPECT_EQ(tok.value, "'tonhudo'");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 22);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestComplexScript)
	{
		lex->loadFromSource(".\\files\\source_4.txt");

		int tokenCount = 0;
		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == eTT_EOF) {
					break;
				}
			}
			tokenCount++;
		}
		EXPECT_EQ(tokenCount, 134);
	}

	TEST_F(LexerWithDirectBufferTest, TestAllKeywordsImpl)
	{
		std::set<U32> keywordsImplSet;

		lex->loadFromSource(".\\files\\source_5.txt");

		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == eTT_Keyword) {
					keywordsImplSet.insert(tok.subType);
				}
				if (tok.type == eTT_EOF) {
					break;
				}
			}
		}

		ASSERT_EQ(detail::keywordsCount, keywordsImplSet.size());
	}

	TEST_F(LexerWithDirectBufferTest, TestAllSymbolsImpl)
	{
		std::set<U32> symbolsImplSet;

		lex->loadFromSource(".\\files\\source_6.txt");

		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == eTT_Symbol) {
					symbolsImplSet.insert(tok.subType);
				}
				if (tok.type == eTT_EOF) {
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Include);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Namespace);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Void);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_u32);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // love
		{
			EXPECT_EQ(tok.value, "love");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

	}

	TEST_F(LexerWithLazyBufferTest, TestParseFile)
	{
		const std::string filename = ".\\files\\source_1.txt";

		lex->loadFromSource(filename);

		lex->parse(tok); // u64
		{
			EXPECT_EQ(tok.value, "u64");
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_u64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // id
		{
			EXPECT_EQ(tok.value, "id");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // string
		{
			EXPECT_EQ(tok.value, "string");
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_String);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // name
		{
			EXPECT_EQ(tok.value, "name");
			EXPECT_EQ(tok.type, fluffy::eTT_Identifier);
			EXPECT_EQ(tok.subType, fluffy::eTST_Unknown);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, filename);
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseInvalidFile)
	{
		const std::string filename = ".\\files\\source_not_exist.txt";

		try
		{
			lex->loadFromSource(filename);
		}
		catch (exceptions::file_not_found_exception& e)
		{
			EXPECT_STREQ(e.what(), "File not found: '.\\files\\source_not_exist.txt'");
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
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_GreaterThan);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // >=
		{
			EXPECT_EQ(tok.value, ">=");
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_GreaterThanOrEqual);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // ==
		{
			EXPECT_EQ(tok.value, "==");
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_Equal);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 6);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // +=
		{
			EXPECT_EQ(tok.value, "+=");
			EXPECT_EQ(tok.type, fluffy::eTT_Symbol);
			EXPECT_EQ(tok.subType, fluffy::eTST_PlusAssign);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 9);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestSkipComments)
	{
		lex->loadFromSource(".\\files\\source_2.txt");

		lex->parse(tok); // void
		{
			EXPECT_EQ(tok.value, "void");
			EXPECT_EQ(tok.type, fluffy::eTT_Keyword);
			EXPECT_EQ(tok.subType, fluffy::eTST_Void);
			EXPECT_EQ(tok.line, 7);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, ".\\files\\source_2.txt");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestSkipCommentInvalid)
	{
		try
		{
			lex->loadFromSource(".\\files\\source_3.txt");
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantHex);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "10a");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantHex);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantBin);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "101");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantBin);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantFp64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.5f
		{
			EXPECT_EQ(tok.value, "0.5");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.95F
		{
			EXPECT_EQ(tok.value, "0.95");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantFp32);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0i8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0u8
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantU8);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 7);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 150u64
		{
			EXPECT_EQ(tok.value, "150");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantU64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 11);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 350i64
		{
			EXPECT_EQ(tok.value, "350");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 18);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 12u16
		{
			EXPECT_EQ(tok.value, "12");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantU16);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 25);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 65i32
		{
			EXPECT_EQ(tok.value, "65");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantI32);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'b'
		{
			EXPECT_EQ(tok.value, "b");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // '0'
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantChar);
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
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // bola
		{
			EXPECT_EQ(tok.value, "bola");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // \n\r
		{
			EXPECT_EQ(tok.value, "\\n\\r");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 15);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'tonhudo'
		{
			EXPECT_EQ(tok.value, "'tonhudo'");
			EXPECT_EQ(tok.type, fluffy::eTT_Constant);
			EXPECT_EQ(tok.subType, fluffy::eTST_ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 22);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestComplexScript)
	{
		lex->loadFromSource(".\\files\\source_4.txt");

		int tokenCount = 0;
		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == eTT_EOF) {
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

		lex->loadFromSource(".\\files\\source_5.txt");

		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == eTT_Keyword) {
					keywordsImplSet.insert(tok.subType);
				}
				if (tok.type == eTT_EOF) {
					break;
				}
			}
		}

		ASSERT_EQ(detail::keywordsCount, keywordsImplSet.size());
	}

	TEST_F(LexerWithLazyBufferTest, TestAllSymbolsImpl)
	{
		std::set<U32> symbolsImplSet;

		lex->loadFromSource(".\\files\\source_6.txt");

		while (true)
		{
			try
			{
				lex->parse(tok);
				{
					if (tok.subType == eTST_Dot) {
						std::cout << "," << std::endl;
					}
					if (tok.type == eTT_Symbol) {
						symbolsImplSet.insert(tok.subType);
					}
					if (tok.type == eTT_EOF) {
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
