#pragma once
#include <memory>
#include <functional>
#include "fl_defs.h"
#include "fl_ast.h"
#include "fl_lex.h"

namespace fluffy { namespace parser_objects { class ParserObjectTypeDecl; } }

namespace fluffy { namespace parser {
	using lexer::Lexer;
	using std::unique_ptr;
	using fluffy::parser_objects::ParserObjectTypeDecl;

	using ProgramPtr			= unique_ptr<ast::Program>;
	using TypeDeclPtr			= unique_ptr<ast::TypeDecl>;

	/**
	 * Parser
	 */

	class Parser
	{
		friend class			ParserObjectTypeDecl;

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
		Bool					isExport();
		Bool					isNamespace();
		Bool					isClass();
		Bool					isExtends();
		Bool					isImplements();
		Bool					isConstructor();
		Bool					isDestructor();
		Bool					isSuper();
		Bool					isPublic();
		Bool					isProtected();
		Bool					isPrivate();
		Bool					isFinal();
		Bool					isOverride();
		Bool					isAbstract();
		Bool					isVirtual();
		Bool					isInterface();
		Bool					isStruct();
		Bool					isEnum();
		Bool					isMatch();
		Bool					isWhen();
		Bool					isTrait();
		Bool					isStatic();
		Bool					isConst();
		Bool					isTrue();
		Bool					isFalse();
		Bool					isVoid();
		Bool					isBool();
		Bool					isI8();
		Bool					isU8();
		Bool					isI16();
		Bool					isU16();
		Bool					isI32();
		Bool					isU32();
		Bool					isI64();
		Bool					isU64();
		Bool					isFp32();
		Bool					isFp64();
		Bool					isString();
		Bool					isVector();
		Bool					isSet();
		Bool					isMap();
		Bool					isObject();
		Bool					isFn();
		Bool					isLet();
		Bool					isThis();
		Bool					isSelf();
		Bool					isSizeOf();
		Bool					isNew();
		Bool					isAs();
		Bool					isIs();
		Bool					isGoto();
		Bool					isIf();
		Bool					isElse();
		Bool					isContinue();
		Bool					isBreak();
		Bool					isFor();
		Bool					isForeach();
		Bool					isIn();
		Bool					isWhile();
		Bool					isDo();
		Bool					isTry();
		Bool					isCatch();
		Bool					isFinally();
		Bool					isPanic();
		Bool					isReturn();
		Bool					isNull();

		Bool					isScopeResolution();
		Bool					isIncrement();
		Bool					isDecrement();
		Bool					isArrow();
		Bool					isLeftParBracket();
		Bool					isRightParBracket();
		Bool					isLeftSquBracket();
		Bool					isRightSquBracket();
		Bool					isLeftBracket();
		Bool					isRightBracket();
		Bool					isPlus();
		Bool					isMinus();
		Bool					isDivision();
		Bool					isMultiplication();
		Bool					isModulo();
		Bool					isAssign();
		Bool					isPlusAssign();
		Bool					isMinusAssign();
		Bool					isMultAssign();
		Bool					isDivAssign();
		Bool					isBitWiseAndAssign();
		Bool					isBitWiseOrAssign();
		Bool					isBitWiseXorAssign();
		Bool					isModAssign();
		Bool					isBitWiseLeftShiftAssign();
		Bool					isBitWiseRightShiftAssign();
		Bool					isGreaterThan();
		Bool					isLessThan();
		Bool					isGreaterThanOrEqual();
		Bool					isLessThanOrEqual();
		Bool					isEqual();
		Bool					isNotEqual();
		Bool					isBitWiseLeftShift();
		Bool					isBitWiseRightShift();
		Bool					isBitWiseAnd();
		Bool					isBitWiseOr();
		Bool					isBitWiseXor();
		Bool					isBitWiseNot();
		Bool					isColon();
		Bool					isSemiColon();
		Bool					isLogicalAnd();
		Bool					isLogicalOr();
		Bool					isLogicalNot();
		Bool					isInterrogation();
		Bool					isComma();
		Bool					isDot();

		const String&			getTokenValue();
		const TokenType_e		getTokenType();
		const TokenSubType_e	getTokenSubType();
		const U32				getTokenLine();
		const U32				getTokenColumn();

	private:
		void					setPosition(U32 position);


	private:
		unique_ptr<Lexer>		m_lex;
		Token_s					m_tok;
		I32						m_typeModeLock;
	};
} }
