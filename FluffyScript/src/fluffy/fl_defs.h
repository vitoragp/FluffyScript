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
		Export,				// ok
		Namespace,			// ok
		Class,				// ok
		Extends,			// ok
		Implements,			// ok
		Where,				// ok
		Constructor,		// ok
		Destructor,			// ok
		Super,				// ok
		Public,				// ok
		Protected,			// ok
		Private,			// ok
		Final,				// ok
		Override,			// ok
		Abstract,			// ok
		Virtual,			// ok
		Interface,			// ok
		Struct,				// ok
		Enum,				// ok
		Match,				// ok
		When,				// ok
		Trait,				// ok
		Static,				// ok
		Const,				// ok
		True,				// ok
		False,				// ok
		Void,				// ok
		Bool,				// ok
		I8,					// ok
		U8,					// ok
		I16,				// ok
		U16,				// ok
		I32,				// ok
		U32,				// ok
		I64,				// ok
		U64,				// ok
		Fp32,				// ok
		Fp64,				// ok
		String,				// ok
		Vector,				// ok
		Set,				// ok
		Map,				// ok
		Object,				// ok
		Fn,					// ok
		Let,				// ok
		This,				// ok
		Ref,				// ok
		Self,				// ok
		New,				// ok
		As,					// ok
		Is,					// ok
		Goto,				// ok
		If,					// ok
		Else,				// ok
		Continue,			// ok
		Break,				// ok
		For,				// ok
		Foreach,			// ok
		In,					// ok
		While,				// ok
		Do,					// ok
		Try,				// ok
		Catch,				// ok
		Finally,			// ok
		Panic,				// ok
		Return,				// ok
		Null,				// ok

		/**
		 * Symbols
		 */

		ScopeResolution, 			// ::
		Increment,					// ++
		Decrement,					// --
		Arrow,						// ->

		LParBracket,				// (
		RParBracket,				// )
		LSquBracket,				// [
		RSquBracket,				// ]
		LBracket,					// {
		RBracket,					// }

		Plus,						// +
		Minus,						// -
		Division,					// /
		Multiplication,				// *
		Modulo,						// %

		Assign,						// =

		PlusAssign,					// +=
		MinusAssign,				// -=
		DivAssign,					// /=
		MultAssign,					// *=
		BitWiseAndAssign,			// &=
		BitWiseOrAssign,			// |=
		BitWiseXorAssign,			// ^=
		ModAssign,					// %=
		BitWiseLShiftAssign,		// <<=
		BitWiseRShiftAssign,		// >>=

		GreaterThan,				// >
		LessThan,					// <
		GreaterThanOrEqual,			// >=
		LessThanOrEqual,			// <=
		Equal,						// ==
		NotEqual,					// !=

		BitWiseLShift,				// <<
		BitWiseRShift,				// >>
		BitWiseAnd,					// &
		BitWiseOr,					// |
		BitWiseXor,					// ^
		BitWiseNot,					// ~

		Colon,						// :
		SemiColon,					// ;

		LogicalAnd,					// &&
		LogicalOr,					// ||
		LogicalNot,					// !

		Interrogation,				// ?

		Comma,						// ,
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
	 * Enums
	 */

	enum class ExpressionDeclType_e
	{
		Unknown,

		Ternary,
		Binary,
		Unary,
		As,
		Is,
		Match,
		FunctionDecl,
		FunctionCall,
		Index,
		This,
		Super,
		ConstantBool,
		ConstantInteger,
		ConstantReal,
		ConstantString,
		ConstantChar,
		ConstantNull,
		ConstantIdentifier
	};

	/**
		* ExpressionUnaryType_e
		*/

	enum class ExpressionUnaryType_e
	{
		Unknown,

		Prefix,
		Posfix
	};

	/**
		* PatternType_e
		*/

	enum class PatterType_e
	{
		Unknown
	};

	/**
		 * GeneralDeclType_e
		 */

	enum class GeneralDeclType_e
	{
		Unknown,
		ClassDecl,
		InterfaceDecl,
		StructDecl,
		EnumDecl,
		TraitDecl,
		VariableDecl,
		FunctionDecl
	};

	/**
	 * ClassMemberType_e
	 */

	enum class ClassMemberType_e
	{
		Unknown,

		Constructor,
		Destructor,

		Function,
		Variable
	};

	/**
	 * ClassMemberAccessModifier_e
	 */

	enum class ClassMemberAccessModifier_e
	{
		Unknown,

		Public,
		Protected,
		Private
	};

	/**
	 * TypeDeclID_e
	 */

	enum class TypeDeclID_e
	{
		Unknown,

		Void,
		Bool,
		I8,
		U8,
		I16,
		U16,
		I32,
		U32,
		I64,
		U64,
		Fp32,
		Fp64,
		String,
		Object,

		Array,

		Vector,
		Set,
		Map,

		Function,

		Tuple,

		Named			// Classes, interfaces, trait, enum...
	};

	/**
	 * BlockType_e
	 */

	enum class BlockType_e
	{
		Default,
		Expression
	};

	/**
	 * ArrayType_e
	 */

	enum class ArrayType_e
	{
		Sized,
		Unsized
	};

	/**
	 * OperatorPrecLevel_e
	 */

	enum OperatorPrecLevel_e : U32
	{
		MinPrec			= 1,
		Interrogation	= 2,
		EnumExpr		= 3,
		Unary			= 12,
		Increment		= 14,
		Max				= 15
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

	/**
	 * Funcoes auxiliares
	 */

	const I8* getTokenString(const TokenSubType_e tokenSubType);
}