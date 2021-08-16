#include "fl_lex_utils.h"
namespace fluffy { namespace utils {
	Bool LexUtils::isEof(const Token_s& tok)
	{
		return tok.type == TokenType_e::Eof;
	}

	Bool LexUtils::isIdentifier(const Token_s& tok)
	{
		return tok.type == TokenType_e::Identifier;
	}

	Bool LexUtils::isConstantI8(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantI8;
	}

	Bool LexUtils::isConstantU8(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantU8;
	}

	Bool LexUtils::isConstantI16(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantI16;
	}

	Bool LexUtils::isConstantU16(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantU16;
	}

	Bool LexUtils::isConstantI32(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantI32;
	}

	Bool LexUtils::isConstantU32(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantU32;
	}

	Bool LexUtils::isConstantI64(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantI64;
	}

	Bool LexUtils::isConstantU64(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantU64;
	}

	Bool LexUtils::isConstantFp32(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantFp32;
	}

	Bool LexUtils::isConstantFp64(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantFp64;
	}

	Bool LexUtils::isConstantChar(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantChar;
	}

	Bool LexUtils::isConstantString(const Token_s& tok)
	{
		return tok.type == TokenType_e::Constant && tok.subType == TokenSubType_e::ConstantString;
	}
} }