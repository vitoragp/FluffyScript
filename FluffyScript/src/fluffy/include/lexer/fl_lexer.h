#pragma once
#include <memory>
#include "fl_defs.h"

namespace fluffy {
	class BufferBase;
}

namespace fluffy { namespace lexer {
	///
	/// Lexer
	///

	class Lexer
	{
	public:
		Lexer(BufferBase* const buffer);
		~Lexer();

		void
		loadSource(const I8* sourceCode);

		void
		loadSourceFromFile(const I8* sourceFilename);

		const I8*
		getFilename();

		void
		nextToken();

		void
		setTabSpaces(U32 newTabSpaces);

		const Token_s&
		getToken();

		void
		resetToPosition(U32 newPosition, U32 newLine, U32 newColumn);

		void
		reinterpretToken(TokenType_e type, U32 offset);

		Token_s
		predictNextToken();

		void
		expectToken(TokenType_e expectedToken);

		String
		expectIdentifier();

		const Bool
		expectConstantBool();

		const I8
		expectConstantInteger();

		const Fp32
		expectConstantFp32();

		const Fp64
		expectConstantFp64();

		const I8
		expectConstantChar();

		String
		expectConstantString();

		Bool
		is(TokenType_e tokenType);

		Bool
		isEof();

		Bool
		isIdentifier();

		Bool
		isConstantInteger();

		Bool
		isConstantFp32();

		Bool
		isConstantFp64();

		Bool
		isConstantChar();

		Bool
		isConstantString();

		Bool
		isConstantTrue();

		Bool
		isConstantFalse();

		Bool
		isInclude();

		Bool
		isExport();

		Bool
		isNamespace();

		Bool
		isClass();

		Bool
		isExtends();

		Bool
		isWhere();

		Bool
		isImplements();

		Bool
		isConstructor();

		Bool
		isDestructor();

		Bool
		isSuper();

		Bool
		isPublic();

		Bool
		isProtected();

		Bool
		isPrivate();

		Bool
		isFinal();

		Bool
		isOverride();

		Bool
		isAbstract();

		Bool
		isInterface();

		Bool
		isStruct();

		Bool
		isEnum();

		Bool
		isMatch();

		Bool
		isWhen();

		Bool
		isTrait();

		Bool
		isStatic();

		Bool
		isConst();

		Bool
		isTrue();

		Bool
		isFalse();

		Bool
		isVoid();

		Bool
		isBool();

		Bool
		isI8();

		Bool
		isU8();

		Bool
		isI16();

		Bool
		isU16();

		Bool
		isI32();

		Bool
		isU32();

		Bool
		isI64();

		Bool
		isU64();

		Bool
		isFp32();

		Bool
		isFp64();

		Bool
		isString();

		Bool
		isObject();

		Bool
		isFn();

		Bool
		isLet();

		Bool
		isThis();

		Bool
		isShared();

		Bool
		isUnique();

		Bool
		isRef();

		Bool
		isSelf();

		Bool
		isNew();

		Bool
		isAs();

		Bool
		isIs();

		Bool
		isGoto();

		Bool
		isIf();

		Bool
		isElse();

		Bool
		isContinue();

		Bool
		isBreak();

		Bool
		isFor();

		Bool
		isIn();

		Bool
		isWhile();

		Bool
		isDo();

		Bool
		isTry();

		Bool
		isCatch();

		Bool
		isFinally();

		Bool
		isPanic();

		Bool
		isReturn();

		Bool
		isNull();

		Bool
		isScopeResolution();

		Bool
		isIncrement();

		Bool
		isDecrement();

		Bool
		isArrow();

		Bool
		isLeftParBracket();

		Bool
		isRightParBracket();

		Bool
		isLeftSquBracket();

		Bool
		isRightSquBracket();

		Bool
		isLeftBracket();

		Bool
		isRightBracket();

		Bool
		isPlus();

		Bool
		isMinus();

		Bool
		isDivision();

		Bool
		isMultiplication();

		Bool
		isModulo();

		Bool
		isAssign();

		Bool
		isPlusAssign();

		Bool
		isMinusAssign();

		Bool
		isMultAssign();

		Bool
		isDivAssign();

		Bool
		isBitWiseAndAssign();

		Bool
		isBitWiseOrAssign();

		Bool
		isBitWiseXorAssign();

		Bool
		isModAssign();

		Bool
		isBitWiseLeftShiftAssign();

		Bool
		isBitWiseRightShiftAssign();

		Bool
		isGreaterThan();

		Bool
		isLessThan();

		Bool
		isGreaterThanOrEqual();

		Bool
		isLessThanOrEqual();

		Bool
		isEqual();

		Bool
		isNotEqual();

		Bool
		isBitWiseLeftShift();

		Bool
		isBitWiseRightShift();

		Bool
		isBitWiseAnd();

		Bool
		isBitWiseOr();

		Bool
		isBitWiseXor();

		Bool
		isBitWiseNot();

		Bool
		isColon();

		Bool
		isSemiColon();

		Bool
		isLogicalAnd();

		Bool
		isLogicalOr();

		Bool
		isLogicalNot();

		Bool
		isInterrogation();

		Bool
		isComma();

		Bool
		isDot();

		Bool
		isSafeDot();

		Bool
		isEllipsis();

	private:
		void
		parse();

		I8
		readChar(U32 offset = 0);

		I8
		readCharAndAdv();

		void
		nextChar();

		void
		skip();

		void
		parseIdentifier();

		void
		parseSymbols();

		void
		parseNumbers();

		void
		parseChar();

		void
		parseString();

	private:
		std::unique_ptr<BufferBase>
		m_buffer;

		Token_s
		m_token;

		String
		m_filename;

		U32
		m_tabSpaces;

		U32
		m_line;

		U32
		m_column;

		Bool
		m_eof;

	};
} }


