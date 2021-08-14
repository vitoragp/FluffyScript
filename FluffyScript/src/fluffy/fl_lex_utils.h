#pragma once
#include "fl_lex.h"

#define FLUFFY_IS_TOKEN_DECL(tokId) \
	static \
	Bool is##tokId(const Token_s& tok) { \
		return tok.subType == TokenSubType_e::eTST_##tokId; \
	}

#define FLUFFY_IS_TOKEN_DECL_2(name, tokId) \
	static \
	Bool is##name(const Token_s& tok) { \
		return tok.subType == TokenSubType_e::eTST_##tokId; \
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
		Bool						isConstantI8(const Token_s& tok);

		static
		Bool						isConstantU8(const Token_s& tok);

		static
		Bool						isConstantI16(const Token_s& tok);

		static
		Bool						isConstantU16(const Token_s& tok);

		static
		Bool						isConstantI32(const Token_s& tok);

		static
		Bool						isConstantU32(const Token_s& tok);

		static
		Bool						isConstantI64(const Token_s& tok);

		static
		Bool						isConstantU64(const Token_s& tok);

		static
		Bool						isConstantFp32(const Token_s& tok);

		static
		Bool						isConstantFp64(const Token_s& tok);

		static
		Bool						isConstantChar(const Token_s& tok);

		static
		Bool						isConstantString(const Token_s& tok);

									FLUFFY_IS_TOKEN_DECL(Include);								// include

									FLUFFY_IS_TOKEN_DECL(From);									// from

									FLUFFY_IS_TOKEN_DECL(Namespace);							// namespace

									FLUFFY_IS_TOKEN_DECL(Export);								// export

									FLUFFY_IS_TOKEN_DECL(True);									// true
									FLUFFY_IS_TOKEN_DECL(False);								// False

									FLUFFY_IS_TOKEN_DECL_2(LeftBracket, LBracket);				// {
									FLUFFY_IS_TOKEN_DECL_2(RightBracket, RBracket);				// }

									FLUFFY_IS_TOKEN_DECL(ScopeResolution);						// ::

									FLUFFY_IS_TOKEN_DECL(SemiColon);							// ;

									FLUFFY_IS_TOKEN_DECL(Comma);								// ,

									FLUFFY_IS_TOKEN_DECL(Multiplication);						// *
	};
} }

#undef FLUFFY_IS_TOKEN_DECL
#undef FLUFFY_IS_TOKEN_DECL_2