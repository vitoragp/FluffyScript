/*
 * fl_defs.h
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#ifndef SRC_FLUFFY_FL_DEFS_H_
#define SRC_FLUFFY_FL_DEFS_H_
#include <string>

namespace fluffy {

	/**
	 * Tipos primitivos
	 */

	typedef bool					Bool;

	typedef char					I8;
	typedef unsigned char			U8;

	typedef short					I16;
	typedef unsigned short			U16;

	typedef int						I32;
	typedef unsigned int			U32;

	typedef long long				I64;
	typedef unsigned long long		U64;

	typedef float					Fp32;
	typedef float					Fp64;

	typedef void					Void;
	typedef void*					Pointer;

	typedef std::string				String;

	/**
	 * TokeyType
	 * Define o tipo de token.
	 */

	enum TokenType_e
	{
		eTT_Unknown,

		eTT_Identifier,
		eTT_Keyword,
		eTT_Symbol,
		eTT_Constant,
		eTT_EOF,

		eTT_TypeCount
	};

	/**
	 * TokenSubType
	 * Define a especializacao do tipo.
	 */

	enum TokenSubType_e
	{
		eTST_Unknown,

		/**
		 * Keyword
		 */

		eTST_Include,
		eTST_From,
		eTST_Namespace,
		eTST_Class,
		eTST_Extends,
		eTST_Implements,
		eTST_Constructor,
		eTST_Destructor,
		eTST_Super,
		eTST_Public,
		eTST_Protected,
		eTST_Private,
		eTST_Final,
		eTST_Override,
		eTST_Abstract,
		eTST_Virtual,
		eTST_Interface,
		eTST_Struct,
		eTST_Enum,
		eTST_Trait,
		eTST_Static,
		eTST_Const,
		eTST_True,
		eTST_False,
		eTST_Void,
		eTST_Bool,
		eTST_i8,
		eTST_u8,
		eTST_i16,
		eTST_u16,
		eTST_i32,
		eTST_u32,
		eTST_i64,
		eTST_u64,
		eTST_fp32,
		eTST_fp64,
		eTST_String,
		eTST_Map,
		eTST_Object,
		eTST_Fn,
		eTST_Let,
		eTST_This,
		eTST_Self,
		eTST_SizeOf,
		eTST_New,
		eTST_As,
		eTST_Is,
		eTST_Goto,
		eTST_If,
		eTST_Else,
		eTST_Switch,
		eTST_Case,
		eTST_Default,
		eTST_Continue,
		eTST_Break,
		eTST_For,
		eTST_Foreach,
		eTST_In,
		eTST_While,
		eTST_Do,
		eTST_Try,
		eTST_Catch,
		eTST_Finally,
		eTST_Panic,
		eTST_Return,
		eTST_Null,

		/**
		 * Symbols
		 */

		eTST_ScopeResolution, 		// ::
		eTST_Increment,				// ++
		eTST_Decrement,				// --
		eTST_ReturnSet,				// ->

		eTST_LParBracket,			// (
		eTST_RParBracket,			// )
		eTST_LSquBracket,			// [
		eTST_RSquBracket,			// ]
		eTST_LBracket,				// {
		eTST_RBracket,				// }

		eTST_Plus,					// +
		eTST_Minus,					// -
		eTST_Division,				// /
		eTST_Multiplication,		// *
		eTST_Modulo,				// %

		eTST_Assign,				// =

		eTST_PlusAssign,			// +=
		eTST_MinusAssign,			// -=
		eTST_MultAssign,			// *=
		eTST_DivAssign,				// /=
		eTST_BitWiseAndAssign,		// &=
		eTST_BitWiseOrAssign,		// |=
		eTST_BitWiseXorAssign,		// ^=
		eTST_ModAssign,				// %=
		eTST_BitWiseLShiftAssign,	// <<=
		eTST_BitWiseRShiftAssign,	// >>=

		eTST_GreaterThan,			// >
		eTST_LessThan,				// <
		eTST_GreaterThanOrEqual,	// >=
		eTST_LessThanOrEqual,		// <=
		eTST_Equal,					// ==
		eTST_NotEqual,				// !=

		eTST_BitWiseLShift,			// <<
		eTST_BitWiseRShift,			// >>
		eTST_BitWiseAnd,			// &
		eTST_BitWiseOr,				// |
		eTST_BitWiseXor,			// ^
		eTST_BitWiseNot,			// ~

		eTST_Colon,					// :
		eTST_SemiColon,				// ;

		eTST_LogicalAnd,			// &&
		eTST_LogicalOr,				// ||
		eTST_LogicalNot,			// !

		eTST_Interrogation,			// ?

		eTST_Comma,					// ,
		eTST_Dot,					// .

		/**
		 * Constant
		 */

		eTST_ConstantI8,
		eTST_ConstantU8,
		eTST_ConstantI16,
		eTST_ConstantU16,
		eTST_ConstantI32,
		eTST_ConstantU32,
		eTST_ConstantI64,
		eTST_ConstantU64,
		eTST_ConstantFp32,
		eTST_ConstantFp64,
		eTST_ConstantBin,
		eTST_ConstantHex,

		eTST_ConstantChar,
		eTST_ConstantString,

		eTST_SubTypeCount
	};

	/**
	 * Token
	 * Estrutura basica de informacao do compilador.
	 */

	struct Token_s
	{
		String value;

		TokenType_e type;
		TokenSubType_e subType;

		U32 line;
		U32 column;

		String filename;
	};
}
#endif /* SRC_FLUFFY_FL_DEFS_H_ */
