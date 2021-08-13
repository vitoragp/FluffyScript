#pragma once
#include <memory>
#include <functional>
#include "fl_defs.h"
#include "fl_ast.h"
#include "fl_lex.h"
namespace fluffy { namespace parser {
	using lexer::Lexer;
	using std::unique_ptr;

	using ProgramPtr			= unique_ptr<ast::Program>;
	using IncludePtr			= unique_ptr<ast::Include>;
	using NamespacePtr			= unique_ptr<ast::Namespace>;
	using GeneralStmtPtr		= unique_ptr<ast::GeneralStmt>;
	using NamedDeclPtr			= unique_ptr<ast::NamedDecl>;

	/**
	 * Parser
	 */

	class Parser
	{
	public:
								Parser(Lexer* const lex);
								~Parser();

		ProgramPtr				parse();

		void					loadSource(String source);
		void					loadSourceFromFile(String sourceFile);

	private:
		void					nextToken();
		void					expectToken(std::function<bool()> callback);
		String					expectIdentifier();
		String					expectConstantString();

		ProgramPtr				parseProgram();
		IncludePtr				parseInclude();
		NamespacePtr			parseNamespace();
		GeneralStmtPtr			parseGeneralStmt();

		NamedDeclPtr			parseNamedDecl();

	private:
		unique_ptr<Lexer>		m_lex;
		Token_s					m_tok;
	};
} }
