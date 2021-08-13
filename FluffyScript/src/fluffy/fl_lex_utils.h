#pragma once
#include "fl_lex.h"

#define FLUFFY_IS_TOKEN_DECL(tokId) \
	static \
	Bool is##tokId(const Token_s& tok) { \
		return tok.subType == eTST_##tokId; \
	}

#define FLUFFY_IS_TOKEN_DECL_2(name, tokId) \
	static \
	Bool is##name(const Token_s& tok) { \
		return tok.subType == eTST_##tokId; \
	}

namespace fluffy { namespace utils {
	class LexUtils
	{
	public:
		static
		Bool						isEof(const Token_s& tok);

		static
		Bool						isIdentifier(const Token_s& tok);

		static
		Bool						isConstantString(const Token_s& tok);

									FLUFFY_IS_TOKEN_DECL(Include);								// include

									FLUFFY_IS_TOKEN_DECL(From);									// from

									FLUFFY_IS_TOKEN_DECL_2(LeftBracket, LBracket);				// {
									FLUFFY_IS_TOKEN_DECL_2(RightBracket, RBracket);				// }

									FLUFFY_IS_TOKEN_DECL(SemiColon);							// ;

									FLUFFY_IS_TOKEN_DECL(Comma);								// ,

									FLUFFY_IS_TOKEN_DECL(Multiplication);						// *
	};
} }

#undef FLUFFY_IS_TOKEN_DECL
#undef FLUFFY_IS_TOKEN_DECL_2