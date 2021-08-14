#include "fl_lex_utils.h"
namespace fluffy { namespace utils {
	Bool LexUtils::isEof(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_EOF;
	}

	Bool LexUtils::isIdentifier(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Identifier;
	}

	Bool LexUtils::isConstantI8(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantI8;
	}

	Bool LexUtils::isConstantU8(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantU8;
	}

	Bool LexUtils::isConstantI16(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantI16;
	}

	Bool LexUtils::isConstantU16(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantU16;
	}

	Bool LexUtils::isConstantI32(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantI32;
	}

	Bool LexUtils::isConstantU32(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantU32;
	}

	Bool LexUtils::isConstantI64(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantI64;
	}

	Bool LexUtils::isConstantU64(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantU64;
	}

	Bool LexUtils::isConstantFp32(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantFp32;
	}

	Bool LexUtils::isConstantFp64(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantFp64;
	}

	Bool LexUtils::isConstantChar(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantChar;
	}

	Bool LexUtils::isConstantString(const Token_s& tok)
	{
		return tok.type == TokenType_e::eTT_Constant && tok.subType == TokenSubType_e::eTST_ConstantString;
	}
} }