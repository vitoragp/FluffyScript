#pragma once
#include "..\..\fl_defs.h"
#include "..\fl_parser.h"
	
namespace fluffy { namespace ast { namespace expr {
	class ExpressionDecl;
	class PatternDecl;
} } }

namespace fluffy { namespace parser_objects { 
	using std::unique_ptr;
	using std::vector;

	using fluffy::parser::Parser;

	using StringList						= vector<String>;

	using ProgramPtr						= unique_ptr<ast::Program>;
	using IncludeDeclPtr					= unique_ptr<ast::IncludeDecl>;
	using NamespaceDeclPtr					= unique_ptr<ast::NamespaceDecl>;
	using GeneralStmtPtr					= unique_ptr<ast::GeneralDecl>;
	using ClassDeclPtr						= unique_ptr<ast::ClassDecl>;
	using ClassFunctionDeclPtr				= unique_ptr<ast::ClassFunctionDecl>;
	using ClassVariableDeclPtr				= unique_ptr<ast::ClassVariableDecl>;
	using ClassConstructorDeclPtr			= unique_ptr<ast::ClassConstructorDecl>;
	using ClassDestructorDeclPtr			= unique_ptr<ast::ClassDestructorDecl>;
	using InterfaceDeclPtr					= unique_ptr<ast::InterfaceDecl>;
	using InterfaceFunctionDeclPtr			= unique_ptr<ast::InterfaceFunctionDecl>;
	using StructDeclPtr						= unique_ptr<ast::StructDecl>;
	using StructVariableDeclPtr				= unique_ptr<ast::StructVariableDecl>;
	using EnumDeclPtr						= unique_ptr<ast::EnumDecl>;
	using EnumItemDeclPtr					= unique_ptr<ast::EnumItemDecl>;
	using TraitDeclPtr						= unique_ptr<ast::TraitDecl>;
	using TraitFunctionDeclPtr				= unique_ptr<ast::TraitFunctionDecl>;
	using FunctionDeclPtr					= unique_ptr<ast::FunctionDecl>;
	using VariableDeclPtr					= unique_ptr<ast::VariableDecl>;

	using FunctionParameterDeclPtr			= unique_ptr<ast::FunctionParameterDecl>;
	using FunctionParameterDeclPtrList		= vector<FunctionParameterDeclPtr>;

	using GenericDeclPtr					= unique_ptr<ast::GenericDecl>;
	using GenericDeclPtrList				= vector<GenericDeclPtr>;

	using TypeDeclPtr						= unique_ptr<ast::TypeDecl>;
	using TypeDeclPtrList					= vector<ast::TypeDeclPtr>;

	using TypeDeclNamedPtr					= unique_ptr<ast::TypeDeclNamed>;

	using ArrayDeclPtr						= unique_ptr<ast::ArrayDecl>;

	using ExpressionDeclPtr					= unique_ptr<ast::expr::ExpressionDecl>;

	using PatternDeclPtr					= unique_ptr<ast::expr::PatternDecl>;

	using BlockDeclPtr						= unique_ptr<ast::BlockDecl>;

	using ScopedIdentifierDeclPtr			= unique_ptr<ast::ScopedIdentifierDecl>;
	using ScopedIdentifierDeclPtrList		= vector<ast::ScopedIdentifierDeclPtr>;
	
	/**
	 * ParserObjectProgram
	 */

	class ParserObjectProgram
	{
	public:
		static
		ProgramPtr							parse(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectIncludeDecl
	 */

	class ParserObjectIncludeDecl
	{
	public:
		static
		IncludeDeclPtr						parse(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectNamespace
	 */

	class ParserObjectNamespace
	{
	public:
		static
		NamespaceDeclPtr					parse(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectGeneralDecl
	 */

	class ParserObjectGeneralDecl
	{
	public:
		static
		GeneralStmtPtr						parse(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectClassDecl
	 */

	class ParserObjectClassDecl
	{
	public:
		static
		ClassDeclPtr						parse(CompilationContext_t* ctx, Bool hasExport, Bool hasAbstract);

	private:
		static
		TypeDeclPtr							parseExtends(CompilationContext_t* ctx);

		static
		TypeDeclPtrList						parseImplements(CompilationContext_t* ctx);

		static
		ClassFunctionDeclPtr				parseStaticFunction(CompilationContext_t* ctx, TokenSubType_e accessModifier);

		static
		ClassFunctionDeclPtr				parseFunction(CompilationContext_t* ctx, TokenSubType_e accessModifier);

		static
		ClassVariableDeclPtr				parseVariable(CompilationContext_t* ctx, TokenSubType_e accessModifier, Bool isStatic);

		static
		ClassConstructorDeclPtr				parseConstructor(CompilationContext_t* ctx, TokenSubType_e accessModifier);

		static
		ClassDestructorDeclPtr				parseDestructor(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectInterfaceDecl
	 */

	class ParserObjectInterfaceDecl
	{
	public:
		static
		InterfaceDeclPtr					parse(CompilationContext_t* ctx, Bool hasExport);

	private:
		static
		InterfaceFunctionDeclPtr			parserFunction(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectStructDecl
	 */

	class ParserObjectStructDecl
	{
	public:
		static
		StructDeclPtr						parse(CompilationContext_t* ctx, Bool hasExport);

	private:
		static
		StructVariableDeclPtr				parserVariable(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectEnumDecl
	 */

	class ParserObjectEnumDecl
	{
	public:
		static
		EnumDeclPtr						parse(CompilationContext_t* ctx, Bool hasExport);

	private:
		static
		EnumItemDeclPtr					parserEnum(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectTraitDecl
	 */

	class ParserObjectTraitDecl
	{
	public:
		static
		TraitDeclPtr					parse(CompilationContext_t* ctx, Bool hasExport);

	private:
		static
		TraitFunctionDeclPtr			parserFunction(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectFunctionDecl
	 */

	class ParserObjectFunctionDecl
	{
	public:
		static
		FunctionDeclPtr					parse(CompilationContext_t* ctx, Bool hasExport);
	};

	/**
	 * ParserObjectVariableDecl
	 */

	class ParserObjectVariableDecl
	{
	public:
		static
		VariableDeclPtr					parse(CompilationContext_t* ctx, Bool hasExport);
	};

	/**
	 * ParserObjectFunctionParameter
	 */

	class ParserObjectFunctionParameter
	{
	public:
		static
		FunctionParameterDeclPtrList		parse(CompilationContext_t* ctx);

		static
		FunctionParameterDeclPtrList		parseWithSelf(CompilationContext_t* ctx);

	public:
		static
		FunctionParameterDeclPtrList		parseParameters(CompilationContext_t* ctx, Bool traitMode);
	};

	/**
	 * ParserObjectGenericDecl
	 */

	class ParserObjectGenericDecl
	{
	public:
		static
		GenericDeclPtrList					parse(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectTypeDecl
	 */

	class ParserObjectTypeDecl
	{
	public:
		static
		TypeDeclPtr							parse(CompilationContext_t* ctx);

		static
		TypeDeclPtr							parseWithSelf(CompilationContext_t* ctx);

	private:
		static
		TypeDeclPtr							parseType(CompilationContext_t* ctx, Bool traitMode);

		static 
		TypeDeclPtr							parseVectorType(CompilationContext_t* ctx, Bool traitMode);

		static
		TypeDeclPtr							parseSetType(CompilationContext_t* ctx, Bool traitMode);

		static
		TypeDeclPtr							parseMapType(CompilationContext_t* ctx, Bool traitMode);

		static
		TypeDeclPtr							parseFunctionType(CompilationContext_t* ctx, Bool traitMode);

		static
		TypeDeclPtr							parseVariableType(CompilationContext_t* ctx, Bool traitMode);

		static
		TypeDeclPtr							parseTupleType(CompilationContext_t* ctx, Bool traitMode);

		static
		TypeDeclPtr							parseNamedType(CompilationContext_t* ctx, Bool traitMode);

		static
		ArrayDeclPtr						parseArrayDecl(CompilationContext_t* ctx, Bool traitMode);

		static
		TypeDeclNamedPtr					parseInternalNamedType(CompilationContext_t* ctx, Bool traitMode);

		static
		void								reinterpretToken(CompilationContext_t* ctx);

	};

	/**
	 * ParserObjectExpressionDecl
	 */

	class ParserObjectExpressionDecl
	{
	public:
		static
		ExpressionDeclPtr					parse(CompilationContext_t* ctx);

		static
		ExpressionDeclPtr					skip(CompilationContext_t* ctx);

		static
		ExpressionDeclPtr					parseEnumExpr(CompilationContext_t* ctx);

		static
		ExpressionDeclPtr					skipEnumExpr(CompilationContext_t* ctx);

	private:
		static
		ExpressionDeclPtr					parseExpression(CompilationContext_t* ctx, U32 prec, Bool skipOnly);

		static
		ExpressionDeclPtr					parseAtom(CompilationContext_t* ctx, Bool skipOnly);
	};

	/**
	 * ParserObjectPatternDecl
	 */

	class ParserObjectPatternDecl
	{
	public:
		static
		PatternDeclPtr						parse(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectBlockDecl
	 */

	class ParserObjectBlockDecl
	{
	public:
		static
		BlockDeclPtr						parse(CompilationContext_t* ctx);

		static
		BlockDeclPtr						skip(CompilationContext_t* ctx);

		static
		BlockDeclPtr						parseExprBlock(CompilationContext_t* ctx);

		static
		BlockDeclPtr						skipExprBlock(CompilationContext_t* ctx);

		static
		BlockDeclPtr						parseBlockOrExprBlock(CompilationContext_t* ctx);

		static
		BlockDeclPtr						skipBlockOrExprBlock(CompilationContext_t* ctx);
	};

	/**
	 * ParserObjectScopedIdentifier
	 */

	class ParserObjectScopedIdentifier
	{
	public:
		static
		ScopedIdentifierDeclPtr				parse(CompilationContext_t* ctx);

	private:
		static
		ScopedIdentifierDeclPtr				parseChildScopedIdentifiers(CompilationContext_t* ctx);
	};
} }