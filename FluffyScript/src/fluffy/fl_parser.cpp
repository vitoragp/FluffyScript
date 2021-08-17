#include <cassert>
#include "fl_exceptions.h"
#include "fl_parser.h"
#include "fl_lex_utils.h"
#include "parser_objects/fl_parser_objects.h"
namespace fluffy { namespace parser {
	/**
	 * Parser
	 */

	Parser::Parser(lexer::Lexer* const lex)
		: m_lex(lex)
		, m_typeModeLock(0)
	{}

	Parser::~Parser()
	{}

	ProgramPtr Parser::parse()
	{
		return parser_objects::ParserObjectProgram::parse(this);
	}

	void Parser::loadSource(String source)
	{
		assert(m_lex != nullptr);
		m_lex->loadSource(source);
		m_fileName = "anomblock";
	}

	void Parser::loadSourceFromFile(String sourceFile)
	{
		assert(m_lex != nullptr);
		m_lex->loadFromSource(sourceFile);
		m_fileName = sourceFile;
	}

	void Parser::reinterpretToken(TokenType_e type, TokenSubType_e subType, U32 offset)
	{
		m_tok.type = type;
		m_tok.subType = subType;

		m_lex->setPosition(m_tok.position + offset);
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
		if (!utils::LexUtils::isIdentifier(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		nextToken();
		return value;
	}

	const Bool Parser::expectConstantBool()
	{
		if (!utils::LexUtils::isTrue(m_tok) && !utils::LexUtils::isFalse(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Bool value = utils::LexUtils::isTrue(m_tok) ? true : false;
		nextToken();
		return value;
	}

	const I8 Parser::expectConstantI8()
	{
		if (!utils::LexUtils::isConstantI8(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I8 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U8 Parser::expectConstantU8()
	{
		if (!utils::LexUtils::isConstantU8(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U8 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I16 Parser::expectConstantI16()
	{
		if (!utils::LexUtils::isConstantI16(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I16 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U16 Parser::expectConstantU16()
	{
		if (!utils::LexUtils::isConstantU16(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U16 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I32 Parser::expectConstantI32()
	{
		if (!utils::LexUtils::isConstantI32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I32 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U32 Parser::expectConstantU32()
	{
		if (!utils::LexUtils::isConstantU32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I32 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I64 Parser::expectConstantI64()
	{
		if (!utils::LexUtils::isConstantI64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I64 value = std::stol(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U64 Parser::expectConstantU64()
	{
		if (!utils::LexUtils::isConstantU64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U64 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const Fp32 Parser::expectConstantFp32()
	{
		if (!utils::LexUtils::isConstantFp32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Fp32 value = std::stof(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const Fp64 Parser::expectConstantFp64()
	{
		if (!utils::LexUtils::isConstantFp64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Fp64 value = std::stod(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I8 Parser::expectConstantChar()
	{
		if (!utils::LexUtils::isConstantChar(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I8 value = m_tok.value[0];
		nextToken();
		return value;
	}

	String Parser::expectConstantString()
	{
		if (!utils::LexUtils::isConstantString(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		String value = m_tok.value;
		nextToken();
		return value;
	}

	Bool Parser::isEof()
	{
		return utils::LexUtils::isEof(m_tok);
	}

	Bool Parser::isIdentifier()
	{
		return utils::LexUtils::isIdentifier(m_tok);
	}

	Bool Parser::isConstantI8()
	{
		return utils::LexUtils::isConstantI8(m_tok);
	}

	Bool Parser::isConstantU8()
	{
		return utils::LexUtils::isConstantU8(m_tok);
	}

	Bool Parser::isConstantI16()
	{
		return utils::LexUtils::isConstantI16(m_tok);
	}

	Bool Parser::isConstantU16()
	{
		return utils::LexUtils::isConstantU16(m_tok);
	}

	Bool Parser::isConstantI32()
	{
		return utils::LexUtils::isConstantI32(m_tok);
	}

	Bool Parser::isConstantU32()
	{
		return utils::LexUtils::isConstantU32(m_tok);
	}

	Bool Parser::isConstantI64()
	{
		return utils::LexUtils::isConstantI64(m_tok);
	}

	Bool Parser::isConstantU64()
	{
		return utils::LexUtils::isConstantU64(m_tok);
	}

	Bool Parser::isConstantFp32()
	{
		return utils::LexUtils::isConstantFp32(m_tok);
	}

	Bool Parser::isConstantFp64()
	{
		return utils::LexUtils::isConstantFp64(m_tok);
	}

	Bool Parser::isConstantChar()
	{
		return utils::LexUtils::isConstantChar(m_tok);
	}

	Bool Parser::isConstantString()
	{
		return utils::LexUtils::isConstantString(m_tok);
	}

	Bool Parser::isConstantTrue()
	{
		return utils::LexUtils::isTrue(m_tok);
	}

	Bool Parser::isConstantFalse()
	{
		return utils::LexUtils::isFalse(m_tok);
	}

	Bool Parser::isInclude()
	{
		return utils::LexUtils::isInclude(m_tok);
	}

	Bool Parser::isFrom()
	{
		return utils::LexUtils::isFrom(m_tok);
	}

	Bool Parser::isExport()
	{
		return utils::LexUtils::isExport(m_tok);
	}

	Bool Parser::isNamespace()
	{
		return utils::LexUtils::isNamespace(m_tok);
	}

	Bool Parser::isClass()
	{
		return utils::LexUtils::isClass(m_tok);
	}

	Bool Parser::isExtends()
	{
		return utils::LexUtils::isExtends(m_tok);
	}

	Bool Parser::isWhere()
	{
		return utils::LexUtils::isWhere(m_tok);
	}

	Bool Parser::isImplements()
	{
		return utils::LexUtils::isImplements(m_tok);
	}

	Bool Parser::isConstructor()
	{
		return utils::LexUtils::isConstructor(m_tok);
	}

	Bool Parser::isDestructor()
	{
		return utils::LexUtils::isDestructor(m_tok);
	}

	Bool Parser::isSuper()
	{
		return utils::LexUtils::isSuper(m_tok);
	}

	Bool Parser::isPublic() 
	{
		return utils::LexUtils::isPublic(m_tok);
	}

	Bool Parser::isProtected() 
	{
		return utils::LexUtils::isProtected(m_tok);
	}

	Bool Parser::isPrivate() 
	{
		return utils::LexUtils::isPrivate(m_tok);
	}

	Bool Parser::isFinal() 
	{
		return utils::LexUtils::isFinal(m_tok);
	}

	Bool Parser::isOverride() 
	{
		return utils::LexUtils::isOverride(m_tok);
	}

	Bool Parser::isAbstract()
	{
		return utils::LexUtils::isAbstract(m_tok);
	}

	Bool Parser::isVirtual()
	{
		return utils::LexUtils::isVirtual(m_tok);
	}

	Bool Parser::isInterface()
	{
		return utils::LexUtils::isInterface(m_tok);
	}

	Bool Parser::isStruct()
	{
		return utils::LexUtils::isStruct(m_tok);
	}

	Bool Parser::isEnum()
	{
		return utils::LexUtils::isEnum(m_tok);
	}

	Bool Parser::isMatch()
	{
		return utils::LexUtils::isMatch(m_tok);
	}

	Bool Parser::isWhen()
	{
		return utils::LexUtils::isWhen(m_tok);
	}

	Bool Parser::isTrait()
	{
		return utils::LexUtils::isTrait(m_tok);
	}

	Bool Parser::isStatic()
	{
		return utils::LexUtils::isStatic(m_tok);
	}

	Bool Parser::isConst()
	{
		return utils::LexUtils::isConst(m_tok);
	}

	Bool Parser::isTrue()
	{
		return utils::LexUtils::isTrue(m_tok);
	}

	Bool Parser::isFalse()
	{
		return utils::LexUtils::isFalse(m_tok);
	}

	Bool Parser::isVoid()
	{
		return utils::LexUtils::isVoid(m_tok);
	}

	Bool Parser::isBool()
	{
		return utils::LexUtils::isBool(m_tok);
	}

	Bool Parser::isI8()
	{
		return utils::LexUtils::isI8(m_tok);
	}

	Bool Parser::isU8()
	{
		return utils::LexUtils::isU8(m_tok);
	}

	Bool Parser::isI16()
	{
		return utils::LexUtils::isI16(m_tok);
	}

	Bool Parser::isU16()
	{
		return utils::LexUtils::isU16(m_tok);
	}

	Bool Parser::isI32()
	{
		return utils::LexUtils::isI32(m_tok);
	}

	Bool Parser::isU32()
	{
		return utils::LexUtils::isU32(m_tok);
	}

	Bool Parser::isI64()
	{
		return utils::LexUtils::isI64(m_tok);
	}

	Bool Parser::isU64()
	{
		return utils::LexUtils::isU64(m_tok);
	}

	Bool Parser::isFp32()
	{
		return utils::LexUtils::isFp32(m_tok);
	}

	Bool Parser::isFp64()
	{
		return utils::LexUtils::isFp64(m_tok);
	}

	Bool Parser::isString()
	{
		return utils::LexUtils::isString(m_tok);
	}

	Bool Parser::isVector()
	{
		return utils::LexUtils::isVector(m_tok);
	}

	Bool Parser::isSet()
	{
		return utils::LexUtils::isSet(m_tok);
	}

	Bool Parser::isMap()
	{
		return utils::LexUtils::isMap(m_tok);
	}

	Bool Parser::isObject()
	{
		return utils::LexUtils::isObject(m_tok);
	}

	Bool Parser::isFn()
	{
		return utils::LexUtils::isFn(m_tok);
	}

	Bool Parser::isLet()
	{
		return utils::LexUtils::isLet(m_tok);
	}

	Bool Parser::isThis()
	{
		return utils::LexUtils::isThis(m_tok);
	}

	Bool Parser::isRef()
	{
		return utils::LexUtils::isRef(m_tok);
	}

	Bool Parser::isSelf()
	{
		return utils::LexUtils::isSelf(m_tok);
	}

	Bool Parser::isNew()
	{
		return utils::LexUtils::isNew(m_tok);
	}

	Bool Parser::isAs()
	{
		return utils::LexUtils::isAs(m_tok);
	}

	Bool Parser::isIs()
	{
		return utils::LexUtils::isIs(m_tok);
	}

	Bool Parser::isGoto()
	{
		return utils::LexUtils::isGoto(m_tok);
	}

	Bool Parser::isIf()
	{
		return utils::LexUtils::isIf(m_tok);
	}

	Bool Parser::isElse()
	{
		return utils::LexUtils::isElse(m_tok);
	}

	Bool Parser::isContinue()
	{
		return utils::LexUtils::isContinue(m_tok);
	}

	Bool Parser::isBreak()
	{
		return utils::LexUtils::isBreak(m_tok);
	}

	Bool Parser::isFor()
	{
		return utils::LexUtils::isFor(m_tok);
	}

	Bool Parser::isForeach()
	{
		return utils::LexUtils::isForeach(m_tok);
	}

	Bool Parser::isIn()
	{
		return utils::LexUtils::isIn(m_tok);
	}

	Bool Parser::isWhile()
	{
		return utils::LexUtils::isWhile(m_tok);
	}

	Bool Parser::isDo()
	{
		return utils::LexUtils::isDo(m_tok);
	}

	Bool Parser::isTry()
	{
		return utils::LexUtils::isTry(m_tok);
	}

	Bool Parser::isCatch()
	{
		return utils::LexUtils::isCatch(m_tok);
	}

	Bool Parser::isFinally()
	{
		return utils::LexUtils::isFinally(m_tok);
	}

	Bool Parser::isPanic()
	{
		return utils::LexUtils::isPanic(m_tok);
	}

	Bool Parser::isReturn()
	{
		return utils::LexUtils::isReturn(m_tok);
	}

	Bool Parser::isNull()
	{
		return utils::LexUtils::isNull(m_tok);
	}

	Bool Parser::isScopeResolution()
	{
		return utils::LexUtils::isScopeResolution(m_tok);
	}

	Bool Parser::isIncrement()
	{
		return utils::LexUtils::isIncrement(m_tok);
	}

	Bool Parser::isDecrement()
	{
		return utils::LexUtils::isDecrement(m_tok);
	}

	Bool Parser::isArrow()
	{
		return utils::LexUtils::isArrow(m_tok);
	}

	Bool Parser::isLeftParBracket()
	{
		return utils::LexUtils::isLeftParBracket(m_tok);
	}

	Bool Parser::isRightParBracket()
	{
		return utils::LexUtils::isRightParBracket(m_tok);
	}

	Bool Parser::isLeftSquBracket()
	{
		return utils::LexUtils::isLeftSquBracket(m_tok);
	}

	Bool Parser::isRightSquBracket()
	{
		return utils::LexUtils::isRightSquBracket(m_tok);
	}

	Bool Parser::isLeftBracket()
	{
		return utils::LexUtils::isLeftBracket(m_tok);
	}

	Bool Parser::isRightBracket()
	{
		return utils::LexUtils::isRightBracket(m_tok);
	}

	Bool Parser::isPlus()
	{
		return utils::LexUtils::isPlus(m_tok);
	}

	Bool Parser::isMinus()
	{
		return utils::LexUtils::isMinus(m_tok);
	}

	Bool Parser::isDivision()
	{
		return utils::LexUtils::isDivision(m_tok);
	}

	Bool Parser::isMultiplication()
	{
		return utils::LexUtils::isMultiplication(m_tok);
	}

	Bool Parser::isModulo()
	{
		return utils::LexUtils::isModulo(m_tok);
	}

	Bool Parser::isAssign()
	{
		return utils::LexUtils::isAssign(m_tok);
	}

	Bool Parser::isPlusAssign()
	{
		return utils::LexUtils::isPlusAssign(m_tok);
	}

	Bool Parser::isMinusAssign()
	{
		return utils::LexUtils::isMinusAssign(m_tok);
	}

	Bool Parser::isMultAssign()
	{
		return utils::LexUtils::isMultAssign(m_tok);
	}

	Bool Parser::isDivAssign()
	{
		return utils::LexUtils::isDivAssign(m_tok);
	}

	Bool Parser::isBitWiseAndAssign()
	{
		return utils::LexUtils::isBitWiseAndAssign(m_tok);
	}

	Bool Parser::isBitWiseOrAssign()
	{
		return utils::LexUtils::isBitWiseOrAssign(m_tok);
	}

	Bool Parser::isBitWiseXorAssign()
	{
		return utils::LexUtils::isBitWiseXorAssign(m_tok);
	}

	Bool Parser::isModAssign()
	{
		return utils::LexUtils::isModAssign(m_tok);
	}

	Bool Parser::isBitWiseLeftShiftAssign()
	{
		return utils::LexUtils::isBitWiseLeftShiftAssign(m_tok);
	}

	Bool Parser::isBitWiseRightShiftAssign()
	{
		return utils::LexUtils::isBitWiseRightShiftAssign(m_tok);
	}

	Bool Parser::isGreaterThan()
	{
		return utils::LexUtils::isGreaterThan(m_tok);
	}

	Bool Parser::isLessThan()
	{
		return utils::LexUtils::isLessThan(m_tok);
	}

	Bool Parser::isGreaterThanOrEqual()
	{
		return utils::LexUtils::isGreaterThanOrEqual(m_tok);
	}

	Bool Parser::isLessThanOrEqual()
	{
		return utils::LexUtils::isLessThanOrEqual(m_tok);
	}

	Bool Parser::isEqual()
	{
		return utils::LexUtils::isEqual(m_tok);
	}

	Bool Parser::isNotEqual()
	{
		return utils::LexUtils::isNotEqual(m_tok);
	}

	Bool Parser::isBitWiseLeftShift()
	{
		return utils::LexUtils::isBitWiseLeftShift(m_tok);
	}

	Bool Parser::isBitWiseRightShift()
	{
		return utils::LexUtils::isBitWiseRightShift(m_tok);
	}

	Bool Parser::isBitWiseAnd()
	{
		return utils::LexUtils::isBitWiseAnd(m_tok);
	}

	Bool Parser::isBitWiseOr()
	{
		return utils::LexUtils::isBitWiseOr(m_tok);
	}

	Bool Parser::isBitWiseXor()
	{
		return utils::LexUtils::isBitWiseXor(m_tok);
	}

	Bool Parser::isBitWiseNot()
	{
		return utils::LexUtils::isBitWiseNot(m_tok);
	}

	Bool Parser::isColon()
	{
		return utils::LexUtils::isColon(m_tok);
	}

	Bool Parser::isSemiColon()
	{
		return utils::LexUtils::isSemiColon(m_tok);
	}

	Bool Parser::isLogicalAnd()
	{
		return utils::LexUtils::isLogicalAnd(m_tok);
	}

	Bool Parser::isLogicalOr()
	{
		return utils::LexUtils::isLogicalOr(m_tok);
	}

	Bool Parser::isLogicalNot()
	{
		return utils::LexUtils::isLogicalNot(m_tok);
	}

	Bool Parser::isInterrogation()
	{
		return utils::LexUtils::isInterrogation(m_tok);
	}

	Bool Parser::isComma()
	{
		return utils::LexUtils::isComma(m_tok);
	}

	Bool Parser::isDot()
	{
		return utils::LexUtils::isDot(m_tok);
	}

	const String& Parser::getFilename()
	{
		return m_fileName;
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

	const U32 Parser::getTokenPosition()
	{
		return m_tok.position;
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