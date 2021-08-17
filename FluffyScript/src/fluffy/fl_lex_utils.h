#pragma once
#include "fl_lex.h"

#define FLUFFY_IS_TOKEN_DECL(tokId) \
	static \
	Bool is##tokId(const Token_s& tok) { \
		return tok.subType == TokenSubType_e::tokId; \
	}

#define FLUFFY_IS_TOKEN_DECL_2(name, tokId) \
	static \
	Bool is##name(const Token_s& tok) { \
		return tok.subType == TokenSubType_e::tokId; \
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
									FLUFFY_IS_TOKEN_DECL(Export);								// export
									FLUFFY_IS_TOKEN_DECL(Namespace);							// namespace
									FLUFFY_IS_TOKEN_DECL(Class);								// class
									FLUFFY_IS_TOKEN_DECL(Extends);								// extends
									FLUFFY_IS_TOKEN_DECL(Implements);							// implements
									FLUFFY_IS_TOKEN_DECL(Where);								// where
									FLUFFY_IS_TOKEN_DECL(Constructor);							// 
									FLUFFY_IS_TOKEN_DECL(Destructor);							// 
									FLUFFY_IS_TOKEN_DECL(Super);								// 
									FLUFFY_IS_TOKEN_DECL(Public);								// 
									FLUFFY_IS_TOKEN_DECL(Protected);							// 
									FLUFFY_IS_TOKEN_DECL(Private);								// 
									FLUFFY_IS_TOKEN_DECL(Final);								// 
									FLUFFY_IS_TOKEN_DECL(Override);								// 
									FLUFFY_IS_TOKEN_DECL(Abstract);							// abtract

									FLUFFY_IS_TOKEN_DECL(Virtual);
									FLUFFY_IS_TOKEN_DECL(Interface);
									FLUFFY_IS_TOKEN_DECL(Struct);
									FLUFFY_IS_TOKEN_DECL(Enum);
									FLUFFY_IS_TOKEN_DECL(Match);
									FLUFFY_IS_TOKEN_DECL(When);
									FLUFFY_IS_TOKEN_DECL(Trait);
									FLUFFY_IS_TOKEN_DECL(Static);
									FLUFFY_IS_TOKEN_DECL(Const);
									FLUFFY_IS_TOKEN_DECL(True);
									FLUFFY_IS_TOKEN_DECL(False);
									FLUFFY_IS_TOKEN_DECL(Void);
									FLUFFY_IS_TOKEN_DECL(Bool);
									FLUFFY_IS_TOKEN_DECL(I8);
									FLUFFY_IS_TOKEN_DECL(U8);
									FLUFFY_IS_TOKEN_DECL(I16);
									FLUFFY_IS_TOKEN_DECL(U16);
									FLUFFY_IS_TOKEN_DECL(I32);
									FLUFFY_IS_TOKEN_DECL(U32);
									FLUFFY_IS_TOKEN_DECL(I64);
									FLUFFY_IS_TOKEN_DECL(U64);
									FLUFFY_IS_TOKEN_DECL(Fp32);
									FLUFFY_IS_TOKEN_DECL(Fp64);
									FLUFFY_IS_TOKEN_DECL(String);
									FLUFFY_IS_TOKEN_DECL(Vector);
									FLUFFY_IS_TOKEN_DECL(Set);
									FLUFFY_IS_TOKEN_DECL(Map);
									FLUFFY_IS_TOKEN_DECL(Object);
									FLUFFY_IS_TOKEN_DECL(Fn);
									FLUFFY_IS_TOKEN_DECL(Let);
									FLUFFY_IS_TOKEN_DECL(This);
									FLUFFY_IS_TOKEN_DECL(Ref);
									FLUFFY_IS_TOKEN_DECL(Self);
									FLUFFY_IS_TOKEN_DECL(SizeOf);
									FLUFFY_IS_TOKEN_DECL(New);
									FLUFFY_IS_TOKEN_DECL(As);
									FLUFFY_IS_TOKEN_DECL(Is);
									FLUFFY_IS_TOKEN_DECL(Goto);
									FLUFFY_IS_TOKEN_DECL(If);
									FLUFFY_IS_TOKEN_DECL(Else);
									FLUFFY_IS_TOKEN_DECL(Continue);
									FLUFFY_IS_TOKEN_DECL(Break);
									FLUFFY_IS_TOKEN_DECL(For);
									FLUFFY_IS_TOKEN_DECL(Foreach);
									FLUFFY_IS_TOKEN_DECL(In);
									FLUFFY_IS_TOKEN_DECL(While);
									FLUFFY_IS_TOKEN_DECL(Do);
									FLUFFY_IS_TOKEN_DECL(Try);
									FLUFFY_IS_TOKEN_DECL(Catch);
									FLUFFY_IS_TOKEN_DECL(Finally);
									FLUFFY_IS_TOKEN_DECL(Panic);
									FLUFFY_IS_TOKEN_DECL(Return);
									FLUFFY_IS_TOKEN_DECL(Null);

									FLUFFY_IS_TOKEN_DECL(ScopeResolution);									// ::
									FLUFFY_IS_TOKEN_DECL(Increment);										// ++
									FLUFFY_IS_TOKEN_DECL(Decrement);										// --
									FLUFFY_IS_TOKEN_DECL(Arrow);											// ->

									FLUFFY_IS_TOKEN_DECL_2(LeftParBracket, LParBracket);					// (
									FLUFFY_IS_TOKEN_DECL_2(RightParBracket, RParBracket);					// )
									FLUFFY_IS_TOKEN_DECL_2(LeftSquBracket, LSquBracket);					// [
									FLUFFY_IS_TOKEN_DECL_2(RightSquBracket, RSquBracket);					// ]
									FLUFFY_IS_TOKEN_DECL_2(LeftBracket, LBracket);							// {
									FLUFFY_IS_TOKEN_DECL_2(RightBracket, RBracket);							// }

									FLUFFY_IS_TOKEN_DECL(Plus);												// +
									FLUFFY_IS_TOKEN_DECL(Minus);											// -
									FLUFFY_IS_TOKEN_DECL(Division);											// /
									FLUFFY_IS_TOKEN_DECL(Multiplication);									// *
									FLUFFY_IS_TOKEN_DECL(Modulo);											// %

									FLUFFY_IS_TOKEN_DECL(Assign);											// <

									FLUFFY_IS_TOKEN_DECL(PlusAssign);										// +=
									FLUFFY_IS_TOKEN_DECL(MinusAssign);										// -=
									FLUFFY_IS_TOKEN_DECL(MultAssign);										// /=
									FLUFFY_IS_TOKEN_DECL(DivAssign);										// *=
									FLUFFY_IS_TOKEN_DECL(BitWiseAndAssign);									// &=
									FLUFFY_IS_TOKEN_DECL(BitWiseOrAssign);									// |=
									FLUFFY_IS_TOKEN_DECL(BitWiseXorAssign);									// ^=
									FLUFFY_IS_TOKEN_DECL(ModAssign);										// %=
									FLUFFY_IS_TOKEN_DECL_2(BitWiseLeftShiftAssign, BitWiseLShiftAssign);	// <<=
									FLUFFY_IS_TOKEN_DECL_2(BitWiseRightShiftAssign, BitWiseRShiftAssign);	// >>=

									FLUFFY_IS_TOKEN_DECL(GreaterThan);										// >
									FLUFFY_IS_TOKEN_DECL(LessThan);											// <
									FLUFFY_IS_TOKEN_DECL(GreaterThanOrEqual);								// >=
									FLUFFY_IS_TOKEN_DECL(LessThanOrEqual);									// <=
									FLUFFY_IS_TOKEN_DECL(Equal);											// ==
									FLUFFY_IS_TOKEN_DECL(NotEqual);											// !=
																											
									FLUFFY_IS_TOKEN_DECL_2(BitWiseLeftShift, BitWiseLShift);				// <<
									FLUFFY_IS_TOKEN_DECL_2(BitWiseRightShift, BitWiseRShift);				// >>
									FLUFFY_IS_TOKEN_DECL(BitWiseAnd);										// &
									FLUFFY_IS_TOKEN_DECL(BitWiseOr);										// |
									FLUFFY_IS_TOKEN_DECL(BitWiseXor);										// ^
									FLUFFY_IS_TOKEN_DECL(BitWiseNot);										// ~
																											
									FLUFFY_IS_TOKEN_DECL(Colon);											// :
									FLUFFY_IS_TOKEN_DECL(SemiColon);										// ;
																											
									FLUFFY_IS_TOKEN_DECL(LogicalAnd);										// &&
									FLUFFY_IS_TOKEN_DECL(LogicalOr);										// ||
									FLUFFY_IS_TOKEN_DECL(LogicalNot);										// !
																											
									FLUFFY_IS_TOKEN_DECL(Interrogation);									// ?
																											
									FLUFFY_IS_TOKEN_DECL(Comma);											// ,
									FLUFFY_IS_TOKEN_DECL(Dot);												// .
	};
} }

#undef FLUFFY_IS_TOKEN_DECL
#undef FLUFFY_IS_TOKEN_DECL_2