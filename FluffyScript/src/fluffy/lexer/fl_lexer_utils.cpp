#include "fl_lexer_utils.h"
namespace fluffy { namespace utils {
	Bool LexUtils::isEof(const Token_s& tok)
	{
		return tok.type == TokenType_e::Eof;
	}

	Bool LexUtils::isIdentifier(const Token_s& tok)
	{
		return tok.type == TokenType_e::Identifier;
	}

	Bool LexUtils::isConstantInteger(const Token_s& tok)
	{
		return tok.type == TokenType_e::ConstantInteger;
	}

	Bool LexUtils::isConstantFp32(const Token_s& tok)
	{
		return tok.type == TokenType_e::ConstantFp32;
	}

	Bool LexUtils::isConstantFp64(const Token_s& tok)
	{
		return tok.type == TokenType_e::ConstantFp64;
	}

	Bool LexUtils::isConstantChar(const Token_s& tok)
	{
		return tok.type == TokenType_e::ConstantChar;
	}

	Bool LexUtils::isConstantString(const Token_s& tok)
	{
		return tok.type == TokenType_e::ConstantString;
	}
} }