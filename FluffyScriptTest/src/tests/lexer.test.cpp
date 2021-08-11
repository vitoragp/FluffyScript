/*
 * lexer.test.cpp
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#include <memory>
#include <gtest/gtest.h>
#include "fl_lex.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	using fluffy::lexer::Lexer;
	using fluffy::Token_s;
	using std::unique_ptr;

	/**
	 * LexerTest
	 */

	struct LexerTest : public ::testing::Test
	{
		unique_ptr<Lexer> 	lex;
		Token_s 			tok;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			lex = std::make_unique<Lexer>();
		}
	};

	/**
	 * Testing
	 */

	TEST_F(LexerTest, TestParseKeywordInclude)
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

	TEST_F(LexerTest, TestParseKeywordNamespace)
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

	TEST_F(LexerTest, TestParseIdentifier)
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

	TEST_F(LexerTest, TestParseMultiplesTokens)
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

	TEST_F(LexerTest, TestParseMultiplesLines)
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

	TEST_F(LexerTest, TestParseFile)
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

	TEST_F(LexerTest, TestParseInvalidFile)
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

	TEST_F(LexerTest, TestParseInvalidToken)
	{
		try
		{
			lex->loadSource("#");
			lex->parse(tok);

			throw std::exception();
		}
		catch (exceptions::unexpected_token_exception& e)
		{
			EXPECT_STREQ(e.what(), "Unexpected token at: line 1, column 1");
		}
	}

	TEST_F(LexerTest, TestParseSymbol)
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

	TEST_F(LexerTest, TestSkipComments)
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

	TEST_F(LexerTest, TestSkipCommentInvalid)
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

	TEST_F(LexerTest, TestHexConstant)
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

	TEST_F(LexerTest, TestBinConstant)
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

	TEST_F(LexerTest, TestRealConstant)
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

	TEST_F(LexerTest, TestIntegerConstant)
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

	TEST_F(LexerTest, TestCharacterConstant)
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

	TEST_F(LexerTest, TestStringConstant)
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

	TEST_F(LexerTest, TestComplexScript)
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
} }
