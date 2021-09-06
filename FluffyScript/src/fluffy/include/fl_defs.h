#pragma once
#include <string>
#include <vector>

namespace fluffy { namespace parser {
	class Parser;
	class AstNodeIdentified;
} }

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
	 * Tokentype
	 * Define a especializacao do tipo.
	 */

	enum class TokenType_e
	{
		Unknown,

		Identifier,

		/**
		 * Keyword
		 */

		Include,			// ok
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
		Object,				// ok
		Fn,					// ok
		Let,				// ok
		This,				// ok
		Shared,				// ok
		Unique,				// ok
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
		SafeDot,					// ?.

		Ellipsis,					// ...

		/**
		 * Constant
		 */

		ConstantInteger,
		ConstantFp32,
		ConstantFp64,
		ConstantBin,
		ConstantHex,

		ConstantChar,
		ConstantString,

		Eof
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
		GenericDecl,
		Match,
		FunctionDecl,
		FunctionCall,
		Index,
		New,
		This,
		Super,
		AnomClassDecl,
		PrimitiveType,
		ConstantBool,
		ConstantInteger,
		ConstantReal,
		ConstantString,
		ConstantChar,
		ConstantNull,
		ConstantIdentifier,
		Mark,
		ArrayInit
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
	 * DestructuringType_e
	 */

	enum class DestructuringType_e
	{
		Unknown,
		ClassOrStruct,
		Tuple,
		Enum
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

		Function,

		Tuple,

		Self,			// Apenas em traits.

		Named			// Classes, interfaces, trait, enum...
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
		Unary			= 13,
		Max				= 15
	};

	/**
	 * StmtDeclType_e
	 */

	enum class StmtDeclType_e
	{
		If,
		IfLet,
		For,
		While,
		DoWhile,
		Match,
		Return,
		Continue,
		Break,
		Goto,
		Label,
		Try,
		Panic,
		Variable,
		Expr
	};

	/**
	 * InferenceType_e
	 */

	enum class InferenceType_e {
		None,

		OnlyParams,
		OnlyReturn,

		Full
	};

	/**
	 * DestructuringItemType_e
	 */

	enum class DestructuringItemType_e
	{
		Unknown,

		DirectReference,			// Faz referencia a uma variavel, ex: { t }.
		IndirectReferenceOrMatch,	// Faz referencia a uma variavel e redireciona para outro identifiador ou e matching, ex: { t: id }.
		SubDestructuring			// Faz referencia ao destructuring de uma variavel, ex: { t: { } }
	};

	/**
	 * AstNodeType_e
	 */

	enum class AstNodeType_e
	{
		Unknown,

		CodeUnit,

		Include,
		Namespace,
		Class,
		ClassFunction,
		ClassVariable,
		ClassConstructor,
		ClassDestructor,
		ClassVariableInitDecl,
		Interface,
		InterfaceFunction,
		Struct,
		StructVariable,
		Trait,
		TraitFunction,
		Enum,
		EnumItem,
		Function,
		Variable,
		FunctionParameter,
		Generic,
		GenericItem,
		ScopedIdentifier,

		Block,

		TernaryExpr,
		BinaryExpr,
		UnaryExpr,
		AsExpr,
		IsExpr,
		MatchExpr,
		FunctionDeclExpr,
		FunctionCallExpr,
		GenericCallExpr,
		IndexExpr,
		NewExpr,
		ConstantBoolExpr,
		ConstantIntegerExpr,
		ConstantRealExpr,
		ConstantStringExpr,
		ConstantCharExpr,
		ConstantNullExpr,
		ThisExpr,
		SuperExpr,
		AnomClassDeclExpr,
		PrimitiveTypeExpr,
		IdentifierExpr,
		ArrayInitExpr,

		MatchWhenExpr,
		ExprFunctionParameterDecl,
		NewBlockExpr,
		NewItemExpr,

		LiteralPattern,
		TuplePattern,
		StructurePattern,
		EnumerablePattern,
		StructureItemPatternDecl,

		IfStmt,
		IfLetStmt,
		ForStmt,
		WhileStmt,
		DoWhileStmt,
		MatchStmt,
		ReturnStmt,
		ContinueStmt,
		BreakStmt,
		GotoStmt,
		LabelStmt,
		TryStmt,
		PanicStmt,
		VariableStmt,
		ExprStmt,

		ForInitDecl,
		MatchWhenStmtDecl,
		CatchBlockStmtDecl,

		VoidType,
		BoolType,
		I8Type,
		U8Type,
		I16Type,
		U16Type,
		I32Type,
		U32Type,
		I64Type,
		U64Type,
		Fp32Type,
		Fp64Type,
		StringType,
		ObjectType,
		ArrayType,
		FunctionType,
		NamedType,
		SelfType,
		TupleType,

		SizedArray,
		UnsizedArray
	};


	/**
	 * Token
	 * Estrutura basica de informacao do compilador.
	 */

	struct Token_s
	{
		String			value;

		TokenType_e	type;

		U32				line;
		U32				column;
		U32				position;

		String			filename;
	};

	/**
	 * Funcoes auxiliares
	 */

	const I8*			getTokenString(const TokenType_e tokentype);
}