#pragma once
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
	typedef double					Fp64;

	typedef void					Void;
	typedef void* Pointer;

	typedef std::string				String;

	/**
	 * TokeyType
	 * Define o tipo de token.
	 */

	enum class TokenType_e
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

	enum class TokenSubType_e
	{
		eTST_Unknown,

		/**
		 * Keyword
		 */

		 eTST_Include,			// ok
		 eTST_From,				// ok
		 eTST_Export,			// ok
		 eTST_Namespace,		// ok
		 eTST_Class,			// ok
		 eTST_Extends,			// ok
		 eTST_Implements,		// ok
		 eTST_Constructor,		// ok
		 eTST_Destructor,		// ok
		 eTST_Super,			// ok
		 eTST_Public,			// ok
		 eTST_Protected,		// ok
		 eTST_Private,			// ok
		 eTST_Final,			// ok
		 eTST_Override,			// ok
		 eTST_Abstract,			// ok
		 eTST_Virtual,			// ok
		 eTST_Interface,		// ok
		 eTST_Struct,			// ok
		 eTST_Enum,				// ok
		 eTST_Match,			// ok
		 eTST_When,				// ok
		 eTST_Trait,			// ok
		 eTST_Static,			// ok
		 eTST_Const,			// ok
		 eTST_True,				// ok
		 eTST_False,			// ok
		 eTST_Void,				// ok
		 eTST_Bool,				// ok
		 eTST_I8,				// ok
		 eTST_U8,				// ok
		 eTST_I16,				// ok
		 eTST_U16,				// ok
		 eTST_I32,				// ok
		 eTST_U32,				// ok
		 eTST_I64,				// ok
		 eTST_U64,				// ok
		 eTST_Fp32,				// ok
		 eTST_Fp64,				// ok
		 eTST_String,			// ok
		 eTST_Map,				// ok
		 eTST_Object,			// ok
		 eTST_Fn,				// ok
		 eTST_Let,				// ok
		 eTST_This,				// ok
		 eTST_Self,				// ok
		 eTST_SizeOf,			// ok
		 eTST_New,				// ok
		 eTST_As,				// ok
		 eTST_Is,				// ok
		 eTST_Goto,				// ok
		 eTST_If,				// ok
		 eTST_Else,				// ok
		 eTST_Continue,			// ok
		 eTST_Break,			// ok
		 eTST_For,				// ok
		 eTST_Foreach,			// ok
		 eTST_In,				// ok
		 eTST_While,			// ok
		 eTST_Do,				// ok
		 eTST_Try,				// ok
		 eTST_Catch,			// ok
		 eTST_Finally,			// ok
		 eTST_Panic,			// ok
		 eTST_Return,			// ok
		 eTST_Null,				// ok

		 /**
		  * Symbols
		  */

		  eTST_ScopeResolution, 		// ::
		  eTST_Increment,				// ++
		  eTST_Decrement,				// --
		  eTST_Arrow,					// ->

		  eTST_LParBracket,				// (
		  eTST_RParBracket,				// )
		  eTST_LSquBracket,				// [
		  eTST_RSquBracket,				// ]
		  eTST_LBracket,				// {
		  eTST_RBracket,				// }

		  eTST_Plus,					// +
		  eTST_Minus,					// -
		  eTST_Division,				// /
		  eTST_Multiplication,			// *
		  eTST_Modulo,					// %

		  eTST_Assign,					// =

		  eTST_PlusAssign,				// +=
		  eTST_MinusAssign,				// -=
		  eTST_DivAssign,				// /=
		  eTST_MultAssign,				// *=
		  eTST_BitWiseAndAssign,		// &=
		  eTST_BitWiseOrAssign,			// |=
		  eTST_BitWiseXorAssign,		// ^=
		  eTST_ModAssign,				// %=
		  eTST_BitWiseLShiftAssign,		// <<=
		  eTST_BitWiseRShiftAssign,		// >>=

		  eTST_GreaterThan,				// >
		  eTST_LessThan,				// <
		  eTST_GreaterThanOrEqual,		// >=
		  eTST_LessThanOrEqual,			// <=
		  eTST_Equal,					// ==
		  eTST_NotEqual,				// !=

		  eTST_BitWiseLShift,			// <<
		  eTST_BitWiseRShift,			// >>
		  eTST_BitWiseAnd,				// &
		  eTST_BitWiseOr,				// |
		  eTST_BitWiseXor,				// ^
		  eTST_BitWiseNot,				// ~

		  eTST_Colon,					// :
		  eTST_SemiColon,				// ;

		  eTST_LogicalAnd,				// &&
		  eTST_LogicalOr,				// ||
		  eTST_LogicalNot,				// !

		  eTST_Interrogation,			// ?

		  eTST_Comma,					// ,
		  eTST_Dot,						// .

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