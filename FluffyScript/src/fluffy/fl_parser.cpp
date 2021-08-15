#include <cassert>
#include "fl_exceptions.h"
#include "fl_parser.h"
#include "fl_lex_utils.h"
#include "parser_objects/fl_parser_objects.h"
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

	Bool Parser::isExport()
	{
		return LexUtils::isExport(m_tok);
	}

	Bool Parser::isNamespace()
	{
		return LexUtils::isNamespace(m_tok);
	}

	Bool Parser::isClass()
	{
		return LexUtils::isClass(m_tok);
	}

	Bool Parser::isExtends()
	{
		return LexUtils::isExtends(m_tok);
	}

	Bool Parser::isImplements()
	{
		return LexUtils::isImplements(m_tok);
	}

	Bool Parser::isConstructor()
	{
		return LexUtils::isConstructor(m_tok);
	}

	Bool Parser::isDestructor()
	{
		return LexUtils::isDestructor(m_tok);
	}

	Bool Parser::isSuper()
	{
		return LexUtils::isSuper(m_tok);
	}

	Bool Parser::isPublic() 
	{
		return LexUtils::isPublic(m_tok);
	}

	Bool Parser::isProtected() 
	{
		return LexUtils::isProtected(m_tok);
	}

	Bool Parser::isPrivate() 
	{
		return LexUtils::isPrivate(m_tok);
	}

	Bool Parser::isFinal() 
	{
		return LexUtils::isFinal(m_tok);
	}

	Bool Parser::isOverride() 
	{
		return LexUtils::isOverride(m_tok);
	}

	Bool Parser::isAbstract()
	{
		return LexUtils::isAbstract(m_tok);
	}

	Bool Parser::isVirtual()
	{
		return LexUtils::isVirtual(m_tok);
	}

	Bool Parser::isInterface()
	{
		return LexUtils::isInterface(m_tok);
	}

	Bool Parser::isStruct()
	{
		return LexUtils::isStruct(m_tok);
	}

	Bool Parser::isEnum()
	{
		return LexUtils::isEnum(m_tok);
	}

	Bool Parser::isMatch()
	{
		return LexUtils::isMatch(m_tok);
	}

	Bool Parser::isWhen()
	{
		return LexUtils::isWhen(m_tok);
	}

	Bool Parser::isTrait()
	{
		return LexUtils::isTrait(m_tok);
	}

	Bool Parser::isStatic()
	{
		return LexUtils::isStatic(m_tok);
	}

	Bool Parser::isConst()
	{
		return LexUtils::isConst(m_tok);
	}

	Bool Parser::isTrue()
	{
		return LexUtils::isTrue(m_tok);
	}

	Bool Parser::isFalse()
	{
		return LexUtils::isFalse(m_tok);
	}

	Bool Parser::isVoid()
	{
		return LexUtils::isVoid(m_tok);
	}

	Bool Parser::isBool()
	{
		return LexUtils::isBool(m_tok);
	}

	Bool Parser::isI8()
	{
		return LexUtils::isI8(m_tok);
	}

	Bool Parser::isU8()
	{
		return LexUtils::isU8(m_tok);
	}

	Bool Parser::isI16()
	{
		return LexUtils::isI16(m_tok);
	}

	Bool Parser::isU16()
	{
		return LexUtils::isU16(m_tok);
	}

	Bool Parser::isI32()
	{
		return LexUtils::isI32(m_tok);
	}

	Bool Parser::isU32()
	{
		return LexUtils::isU32(m_tok);
	}

	Bool Parser::isI64()
	{
		return LexUtils::isI64(m_tok);
	}

	Bool Parser::isU64()
	{
		return LexUtils::isU64(m_tok);
	}

	Bool Parser::isFp32()
	{
		return LexUtils::isFp32(m_tok);
	}

	Bool Parser::isFp64()
	{
		return LexUtils::isFp64(m_tok);
	}

	Bool Parser::isString()
	{
		return LexUtils::isString(m_tok);
	}

	Bool Parser::isVector()
	{
		return LexUtils::isVector(m_tok);
	}

	Bool Parser::isSet()
	{
		return LexUtils::isSet(m_tok);
	}

	Bool Parser::isMap()
	{
		return LexUtils::isMap(m_tok);
	}

	Bool Parser::isObject()
	{
		return LexUtils::isObject(m_tok);
	}

	Bool Parser::isFn()
	{
		return LexUtils::isFn(m_tok);
	}

	Bool Parser::isLet()
	{
		return LexUtils::isLet(m_tok);
	}

	Bool Parser::isThis()
	{
		return LexUtils::isThis(m_tok);
	}

	Bool Parser::isSelf()
	{
		return LexUtils::isSelf(m_tok);
	}

	Bool Parser::isSizeOf()
	{
		return LexUtils::isSizeOf(m_tok);
	}

	Bool Parser::isNew()
	{
		return LexUtils::isNew(m_tok);
	}

	Bool Parser::isAs()
	{
		return LexUtils::isAs(m_tok);
	}

	Bool Parser::isIs()
	{
		return LexUtils::isIs(m_tok);
	}

	Bool Parser::isGoto()
	{
		return LexUtils::isGoto(m_tok);
	}

	Bool Parser::isIf()
	{
		return LexUtils::isIf(m_tok);
	}

	Bool Parser::isElse()
	{
		return LexUtils::isElse(m_tok);
	}

	Bool Parser::isContinue()
	{
		return LexUtils::isContinue(m_tok);
	}

	Bool Parser::isBreak()
	{
		return LexUtils::isBreak(m_tok);
	}

	Bool Parser::isFor()
	{
		return LexUtils::isFor(m_tok);
	}

	Bool Parser::isForeach()
	{
		return LexUtils::isForeach(m_tok);
	}

	Bool Parser::isIn()
	{
		return LexUtils::isIn(m_tok);
	}

	Bool Parser::isWhile()
	{
		return LexUtils::isWhile(m_tok);
	}

	Bool Parser::isDo()
	{
		return LexUtils::isDo(m_tok);
	}

	Bool Parser::isTry()
	{
		return LexUtils::isTry(m_tok);
	}

	Bool Parser::isCatch()
	{
		return LexUtils::isCatch(m_tok);
	}

	Bool Parser::isFinally()
	{
		return LexUtils::isFinally(m_tok);
	}

	Bool Parser::isPanic()
	{
		return LexUtils::isPanic(m_tok);
	}

	Bool Parser::isReturn()
	{
		return LexUtils::isReturn(m_tok);
	}

	Bool Parser::isNull()
	{
		return LexUtils::isNull(m_tok);
	}

	Bool Parser::isScopeResolution()
	{
		return LexUtils::isScopeResolution(m_tok);
	}

	Bool Parser::isIncrement()
	{
		return LexUtils::isIncrement(m_tok);
	}

	Bool Parser::isDecrement()
	{
		return LexUtils::isDecrement(m_tok);
	}

	Bool Parser::isArrow()
	{
		return LexUtils::isArrow(m_tok);
	}

	Bool Parser::isLeftParBracket()
	{
		return LexUtils::isLeftParBracket(m_tok);
	}

	Bool Parser::isRightParBracket()
	{
		return LexUtils::isRightParBracket(m_tok);
	}

	Bool Parser::isLeftSquBracket()
	{
		return LexUtils::isLeftSquBracket(m_tok);
	}

	Bool Parser::isRightSquBracket()
	{
		return LexUtils::isRightSquBracket(m_tok);
	}

	Bool Parser::isLeftBracket()
	{
		return LexUtils::isLeftBracket(m_tok);
	}

	Bool Parser::isRightBracket()
	{
		return LexUtils::isRightBracket(m_tok);
	}

	Bool Parser::isPlus()
	{
		return LexUtils::isPlus(m_tok);
	}

	Bool Parser::isMinus()
	{
		return LexUtils::isMinus(m_tok);
	}

	Bool Parser::isDivision()
	{
		return LexUtils::isDivision(m_tok);
	}

	Bool Parser::isMultiplication()
	{
		return LexUtils::isMultiplication(m_tok);
	}

	Bool Parser::isModulo()
	{
		return LexUtils::isModulo(m_tok);
	}

	Bool Parser::isAssign()
	{
		return LexUtils::isAssign(m_tok);
	}

	Bool Parser::isPlusAssign()
	{
		return LexUtils::isPlusAssign(m_tok);
	}

	Bool Parser::isMinusAssign()
	{
		return LexUtils::isMinusAssign(m_tok);
	}

	Bool Parser::isMultAssign()
	{
		return LexUtils::isMultAssign(m_tok);
	}

	Bool Parser::isDivAssign()
	{
		return LexUtils::isDivAssign(m_tok);
	}

	Bool Parser::isBitWiseAndAssign()
	{
		return LexUtils::isBitWiseAndAssign(m_tok);
	}

	Bool Parser::isBitWiseOrAssign()
	{
		return LexUtils::isBitWiseOrAssign(m_tok);
	}

	Bool Parser::isBitWiseXorAssign()
	{
		return LexUtils::isBitWiseXorAssign(m_tok);
	}

	Bool Parser::isModAssign()
	{
		return LexUtils::isModAssign(m_tok);
	}

	Bool Parser::isBitWiseLeftShiftAssign()
	{
		return LexUtils::isBitWiseLeftShiftAssign(m_tok);
	}

	Bool Parser::isBitWiseRightShiftAssign()
	{
		return LexUtils::isBitWiseRightShiftAssign(m_tok);
	}

	Bool Parser::isGreaterThan()
	{
		return LexUtils::isGreaterThan(m_tok);
	}

	Bool Parser::isLessThan()
	{
		return LexUtils::isLessThan(m_tok);
	}

	Bool Parser::isGreaterThanOrEqual()
	{
		return LexUtils::isGreaterThanOrEqual(m_tok);
	}

	Bool Parser::isLessThanOrEqual()
	{
		return LexUtils::isLessThanOrEqual(m_tok);
	}

	Bool Parser::isEqual()
	{
		return LexUtils::isEqual(m_tok);
	}

	Bool Parser::isNotEqual()
	{
		return LexUtils::isNotEqual(m_tok);
	}

	Bool Parser::isBitWiseLeftShift()
	{
		return LexUtils::isBitWiseLeftShift(m_tok);
	}

	Bool Parser::isBitWiseRightShift()
	{
		return LexUtils::isBitWiseRightShift(m_tok);
	}

	Bool Parser::isBitWiseAnd()
	{
		return LexUtils::isBitWiseAnd(m_tok);
	}

	Bool Parser::isBitWiseOr()
	{
		return LexUtils::isBitWiseOr(m_tok);
	}

	Bool Parser::isBitWiseXor()
	{
		return LexUtils::isBitWiseXor(m_tok);
	}

	Bool Parser::isBitWiseNot()
	{
		return LexUtils::isBitWiseNot(m_tok);
	}

	Bool Parser::isColon()
	{
		return LexUtils::isColon(m_tok);
	}

	Bool Parser::isSemiColon()
	{
		return LexUtils::isSemiColon(m_tok);
	}

	Bool Parser::isLogicalAnd()
	{
		return LexUtils::isLogicalAnd(m_tok);
	}

	Bool Parser::isLogicalOr()
	{
		return LexUtils::isLogicalOr(m_tok);
	}

	Bool Parser::isLogicalNot()
	{
		return LexUtils::isLogicalNot(m_tok);
	}

	Bool Parser::isInterrogation()
	{
		return LexUtils::isInterrogation(m_tok);
	}

	Bool Parser::isComma()
	{
		return LexUtils::isComma(m_tok);
	}

	Bool Parser::isDot()
	{
		return LexUtils::isDot(m_tok);
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