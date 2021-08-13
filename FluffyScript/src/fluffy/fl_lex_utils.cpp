#include "fl_lex_utils.h"
namespace fluffy { namespace utils {
	Bool LexUtils::isEof(const Token_s& tok)
	{
		return tok.type == eTT_EOF;
	}

	Bool LexUtils::isIdentifier(const Token_s& tok)
	{
		return tok.type == eTT_Identifier;
	}

	Bool LexUtils::isConstantString(const Token_s& tok)
	{
		return tok.type == eTT_Constant && tok.subType == eTST_ConstantString;
	}
} }