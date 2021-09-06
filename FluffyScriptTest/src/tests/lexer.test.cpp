/*
 * lexer.test.cpp
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#include <memory>
#include <set>
#include "test.h"
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
		{
			EXPECT_EQ(lex->getToken().value, "include");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Include);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseKeywordNamespace)
	{
		lex->loadSource("namespace");
		{
			EXPECT_EQ(lex->getToken().value, "namespace");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Namespace);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseIdentifier)
	{
		lex->loadSource("test");
		{
			EXPECT_EQ(lex->getToken().value, "test");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseMultiplesTokens)
	{
		lex->loadSource("void test");
		{
			EXPECT_EQ(lex->getToken().value, "void");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Void);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
		lex->nextToken();
		{
			EXPECT_EQ(lex->getToken().value, "test");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 6);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseMultiplesLines)
	{
		lex->loadSource("void test\nu32 love");

		lex->nextToken(); // test
		lex->nextToken(); // u32
		{
			EXPECT_EQ(lex->getToken().value, "u32");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::U32);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // love
		{
			EXPECT_EQ(lex->getToken().value, "love");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseFile)
	{
		String file = getProjectFilePath("files\\lexer\\source_1.txt");
		lex->loadSourceFromFile(file.c_str());
		{
			EXPECT_EQ(lex->getToken().value, "u64");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::U64);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}

		lex->nextToken(); // id
		{
			EXPECT_EQ(lex->getToken().value, "id");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}

		lex->nextToken(); // string
		{
			EXPECT_EQ(lex->getToken().value, "string");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::String);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}

		lex->nextToken(); // identifier
		{
			EXPECT_EQ(lex->getToken().value, "identifier");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 8);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestParseInvalidFile)
	{
		const I8* filename = ".\\files\\lexer\\source_not_exist.txt";

		try
		{
			lex->loadSourceFromFile(filename);
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

		// >
		{
			EXPECT_EQ(lex->getToken().value, ">");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::GreaterThan);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // >=
		{
			EXPECT_EQ(lex->getToken().value, ">=");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::GreaterThanOrEqual);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 3);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // ==
		{
			EXPECT_EQ(lex->getToken().value, "==");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Equal);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 6);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // +=
		{
			EXPECT_EQ(lex->getToken().value, "+=");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::PlusAssign);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 9);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestSkipComments)
	{
		String file = getProjectFilePath("files\\lexer\\source_2.txt");
		lex->loadSourceFromFile(file.c_str());

		// void
		{
			EXPECT_EQ(lex->getToken().value, "void");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Void);
			EXPECT_EQ(lex->getToken().line, 7);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestSkipCommentInvalid)
	{
		try
		{
			String file = getProjectFilePath("files\\lexer\\source_3.txt");
			lex->loadSourceFromFile(file.c_str());

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

		// 0x0
		{
			EXPECT_EQ(lex->getToken().value, "0");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantHex);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // 0x10a
		{
			EXPECT_EQ(lex->getToken().value, "10a");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantHex);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestBinConstant)
	{
		lex->loadSource("0b0 0b101");

		// 0x0
		{
			EXPECT_EQ(lex->getToken().value, "0");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantBin);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // 0x10a
		{
			EXPECT_EQ(lex->getToken().value, "101");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantBin);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestRealConstant)
	{
		lex->loadSource("0.0 0.5f 0.95F");

		// 0.0
		{
			EXPECT_EQ(lex->getToken().value, "0.0");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantFp64);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // 0.5f
		{
			EXPECT_EQ(lex->getToken().value, "0.5");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantFp32);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // 0.95F
		{
			EXPECT_EQ(lex->getToken().value, "0.95");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantFp32);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 10);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestIntegerConstant)
	{
		lex->loadSource("0");

		// 0
		{
			EXPECT_EQ(lex->getToken().value, "0");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantInteger);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestCharacterConstant)
	{
		lex->loadSource("'a' 'b' '0'");

		// 'a'
		{
			EXPECT_EQ(lex->getToken().value, "a");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantChar);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // 'b'
		{
			EXPECT_EQ(lex->getToken().value, "b");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantChar);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // '0'
		{
			EXPECT_EQ(lex->getToken().value, "0");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantChar);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 9);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestStringConstant)
	{
		lex->loadSource("\"test\" \"bola\" \"\\n\\r\" \"'tonhudo'\"");

		// test
		{
			EXPECT_EQ(lex->getToken().value, "test");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantString);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // bola
		{
			EXPECT_EQ(lex->getToken().value, "bola");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantString);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 8);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // \n\r
		{
			EXPECT_EQ(lex->getToken().value, "\\n\\r");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantString);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 15);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}

		lex->nextToken(); // 'tonhudo'
		{
			EXPECT_EQ(lex->getToken().value, "'tonhudo'");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::ConstantString);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 22);
			EXPECT_EQ(lex->getToken().filename, "anom_block");
		}
	}

	TEST_F(LexerWithDirectBufferTest, TestComplexScript)
	{
		String file = getProjectFilePath("files\\lexer\\source_4.txt");
		lex->loadSourceFromFile(file.c_str());

		int tokenCount = 0;
		while (true)
		{
			{
				if (lex->getToken().type == TokenType_e::Eof) {
					break;
				}
			}
			tokenCount++;
			lex->nextToken();
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
		String file = getProjectFilePath("files\\lexer\\source_1.txt");
		lex->loadSourceFromFile(file.c_str());

		// u64
		{
			EXPECT_EQ(lex->getToken().value, "u64");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::U64);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}

		lex->nextToken(); // id
		{
			EXPECT_EQ(lex->getToken().value, "id");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}

		lex->nextToken(); // string
		{
			EXPECT_EQ(lex->getToken().value, "string");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::String);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}

		lex->nextToken(); // identifier
		{
			EXPECT_EQ(lex->getToken().value, "identifier");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 8);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestParseInvalidFile)
	{
		const I8* filename = ".\\files\\lexer\\source_not_exist.txt";

		try
		{
			lex->loadSourceFromFile(filename);
		}
		catch (exceptions::file_not_found_exception& e)
		{
			EXPECT_STREQ(e.what(), "File not found: '.\\files\\lexer\\source_not_exist.txt'");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestSkipComments)
	{
		String file = getProjectFilePath("files\\lexer\\source_2.txt");
		lex->loadSourceFromFile(file.c_str());

		// void
		{
			EXPECT_EQ(lex->getToken().value, "void");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Void);
			EXPECT_EQ(lex->getToken().line, 7);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, file.c_str());
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestSkipCommentInvalid)
	{
		try
		{
			String file = getProjectFilePath("files\\lexer\\source_3.txt");
			lex->loadSourceFromFile(file.c_str());
			lex->nextToken();

			throw std::exception();
		}
		catch (exceptions::unexpected_end_of_file_exception& e)
		{
			EXPECT_STREQ(e.what(), "Unexpected end of file");
		}
	}

	TEST_F(LexerWithLazyBufferTest, TestComplexScript)
	{
		String file = getProjectFilePath("files\\lexer\\source_4.txt");
		lex->loadSourceFromFile(file.c_str());

		int tokenCount = 0;
		while (true)
		{
			{
				if (lex->getToken().type == TokenType_e::Eof) {
					break;
				}
			}
			tokenCount++;
			lex->nextToken();
		}
		EXPECT_EQ(tokenCount, 134);
	}

	/**
	 * Testing
	 */

	TEST_F(LexerWithLazyBufferTest, TestParseSourceCode)
	{
		const I8* filename = "anom_block";

		lex->loadSource("u64 id\nstring identifier");

		// u64
		{
			EXPECT_EQ(lex->getToken().value, "u64");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::U64);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, filename);
		}

		lex->nextToken(); // id
		{
			EXPECT_EQ(lex->getToken().value, "id");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 1);
			EXPECT_EQ(lex->getToken().column, 5);
			EXPECT_EQ(lex->getToken().filename, filename);
		}

		lex->nextToken(); // string
		{
			EXPECT_EQ(lex->getToken().value, "string");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::String);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 1);
			EXPECT_EQ(lex->getToken().filename, filename);
		}

		lex->nextToken(); // identifier
		{
			EXPECT_EQ(lex->getToken().value, "identifier");
			EXPECT_EQ(lex->getToken().type, fluffy::TokenType_e::Identifier);
			EXPECT_EQ(lex->getToken().line, 2);
			EXPECT_EQ(lex->getToken().column, 8);
			EXPECT_EQ(lex->getToken().filename, filename);
		}
	}
} }
