#pragma once
#include "../fl_defs.h"
#include "../fl_parser.h"
	

namespace fluffy { namespace parser_objects { 
	using std::unique_ptr;
	using std::vector;

	using fluffy::parser::Parser;

	using StringList						= vector<String>;

	using ProgramPtr						= unique_ptr<ast::Program>;
	using IncludeDeclPtr					= unique_ptr<ast::IncludeDecl>;
	using NamespaceDeclPtr					= unique_ptr<ast::NamespaceDecl>;
	using GeneralStmtPtr					= unique_ptr<ast::GeneralStmt>;
	using ClassDeclPtr						= unique_ptr<ast::ClassDecl>;
	using ClassFunctionDeclPtr				= unique_ptr<ast::ClassFunctionDecl>;

	using ClassFunctionParameterDeclPtr		= unique_ptr<ast::ClassFunctionParameterDecl>;
	using ClassFunctionParameterDeclPtrList = vector<ClassFunctionParameterDeclPtr>;
	

	using GenericDeclPtr					= unique_ptr<ast::GenericDecl>;
	using GenericDeclPtrList				= vector<GenericDeclPtr>;

	using TypeDeclPtr						= unique_ptr<ast::TypeDecl>;
	using TypeDeclPtrList					= vector<ast::TypeDeclPtr>;

	using TypeDeclNamedPtr					= unique_ptr<ast::TypeDeclNamed>;

	using ArrayDeclPtr						= unique_ptr<ast::ArrayDecl>;

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
	 * ParserObjectGeneralStmt
	 */

	class ParserObjectGeneralStmt
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
		ClassFunctionParameterDeclPtrList	parseParameterList(Parser* parser);
	};

	/**
	 * ParserObjectGenericTemplateDecl
	 */

	class ParserObjectGenericTemplateDecl
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
		TypeDeclPtr							parseNamedType(Parser* parser);
		static
		ArrayDeclPtr						parseArrayDecl(Parser* parser);
		static
		TypeDeclNamedPtr					parseInternalNamedType(Parser* parser);
		static
		void								reinterpretToken(Parser* parser);

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