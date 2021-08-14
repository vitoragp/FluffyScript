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

	using ProgramPtr			= unique_ptr<ast::Program>;

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

		void					nextToken();

		void					expectToken(std::function<bool()> callback);
		String					expectIdentifier();
		const Bool				expectConstantBool();
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

		Bool					isEof();
		Bool					isIdentifier();		
		Bool					isConstantI8();		
		Bool					isConstantU8();		
		Bool					isConstantI16();		
		Bool					isConstantU16();		
		Bool					isConstantI32();		
		Bool					isConstantU32();		
		Bool					isConstantI64();		
		Bool					isConstantU64();		
		Bool					isConstantFp32();		
		Bool					isConstantFp64();		
		Bool					isConstantChar();
		Bool					isConstantString();
		Bool					isConstantTrue();
		Bool					isConstantFalse();
		Bool					isInclude();
		Bool					isFrom();
		Bool					isNamespace();
		Bool					isExport();
		Bool					isLeftBracket();
		Bool					isRightBracket();
		Bool					isScopeResolution();
		Bool					isSemiColon();
		Bool					isComma();
		Bool					isMultiplication();

		const String&			getTokenValue();
		const TokenType_e		getTokenType();
		const TokenSubType_e	getTokenSubType();
		const U32				getTokenLine();
		const U32				getTokenColumn();

	private:
		unique_ptr<Lexer>		m_lex;
		Token_s					m_tok;
	};
} }
