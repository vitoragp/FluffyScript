#pragma once
#include <memory>
#include <functional>
#include "..\fl_defs.h"
#include "fl_ast_decl.h"

namespace fluffy { namespace lexer { class Lexer; } }

namespace fluffy { namespace parser {
	using fluffy::lexer::Lexer;

	using CodeUnitPtr					= std::unique_ptr<ast::CodeUnit>;

	using ErrCallback					= std::function<void()>;

	/**
	 * Parser
	 */

	class Parser
	{
	public:
										Parser(Lexer* const lex);
										~Parser();

		CodeUnitPtr						parse();

		Token_s							parseNextToken();

		void							loadSource(String source);
		void							loadSourceFromFile(String sourceFile);

		void							resetToPosition(U32 position);

		void							reinterpretToken(TokenType_e type, U32 offset);

		void							nextToken();

		void							expectToken(TokenType_e expectedToken);
		void							expectToken(TokenType_e expectedToken, ErrCallback errcallback);

		String							expectIdentifier();
		const Bool						expectConstantBool();
		const I8						expectConstantInteger();
		const Fp32						expectConstantFp32();
		const Fp64						expectConstantFp64();
		const I8						expectConstantChar();
		String							expectConstantString();

		Bool							isEof();
		Bool							isIdentifier();		
		Bool							isConstantInteger();				
		Bool							isConstantFp32();		
		Bool							isConstantFp64();		
		Bool							isConstantChar();
		Bool							isConstantString();
		Bool							isConstantTrue();
		Bool							isConstantFalse();

		Bool							isInclude();
		Bool							isFrom();
		Bool							isExport();
		Bool							isNamespace();
		Bool							isClass();
		Bool							isExtends();
		Bool							isWhere();
		Bool							isImplements();
		Bool							isConstructor();
		Bool							isDestructor();
		Bool							isSuper();
		Bool							isPublic();
		Bool							isProtected();
		Bool							isPrivate();
		Bool							isFinal();
		Bool							isOverride();
		Bool							isAbstract();
		Bool							isVirtual();
		Bool							isInterface();
		Bool							isStruct();
		Bool							isEnum();
		Bool							isMatch();
		Bool							isWhen();
		Bool							isTrait();
		Bool							isStatic();
		Bool							isConst();
		Bool							isTrue();
		Bool							isFalse();
		Bool							isVoid();
		Bool							isBool();
		Bool							isI8();
		Bool							isU8();
		Bool							isI16();
		Bool							isU16();
		Bool							isI32();
		Bool							isU32();
		Bool							isI64();
		Bool							isU64();
		Bool							isFp32();
		Bool							isFp64();
		Bool							isString();
		Bool							isObject();
		Bool							isFn();
		Bool							isLet();
		Bool							isThis();
		Bool							isRef();
		Bool							isSelf();
		Bool							isNew();
		Bool							isAs();
		Bool							isIs();
		Bool							isGoto();
		Bool							isIf();
		Bool							isElse();
		Bool							isContinue();
		Bool							isBreak();
		Bool							isFor();
		Bool							isIn();
		Bool							isWhile();
		Bool							isDo();
		Bool							isTry();
		Bool							isCatch();
		Bool							isFinally();
		Bool							isPanic();
		Bool							isReturn();
		Bool							isNull();

		Bool							isScopeResolution();
		Bool							isIncrement();
		Bool							isDecrement();
		Bool							isArrow();
		Bool							isLeftParBracket();
		Bool							isRightParBracket();
		Bool							isLeftSquBracket();
		Bool							isRightSquBracket();
		Bool							isLeftBracket();
		Bool							isRightBracket();
		Bool							isPlus();
		Bool							isMinus();
		Bool							isDivision();
		Bool							isMultiplication();
		Bool							isModulo();
		Bool							isAssign();
		Bool							isPlusAssign();
		Bool							isMinusAssign();
		Bool							isMultAssign();
		Bool							isDivAssign();
		Bool							isBitWiseAndAssign();
		Bool							isBitWiseOrAssign();
		Bool							isBitWiseXorAssign();
		Bool							isModAssign();
		Bool							isBitWiseLeftShiftAssign();
		Bool							isBitWiseRightShiftAssign();
		Bool							isGreaterThan();
		Bool							isLessThan();
		Bool							isGreaterThanOrEqual();
		Bool							isLessThanOrEqual();
		Bool							isEqual();
		Bool							isNotEqual();
		Bool							isBitWiseLeftShift();
		Bool							isBitWiseRightShift();
		Bool							isBitWiseAnd();
		Bool							isBitWiseOr();
		Bool							isBitWiseXor();
		Bool							isBitWiseNot();
		Bool							isColon();
		Bool							isSemiColon();
		Bool							isLogicalAnd();
		Bool							isLogicalOr();
		Bool							isLogicalNot();
		Bool							isInterrogation();
		Bool							isComma();
		Bool							isDot();
		Bool							isEllipsis();

		const String&					getFilename();

		const String&					getTokenValue();
		const TokenType_e				getTokenType();
		const U32						getTokenPosition();
		const U32						getTokenLine();
		const U32						getTokenColumn();

	private:
		std::unique_ptr<lexer::Lexer>	m_lex;
		Token_s							m_tok;
		I32								m_typeModeLock;
		String							m_fileName;
	};
} }
