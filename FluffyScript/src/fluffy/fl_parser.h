#pragma once
#include <memory>
#include <functional>
#include "fl_defs.h"
#include "fl_ast.h"
#include "fl_lex.h"

namespace fluffy { namespace testing { class ParserTest_TestParseConstants_Test; } }

namespace fluffy { namespace parser {
	using lexer::Lexer;
	using std::unique_ptr;

	using ProgramPtr					= unique_ptr<ast::Program>;
	using IncludePtr					= unique_ptr<ast::Include>;
	using NamespacePtr					= unique_ptr<ast::Namespace>;
	using GeneralStmtPtr				= unique_ptr<ast::GeneralStmt>;
	using ScopedIdentifierDeclPtr		= unique_ptr<ast::ScopedIdentifierDecl>;

	/**
	 * Parser
	 */

	class Parser
	{
		friend class			testing::ParserTest_TestParseConstants_Test;

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
		const I8				expectConstantI8();
		const U8				expectConstantU8();
		const I16				expectConstantI16();
		const U16				expectConstantU16();
		const I32				expectConstantI32();
		const U32				expectConstantU32();
		const I64				expectConstantI64();
		const U64				expectConstantU64();
		const Fp32				expectConstantFp32();
		const Fp64				expectConstantFp64();
		const I8				expectConstantChar();
		String					expectConstantString();

		ProgramPtr				parseProgram();
		IncludePtr				parseInclude();
		NamespacePtr			parseNamespace();
		GeneralStmtPtr			parseGeneralStmt();

		ScopedIdentifierDeclPtr	parseScopedIdentifierDecl();

	private:
		unique_ptr<Lexer>		m_lex;
		Token_s					m_tok;
	};
} }
