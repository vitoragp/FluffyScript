#pragma once
#include "../fl_defs.h"
#include "../fl_parser.h"
	

namespace fluffy { namespace parser_objects { 
	using std::unique_ptr;
	using fluffy::parser::Parser;

	using ProgramPtr						= unique_ptr<ast::Program>;
	using IncludePtr						= unique_ptr<ast::Include>;
	using NamespacePtr						= unique_ptr<ast::Namespace>;
	using GeneralStmtPtr					= unique_ptr<ast::GeneralStmt>;
	using ScopedIdentifierDeclPtr			= unique_ptr<ast::ScopedIdentifierDecl>;

	/**
	 * ParserObjectProgram
	 */

	class ParserObjectProgram
	{
	public:
		static
		ProgramPtr				parse(Parser* parser);
	};

	/**
	 * ParserObjectInclude
	 */

	class ParserObjectInclude
	{
	public:
		static
		IncludePtr				parse(Parser* parser);
	};

	/**
	 * ParserObjectNamespace
	 */

	class ParserObjectNamespace
	{
	public:
		static
		NamespacePtr			parse(Parser* parser);
	};

	/**
	 * ParserObjectGeneralStmt
	 */

	class ParserObjectGeneralStmt
	{
	public:
		static
		GeneralStmtPtr			parse(Parser* parser);
	};

	/**
	 * ParserObjectScopedIdentifier
	 */

	class ParserObjectScopedIdentifier
	{
	public:
		static
		ScopedIdentifierDeclPtr	parse(Parser* parser);
	};
} }