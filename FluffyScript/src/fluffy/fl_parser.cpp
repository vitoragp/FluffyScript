#include <cassert>
#include "fl_exceptions.h"
#include "fl_parser.h"
#include "fl_lex_utils.h"
#include "parser_objects/fl_parser_object.h"
namespace fluffy { namespace parser {
	using utils::LexUtils;
	using parser_objects::ParserObjectProgram;

	/**
	 * Parser
	 */

	Parser::Parser(Lexer* const lex)
		: m_lex(lex)
	{}

	Parser::~Parser()
	{}

	ProgramPtr Parser::parse()
	{
		return ParserObjectProgram::parse(this);
	}

	void Parser::loadSource(String source)
	{
		assert(m_lex != nullptr);
		m_lex->loadSource(source);
	}

	void Parser::loadSourceFromFile(String sourceFile)
	{
		assert(m_lex != nullptr);
		m_lex->loadFromSource(sourceFile);
	}

	void Parser::nextToken()
	{
		m_lex->parse(m_tok);
	}

	void Parser::expectToken(std::function<bool()> callback)
	{
		if (!callback()) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		nextToken();
	}

	String Parser::expectIdentifier()
	{
		String value = m_tok.value;
		if (!LexUtils::isIdentifier(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		nextToken();
		return value;
	}

	const Bool Parser::expectConstantBool()
	{
		if (!LexUtils::isTrue(m_tok) && !LexUtils::isFalse(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Bool value = LexUtils::isTrue(m_tok) ? true : false;
		nextToken();
		return value;
	}

	const I8 Parser::expectConstantI8()
	{
		if (!LexUtils::isConstantI8(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I8 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U8 Parser::expectConstantU8()
	{
		if (!LexUtils::isConstantU8(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U8 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I16 Parser::expectConstantI16()
	{
		if (!LexUtils::isConstantI16(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I16 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U16 Parser::expectConstantU16()
	{
		if (!LexUtils::isConstantU16(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U16 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I32 Parser::expectConstantI32()
	{
		if (!LexUtils::isConstantI32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I32 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U32 Parser::expectConstantU32()
	{
		if (!LexUtils::isConstantU32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I32 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I64 Parser::expectConstantI64()
	{
		if (!LexUtils::isConstantI64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I64 value = std::stol(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U64 Parser::expectConstantU64()
	{
		if (!LexUtils::isConstantU64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U64 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const Fp32 Parser::expectConstantFp32()
	{
		if (!LexUtils::isConstantFp32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Fp32 value = std::stof(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const Fp64 Parser::expectConstantFp64()
	{
		if (!LexUtils::isConstantFp64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Fp64 value = std::stod(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I8 Parser::expectConstantChar()
	{
		if (!LexUtils::isConstantChar(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I8 value = m_tok.value[0];
		nextToken();
		return value;
	}

	String Parser::expectConstantString()
	{
		if (!LexUtils::isConstantString(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		String value = m_tok.value;
		nextToken();
		return value;
	}

	Bool Parser::isEof()
	{
		return LexUtils::isEof(m_tok);
	}

	Bool Parser::isIdentifier()
	{
		return LexUtils::isIdentifier(m_tok);
	}

	Bool Parser::isConstantI8()
	{
		return LexUtils::isConstantI8(m_tok);
	}

	Bool Parser::isConstantU8()
	{
		return LexUtils::isConstantU8(m_tok);
	}

	Bool Parser::isConstantI16()
	{
		return LexUtils::isConstantI16(m_tok);
	}

	Bool Parser::isConstantU16()
	{
		return LexUtils::isConstantU16(m_tok);
	}

	Bool Parser::isConstantI32()
	{
		return LexUtils::isConstantI32(m_tok);
	}

	Bool Parser::isConstantU32()
	{
		return LexUtils::isConstantU32(m_tok);
	}

	Bool Parser::isConstantI64()
	{
		return LexUtils::isConstantI64(m_tok);
	}

	Bool Parser::isConstantU64()
	{
		return LexUtils::isConstantU64(m_tok);
	}

	Bool Parser::isConstantFp32()
	{
		return LexUtils::isConstantFp32(m_tok);
	}

	Bool Parser::isConstantFp64()
	{
		return LexUtils::isConstantFp64(m_tok);
	}

	Bool Parser::isConstantChar()
	{
		return LexUtils::isConstantChar(m_tok);
	}

	Bool Parser::isConstantString()
	{
		return LexUtils::isConstantString(m_tok);
	}

	Bool Parser::isConstantTrue()
	{
		return LexUtils::isTrue(m_tok);
	}

	Bool Parser::isConstantFalse()
	{
		return LexUtils::isFalse(m_tok);
	}

	Bool Parser::isInclude()
	{
		return LexUtils::isInclude(m_tok);
	}

	Bool Parser::isFrom()
	{
		return LexUtils::isFrom(m_tok);
	}

	Bool Parser::isNamespace()
	{
		return LexUtils::isNamespace(m_tok);
	}

	Bool Parser::isExport()
	{
		return LexUtils::isExport(m_tok);
	}

	Bool Parser::isLeftBracket()
	{
		return LexUtils::isLeftBracket(m_tok);
	}

	Bool Parser::isRightBracket()
	{
		return LexUtils::isRightBracket(m_tok);
	}

	Bool Parser::isScopeResolution()
	{
		return LexUtils::isScopeResolution(m_tok);
	}

	Bool Parser::isSemiColon()
	{
		return LexUtils::isSemiColon(m_tok);
	}

	Bool Parser::isComma()
	{
		return LexUtils::isComma(m_tok);
	}

	Bool Parser::isMultiplication()
	{
		return LexUtils::isMultiplication(m_tok);
	}

	const String& Parser::getTokenValue()
	{
		return m_tok.value;
	}

	const TokenType_e Parser::getTokenType()
	{
		return m_tok.type;
	}

	const TokenSubType_e Parser::getTokenSubType()
	{
		return m_tok.subType;
	}

	const U32 Parser::getTokenLine()
	{
		return m_tok.line;
	}

	const U32 Parser::getTokenColumn()
	{
		return m_tok.column;
	}
} }