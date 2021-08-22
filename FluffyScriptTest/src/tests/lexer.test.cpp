/*
 * lexer.test.cpp
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#include <memory>
#include <set>
#include "gtest\gtest.h"
#include "lexer\fl_lexer.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"

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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Include);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Namespace);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Identifier);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Void);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
		lex->parse(tok);
		{
			EXPECT_EQ(tok.value, "test");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Identifier);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::U32);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // love
		{
			EXPECT_EQ(tok.value, "love");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Identifier);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::U64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // id
		{
			EXPECT_EQ(tok.value, "id");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // string
		{
			EXPECT_EQ(tok.value, "string");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::String);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // identifier
		{
			EXPECT_EQ(tok.value, "identifier");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Identifier);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::GreaterThan);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // >=
		{
			EXPECT_EQ(tok.value, ">=");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::GreaterThanOrEqual);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 3);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // ==
		{
			EXPECT_EQ(tok.value, "==");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Equal);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 6);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // +=
		{
			EXPECT_EQ(tok.value, "+=");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::PlusAssign);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Void);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantHex);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "10a");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantHex);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantBin);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0x10a
		{
			EXPECT_EQ(tok.value, "101");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantBin);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantFp64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.5f
		{
			EXPECT_EQ(tok.value, "0.5");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 0.95F
		{
			EXPECT_EQ(tok.value, "0.95");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantFp32);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 10);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestIntegerConstant)
	{
		lex->loadSource("0");

		lex->parse(tok); // 0
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantInteger);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestCharacterConstant)
	{
		lex->loadSource("'a' 'b' '0'");

		lex->parse(tok); // 'a'
		{
			EXPECT_EQ(tok.value, "a");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'b'
		{
			EXPECT_EQ(tok.value, "b");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantChar);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // '0'
		{
			EXPECT_EQ(tok.value, "0");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantChar);
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
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // bola
		{
			EXPECT_EQ(tok.value, "bola");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 8);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // \n\r
		{
			EXPECT_EQ(tok.value, "\\n\\r");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantString);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 15);
			EXPECT_EQ(tok.filename, "anom_block");
		}

		lex->parse(tok); // 'tonhudo'
		{
			EXPECT_EQ(tok.value, "'tonhudo'");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::ConstantString);
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
				if (tok.type == TokenType_e::Eof) {
					break;
				}
			}
			tokenCount++;
		}
		EXPECT_EQ(tokenCount, 134);
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
			lex = std::make_unique<Lexer>(new LazyBuffer(2048));
		}
	};

	/**
	 * Testing
	 */

	TEST_F(LexerWithLazyBufferTest, TestParseFile)
	{
		const std::string filename = ".\\files\\lexer\\source_1.txt";

		lex->loadFromSource(filename);

		lex->parse(tok); // u64
		{
			EXPECT_EQ(tok.value, "u64");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::U64);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // id
		{
			EXPECT_EQ(tok.value, "id");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(tok.line, 1);
			EXPECT_EQ(tok.column, 5);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // string
		{
			EXPECT_EQ(tok.value, "string");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::String);
			EXPECT_EQ(tok.line, 2);
			EXPECT_EQ(tok.column, 1);
			EXPECT_EQ(tok.filename, filename);
		}

		lex->parse(tok); // identifier
		{
			EXPECT_EQ(tok.value, "identifier");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Identifier);
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

	TEST_F(LexerWithLazyBufferTest, TestSkipComments)
	{
		lex->loadFromSource(".\\files\\lexer\\source_2.txt");

		lex->parse(tok); // void
		{
			EXPECT_EQ(tok.value, "void");
			EXPECT_EQ(tok.type, fluffy::TokenType_e::Void);
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

	TEST_F(LexerWithLazyBufferTest, TestComplexScript)
	{
		lex->loadFromSource(".\\files\\lexer\\source_4.txt");

		int tokenCount = 0;
		while (true)
		{
			lex->parse(tok);
			{
				if (tok.type == TokenType_e::Eof) {
					break;
				}
			}
			tokenCount++;
		}
		EXPECT_EQ(tokenCount, 134);
	}
} }
