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
		Unknown,

		Identifier,
		Keyword,
		Symbol,
		Constant,
		Eof
	};

	/**
	 * TokenSubType
	 * Define a especializacao do tipo.
	 */

	enum class TokenSubType_e
	{
		Unknown,

		/**
		 * Keyword
		 */

		 Include,			// ok
		 From,				// ok
		 Export,			// ok
		 Namespace,			// ok
		 Class,				// ok
		 Extends,			// ok
		 Implements,		// ok
		 Where,				// ok
		 Constructor,		// ok
		 Destructor,		// ok
		 Super,				// ok
		 Public,			// ok
		 Protected,			// ok
		 Private,			// ok
		 Final,				// ok
		 Override,			// ok
		 Abstract,			// ok
		 Virtual,			// ok
		 Interface,			// ok
		 Struct,			// ok
		 Enum,				// ok
		 Match,				// ok
		 When,				// ok
		 Trait,				// ok
		 Static,			// ok
		 Const,				// ok
		 True,				// ok
		 False,				// ok
		 Void,				// ok
		 Bool,				// ok
		 I8,				// ok
		 U8,				// ok
		 I16,				// ok
		 U16,				// ok
		 I32,				// ok
		 U32,				// ok
		 I64,				// ok
		 U64,				// ok
		 Fp32,				// ok
		 Fp64,				// ok
		 String,			// ok
		 Vector,			// ok
		 Set,				// ok
		 Map,				// ok
		 Object,			// ok
		 Fn,				// ok
		 Let,				// ok
		 This,				// ok
		 Ref,				// ok
		 Self,				// ok
		 New,				// ok
		 As,				// ok
		 Is,				// ok
		 Goto,				// ok
		 If,				// ok
		 Else,				// ok
		 Continue,			// ok
		 Break,				// ok
		 For,				// ok
		 Foreach,			// ok
		 In,				// ok
		 While,				// ok
		 Do,				// ok
		 Try,				// ok
		 Catch,				// ok
		 Finally,			// ok
		 Panic,				// ok
		 Return,			// ok
		 Null,				// ok

		 /**
		  * Symbols
		  */

		  ScopeResolution, 			// ::
		  Increment,				// ++
		  Decrement,				// --
		  Arrow,					// ->

		  LParBracket,				// (
		  RParBracket,				// )
		  LSquBracket,				// [
		  RSquBracket,				// ]
		  LBracket,					// {
		  RBracket,					// }

		  Plus,						// +
		  Minus,						// -
		  Division,					// /
		  Multiplication,			// *
		  Modulo,					// %

		  Assign,					// =

		  PlusAssign,				// +=
		  MinusAssign,				// -=
		  DivAssign,				// /=
		  MultAssign,				// *=
		  BitWiseAndAssign,			// &=
		  BitWiseOrAssign,			// |=
		  BitWiseXorAssign,			// ^=
		  ModAssign,				// %=
		  BitWiseLShiftAssign,		// <<=
		  BitWiseRShiftAssign,		// >>=

		  GreaterThan,				// >
		  LessThan,					// <
		  GreaterThanOrEqual,		// >=
		  LessThanOrEqual,			// <=
		  Equal,					// ==
		  NotEqual,					// !=

		  BitWiseLShift,			// <<
		  BitWiseRShift,			// >>
		  BitWiseAnd,				// &
		  BitWiseOr,				// |
		  BitWiseXor,				// ^
		  BitWiseNot,				// ~

		  Colon,					// :
		  SemiColon,				// ;

		  LogicalAnd,				// &&
		  LogicalOr,				// ||
		  LogicalNot,				// !

		  Interrogation,			// ?

		  Comma,					// ,
		  Dot,						// .

		  /**
		   * Constant
		   */

		   ConstantI8,
		   ConstantU8,
		   ConstantI16,
		   ConstantU16,
		   ConstantI32,
		   ConstantU32,
		   ConstantI64,
		   ConstantU64,
		   ConstantFp32,
		   ConstantFp64,
		   ConstantBin,
		   ConstantHex,

		   ConstantChar,
		   ConstantString
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
		U32 position;

		String filename;
	};
}