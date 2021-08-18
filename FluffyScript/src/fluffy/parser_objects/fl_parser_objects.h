#pragma once
#include "../fl_defs.h"
#include "../fl_parser.h"
	
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
		ProgramPtr							parse(Parser* parser);
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
		ClassFunctionDeclPtr				parseStaticFunction(Parser* parser, TokenSubType_e accessModifier);

		static
		ClassFunctionDeclPtr				parseFunction(Parser* parser, TokenSubType_e accessModifier);

		static
		ClassVariableDeclPtr				parseVariable(Parser* parser, TokenSubType_e accessModifier, Bool isStatic);

		static
		ClassConstructorDeclPtr				parseConstructor(Parser* parser, TokenSubType_e accessModifier);

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
		EnumDeclPtr						parse(Parser* parser, Bool hasExport);

	private:
		static
		EnumItemDeclPtr					parserEnum(Parser* parser);
	};

	/**
	 * ParserObjectFunctionParameter
	 */

	class ParserObjectFunctionParameter
	{
	public:
		static
		FunctionParameterDeclPtrList		parse(Parser* parser);
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
		TypeDeclPtr							parseOnlyNamedType(Parser* parser);

	private:
		static 
		TypeDeclPtr							parseVectorType(Parser* parser);

		static 
		TypeDeclPtr							parseSetType(Parser* parser);

		static 
		TypeDeclPtr							parseMapType(Parser* parser);

		static 
		TypeDeclPtr							parseFunctionType(Parser* parser);

		static 
		TypeDeclPtr							parseVariableType(Parser* parser);

		static
		TypeDeclPtr							parseTupleType(Parser* parser);

		static 
		TypeDeclPtr							parseNamedType(Parser* parser);

		static
		ArrayDeclPtr						parseArrayDecl(Parser* parser);

		static
		TypeDeclNamedPtr					parseInternalNamedType(Parser* parser);

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
		ExpressionDeclPtr					parse(Parser* parser);

		static
		ExpressionDeclPtr					skip(Parser* parser);

		static
		ExpressionDeclPtr					parseEnumExpr(Parser* parser);

		static
		ExpressionDeclPtr					skipEnumExpr(Parser* parser);

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
		PatternDeclPtr						parse(Parser* parser);
	};

	/**
	 * ParserObjectBlockDecl
	 */

	class ParserObjectBlockDecl
	{
	public:
		static
		BlockDeclPtr						parse(Parser* parser);

		static
		BlockDeclPtr						skip(Parser* parser);

		static
		BlockDeclPtr						parseExprBlock(Parser* parser);

		static
		BlockDeclPtr						skipExprBlock(Parser* parser);

		static
		BlockDeclPtr						parseBlockOrExprBlock(Parser* parser);

		static
		BlockDeclPtr						skipBlockOrExprBlock(Parser* parser);
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