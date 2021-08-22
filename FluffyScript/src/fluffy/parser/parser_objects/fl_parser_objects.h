#pragma once
#include "..\..\fl_defs.h"
#include "..\fl_parser.h"
	
namespace fluffy { namespace ast { namespace expr {
	class ExpressionDecl;
} } }

namespace fluffy { namespace ast { namespace pattern {
	class PatternDecl;
} } }

namespace fluffy { namespace ast { namespace stmt {
	class StmtDecl;
} } }

namespace fluffy { namespace parser_objects { 
	using std::unique_ptr;
	using std::vector;

	using fluffy::parser::Parser;

	using StringList						= vector<String>;

	using CodeUnitPtr						= unique_ptr<ast::CodeUnit>;
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

	using PatternDeclPtr					= unique_ptr<ast::pattern::PatternDecl>;

	using BlockDeclPtr						= unique_ptr<ast::BlockDecl>;

	using StmtDeclPtr						= unique_ptr<ast::stmt::StmtDecl>;

	using ScopedIdentifierDeclPtr			= unique_ptr<ast::ScopedIdentifierDecl>;
	using ScopedIdentifierDeclPtrList		= vector<ast::ScopedIdentifierDeclPtr>;
	
	/**
	 * ParserObjectCodeUnit
	 */

	class ParserObjectCodeUnit
	{
	public:
		static
		CodeUnitPtr							parse(Parser* parser);
	};

	/**
	 * ParserObjectIncludeDecl
	 */

	class ParserObjectIncludeDecl
	{
	public:
		static
		IncludeDeclPtr						parse(Parser* parser);
	};

	/**
	 * ParserObjectNamespace
	 */

	class ParserObjectNamespace
	{
	public:
		static
		NamespaceDeclPtr					parse(Parser* parser);
	};

	/**
	 * ParserObjectGeneralDecl
	 */

	class ParserObjectGeneralDecl
	{
	public:
		static
		GeneralStmtPtr						parse(Parser* parser);
	};

	/**
	 * ParserObjectClassDecl
	 */

	class ParserObjectClassDecl
	{
	public:
		static
		ClassDeclPtr						parse(Parser* parser, Bool hasExport, Bool hasAbstract);

	private:
		static
		TypeDeclPtr							parseExtends(Parser* parser);

		static
		TypeDeclPtrList						parseImplements(Parser* parser);

		static
		ClassFunctionDeclPtr				parseStaticFunction(Parser* parser, TokenType_e accessModifier);

		static
		ClassFunctionDeclPtr				parseFunction(Parser* parser, TokenType_e accessModifier);

		static
		ClassVariableDeclPtr				parseVariable(Parser* parser, TokenType_e accessModifier, Bool isStatic);

		static
		ClassConstructorDeclPtr				parseConstructor(Parser* parser, TokenType_e accessModifier);

		static
		ClassDestructorDeclPtr				parseDestructor(Parser* parser);
	};

	/**
	 * ParserObjectInterfaceDecl
	 */

	class ParserObjectInterfaceDecl
	{
	public:
		static
		InterfaceDeclPtr					parse(Parser* parser, Bool hasExport);

	private:
		static
		InterfaceFunctionDeclPtr			parserFunction(Parser* parser);
	};

	/**
	 * ParserObjectStructDecl
	 */

	class ParserObjectStructDecl
	{
	public:
		static
		StructDeclPtr						parse(Parser* parser, Bool hasExport);

	private:
		static
		StructVariableDeclPtr				parserVariable(Parser* parser);
	};

	/**
	 * ParserObjectEnumDecl
	 */

	class ParserObjectEnumDecl
	{
	public:
		static
		EnumDeclPtr							parse(Parser* parser, Bool hasExport);

	private:
		static
		EnumItemDeclPtr						parserEnum(Parser* parser);
	};

	/**
	 * ParserObjectTraitDecl
	 */

	class ParserObjectTraitDecl
	{
	public:
		static
		TraitDeclPtr						parse(Parser* parser, Bool hasExport);

	private:
		static
		TraitFunctionDeclPtr				parserFunction(Parser* parser, Bool isDefinition, Bool isStatic);
	};

	/**
	 * ParserObjectFunctionDecl
	 */

	class ParserObjectFunctionDecl
	{
	public:
		static
		FunctionDeclPtr						parse(Parser* parser, Bool hasExport);
	};

	/**
	 * ParserObjectVariableDecl
	 */

	class ParserObjectVariableDecl
	{
	public:
		static
		VariableDeclPtr						parse(Parser* parser, Bool hasExport);
	};

	/**
	 * ParserObjectFunctionParameter
	 */

	class ParserObjectFunctionParameter
	{
	public:
		static
		FunctionParameterDeclPtrList		parse(Parser* parser, Bool skipOnly);

		static
		FunctionParameterDeclPtrList		parseWithSelf(Parser* parser, Bool skipOnly);

	public:
		static
		FunctionParameterDeclPtrList		parseParameters(Parser* parser, Bool traitMode, Bool skipOnly);

	};

	/**
	 * ParserObjectGenericDecl
	 */

	class ParserObjectGenericDecl
	{
	public:
		static
		GenericDeclPtrList					parse(Parser* parser);
	};

	/**
	 * ParserObjectTypeDecl
	 */

	class ParserObjectTypeDecl
	{
	public:
		static
		TypeDeclPtr							parse(Parser* parser);

		static
		TypeDeclPtr							skip(Parser* parser);

		static
		TypeDeclPtr							parseWithSelf(Parser* parser);

	private:
		static
		TypeDeclPtr							parseType(Parser* parser, Bool traitMode, Bool skipOnly);

		static
		TypeDeclPtr							parseFunctionType(Parser* parser, Bool traitMode, Bool skipOnly);

		static
		TypeDeclPtr							parseVariableType(Parser* parser, Bool traitMode, Bool skipOnly);

		static
		TypeDeclPtr							parseTupleType(Parser* parser, Bool traitMode, Bool skipOnly);

		static
		TypeDeclPtr							parseNamedType(Parser* parser, Bool traitMode, Bool skipOnly);

		static
		ArrayDeclPtr						parseArrayDecl(Parser* parser, Bool traitMode, Bool skipOnly);

		static
		TypeDeclNamedPtr					parseInternalNamedType(Parser* parser, Bool traitMode, Bool skipOnly);

		static
		void								reinterpretToken(Parser* parser);

	};

	/**
	 * ParserObjectExpressionDecl
	 */

	class ParserObjectExpressionDecl
	{
	public:
		static
		ExpressionDeclPtr					parse(Parser* parser, U32 prec, Bool skipOnly);

	private:
		static
		ExpressionDeclPtr					parseExpression(Parser* parser, U32 prec, Bool skipOnly);

		static
		ExpressionDeclPtr					parseAtom(Parser* parser, Bool skipOnly);
	};

	/**
	 * ParserObjectPatternDecl
	 */

	class ParserObjectPatternDecl
	{
	public:
		static
		PatternDeclPtr						parse(Parser* parser, Bool skipOnly);

	private:
		static
		PatternDeclPtr						parsePattern(Parser* parser, Bool skipOnly);

		static
		PatternDeclPtr						parseLiteralPattern(Parser* parser, Bool skipOnly);

		static
		PatternDeclPtr						parseEnumPattern(Parser* parser, Bool skipOnly);

		static
		PatternDeclPtr						parseTuplePattern(Parser* parser, Bool skipOnly);

		static
		PatternDeclPtr						parseClassOrStructPattern(Parser* parser, Bool skipOnly);
	};

	/**
	 * ParserObjectBlockDecl
	 */

	class ParserObjectBlockDecl
	{
	public:
		static
		BlockDeclPtr						parse(Parser* parser, Bool skipOnly);

	};

	/**
	 * ParserObjectStmtDecl
	 */

	class ParserObjectStmtDecl
	{
	public:
		static
		StmtDeclPtr							parse(Parser* parser, Bool skipOnly);

	private:
		static
		StmtDeclPtr							parseStmt(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseIf(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseIfLet(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseFor(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseWhile(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseDoWhile(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseMatch(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseReturn(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseContinue(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseBreak(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseGoto(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseLabel(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseTry(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parsePanic(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseVariable(Parser* parser, Bool skipOnly);

		static
		StmtDeclPtr							parseExpr(Parser* parser, Bool skipOnly);
	};

	/**
	 * ParserObjectScopedIdentifier
	 */

	class ParserObjectScopedIdentifier
	{
	public:
		static
		ScopedIdentifierDeclPtr				parse(Parser* parser);

	private:
		static
		ScopedIdentifierDeclPtr				parseChildScopedIdentifiers(Parser* parser);
	};
} }