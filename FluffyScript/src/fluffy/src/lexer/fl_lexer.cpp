#include <cstring>
#include "lexer\fl_lexer.h"
#include "fl_exceptions.h"
#include "fl_buffer.h"

namespace fluffy {
	static constexpr const std::tuple<const I8*, TokenType_e> keywords[] = {
		std::make_tuple("include", 		TokenType_e::Include),		// ok
		std::make_tuple("export", 		TokenType_e::Export),		// ok
		std::make_tuple("namespace",	TokenType_e::Namespace),	// ok
		std::make_tuple("class", 		TokenType_e::Class),		// ok
		std::make_tuple("extends", 		TokenType_e::Extends),		// ok
		std::make_tuple("implements",	TokenType_e::Implements),	// ok
		std::make_tuple("where", 		TokenType_e::Where),		// ok
		std::make_tuple("constructor",	TokenType_e::Constructor),	// ok
		std::make_tuple("destructor",	TokenType_e::Destructor),	// ok
		std::make_tuple("super",		TokenType_e::Super),		// ok
		std::make_tuple("public", 		TokenType_e::Public),		// ok
		std::make_tuple("protected",	TokenType_e::Protected),	// ok
		std::make_tuple("private", 		TokenType_e::Private),		// ok
		std::make_tuple("final",		TokenType_e::Final),		// ok
		std::make_tuple("override", 	TokenType_e::Override),		// ok
		std::make_tuple("abstract", 	TokenType_e::Abstract),		// ok
		std::make_tuple("virtual", 		TokenType_e::Virtual),		// ok
		std::make_tuple("interface",	TokenType_e::Interface),	// ok
		std::make_tuple("struct",		TokenType_e::Struct),		// ok
		std::make_tuple("enum", 		TokenType_e::Enum),			// ok
		std::make_tuple("match", 		TokenType_e::Match),		// ok
		std::make_tuple("when", 		TokenType_e::When),			// ok
		std::make_tuple("trait", 		TokenType_e::Trait),		// ok
		std::make_tuple("static", 		TokenType_e::Static),		// ok
		std::make_tuple("const", 		TokenType_e::Const),		// ok
		std::make_tuple("true", 		TokenType_e::True),			// ok
		std::make_tuple("false", 		TokenType_e::False),		// ok
		std::make_tuple("void", 		TokenType_e::Void),			// ok
		std::make_tuple("bool", 		TokenType_e::Bool),			// ok
		std::make_tuple("i8", 			TokenType_e::I8),			// ok
		std::make_tuple("u8", 			TokenType_e::U8),			// ok
		std::make_tuple("i16", 			TokenType_e::I16),			// ok
		std::make_tuple("u16", 			TokenType_e::U16),			// ok
		std::make_tuple("i32", 			TokenType_e::I32),			// ok
		std::make_tuple("u32", 			TokenType_e::U32),			// ok
		std::make_tuple("i64", 			TokenType_e::I64),			// ok
		std::make_tuple("u64", 			TokenType_e::U64),			// ok
		std::make_tuple("fp32",			TokenType_e::Fp32),			// ok
		std::make_tuple("fp64",			TokenType_e::Fp64),			// ok
		std::make_tuple("string", 		TokenType_e::String),		// ok
		std::make_tuple("object", 		TokenType_e::Object),		// ok
		std::make_tuple("fn", 			TokenType_e::Fn),			// ok
		std::make_tuple("let", 			TokenType_e::Let),			// ok
		std::make_tuple("this", 		TokenType_e::This),			// ok
		std::make_tuple("shared", 		TokenType_e::Shared),		// ok
		std::make_tuple("unique", 		TokenType_e::Unique),		// ok
		std::make_tuple("ref", 			TokenType_e::Ref),			// ok
		std::make_tuple("self", 		TokenType_e::Self),			// ok
		std::make_tuple("new", 			TokenType_e::New),			// ok
		std::make_tuple("as", 			TokenType_e::As),			// ok
		std::make_tuple("is", 			TokenType_e::Is),			// ok
		std::make_tuple("goto", 		TokenType_e::Goto),			// ok
		std::make_tuple("if", 			TokenType_e::If),			// ok
		std::make_tuple("else", 		TokenType_e::Else),			// ok
		std::make_tuple("continue", 	TokenType_e::Continue),		// ok
		std::make_tuple("break", 		TokenType_e::Break),		// ok
		std::make_tuple("for", 			TokenType_e::For),			// ok
		std::make_tuple("in",			TokenType_e::In),			// ok
		std::make_tuple("while", 		TokenType_e::While),		// ok
		std::make_tuple("do", 			TokenType_e::Do),			// ok
		std::make_tuple("try", 			TokenType_e::Try),			// ok
		std::make_tuple("catch",		TokenType_e::Catch),		// ok
		std::make_tuple("finally",		TokenType_e::Finally),		// ok
		std::make_tuple("panic", 		TokenType_e::Panic),		// ok
		std::make_tuple("return", 		TokenType_e::Return),		// ok
		std::make_tuple("null", 		TokenType_e::Null)			// ok
	};

	// Symbol
	static Bool issymbol(I8 ch) {
		switch (ch) {
		case '=': case '+': case '-': case '*': case '/':
		case '%': case '!': case '>': case '<': case '&':
		case ':': case ';': case '|': case '~': case '^':
		case '[': case ']': case '(': case ')': case '{':
		case '}': case '.': case ',': case '?':
			return true;
		}
		return false;
	}

	// identifier
	static Bool isidentifier(const I8 ch) {
		return isalpha(ch) || ch == '_';
	}

	static Bool isnumber(const I8 ch) {
		return isdigit(ch);
	}

	// hex digit
	static Bool ishex(I8 ch) {
		return isdigit(ch) || (
			(ch == 'a' || ch == 'A') ||
			(ch == 'b' || ch == 'B') ||
			(ch == 'c' || ch == 'C') ||
			(ch == 'd' || ch == 'D') ||
			(ch == 'e' || ch == 'E') ||
			(ch == 'f' || ch == 'F')
			);
	}

	// bin digit
	static Bool isbin(I8 ch) {
		return ch == '0' || ch == '1';
	}

	static Bool ischar(I8 ch) {
		return ch == '\'';
	}

	static Bool isstring(I8 ch) {
		return ch == '\"';
	}

	static const auto isSpace = [](char ch) -> bool { return ch == ' '; };
	static const auto isNewLine = [](char ch) -> bool { return ch == '\n'; };
	static const auto isCarriage = [](char ch) -> bool { return ch == '\r'; };
	static const auto isTab = [](char ch) -> bool { return ch == '\t'; };
}

namespace fluffy { namespace lexer {
	Lexer::Lexer(BufferBase* const buffer)
		: m_buffer(buffer)
		, m_filename()
		, m_tabSpaces(4)
		, m_line(1)
		, m_column(1)
		, m_eof(false)
	{}

	Lexer::~Lexer()
	{}

	void
	Lexer::loadSource(const I8* sourceCode)
	{
		m_buffer->load(sourceCode, static_cast<U32>(strlen(sourceCode)));
		m_filename = "anom_block";
		parse();
	}

	void
	Lexer::loadSourceFromFile(const I8* sourceFilename)
	{
		m_buffer->loadFromFile(sourceFilename);
		m_filename = sourceFilename;
		parse();
	}

	const I8*
	Lexer::getFilename()
	{
		return m_filename.c_str();
	}

	void
	Lexer::nextToken()
	{
		parse();
	}

	void
	Lexer::setTabSpaces(U32 newTabSpaces)
	{
		m_tabSpaces = newTabSpaces;
	}

	const Token_s&
	Lexer::getToken()
	{
		return m_token;
	}

	void
	Lexer::resetToPosition(U32 newPosition, U32 newLine, U32 newColumn)
	{
		m_eof = false;
		m_line = newLine;
		m_column = newColumn;
		m_buffer->reset(newPosition);
		parse();
	}

	void
	Lexer::reinterpretToken(TokenType_e type, U32 offset)
	{
		m_eof = false;
		m_token.type = type;
		m_buffer->reset(m_token.position + offset);
	}

	Token_s
	Lexer::predictNextToken()
	{
		const U32 position = m_token.position;
		const U32 line = m_token.line;
		const U32 column = m_token.column;

		parse();
		const Token_s token = m_token;
		resetToPosition(position, line, column);
		return token;
	}

	void
	Lexer::expectToken(TokenType_e expectedToken)
	{
		if (m_token.type != expectedToken) {
			throw exceptions::custom_exception(
				"Expected token '%s', received '%s'",
				m_token.line,
				m_token.column,
				getTokenString(expectedToken),
				m_token.value.c_str()
			);
		}
		nextToken();
	}

	String
	Lexer::expectIdentifier()
	{
		String value = m_token.value;
		if (m_token.type != TokenType_e::Identifier) {
			throw exceptions::custom_exception(
				"Expected an identifier, received '%s'",
				m_token.line,
				m_token.column,
				m_token.value.c_str()
			);
		}
		nextToken();
		return value;
	}

	const Bool
	Lexer::expectConstantBool()
	{
		if (m_token.type != TokenType_e::True && m_token.type != TokenType_e::False) {
			throw exceptions::custom_exception(
				"Expected an boolean constant, received '%s'",
				m_token.line,
				m_token.column,
				m_token.value.c_str()
			);
		}
		const Bool value = m_token.type == TokenType_e::True ? true : false;
		nextToken();
		return value;
	}

	const I8
	Lexer::expectConstantInteger()
	{
		if (m_token.type != TokenType_e::ConstantInteger) {
			throw exceptions::custom_exception(
				"Expected an integer 8-bits constant, received '%s'",
				m_token.line,
				m_token.column,
				m_token.value.c_str()
			);
		}
		const I8 value = std::stoi(m_token.value, nullptr);
		nextToken();
		return value;
	}

	const Fp32
	Lexer::expectConstantFp32()
	{
		if (m_token.type != TokenType_e::ConstantFp32) {
			throw exceptions::custom_exception(
				"Expected a real 32-bits constant, received '%s'",
				m_token.line,
				m_token.column,
				m_token.value.c_str()
			);
		}
		const Fp32 value = std::stof(m_token.value, nullptr);
		nextToken();
		return value;
	}

	const Fp64
	Lexer::expectConstantFp64()
	{
		if (m_token.type != TokenType_e::ConstantFp64) {
			throw exceptions::custom_exception(
				"Expected a real 64-bits constant, received '%s'",
				m_token.line,
				m_token.column,
				m_token.value.c_str()
			);
		}
		const Fp64 value = std::stod(m_token.value, nullptr);
		nextToken();
		return value;
	}

	const I8
	Lexer::expectConstantChar()
	{
		if (m_token.type != TokenType_e::ConstantChar) {
			throw exceptions::custom_exception(
				"Expected a character constant, received '%s'",
				m_token.line,
				m_token.column,
				m_token.value.c_str()
			);
		}
		const I8 value = m_token.value[0];
		nextToken();
		return value;
	}

	String
	Lexer::expectConstantString()
	{
		if (m_token.type != TokenType_e::ConstantString) {
			throw exceptions::custom_exception(
				"Expected a string constant, received '%s'",
				m_token.line,
				m_token.column,
				m_token.value.c_str()
			);
		}
		String value = m_token.value;
		nextToken();
		return value;
	}

	Bool
	Lexer::is(TokenType_e m_tokenenType)
	{
		return m_token.type == m_tokenenType;
	}


	Bool
	Lexer::isEof()
	{
		return is(TokenType_e::Eof);
	}

	Bool
	Lexer::isIdentifier()
	{
		return is(TokenType_e::Identifier);
	}

	Bool
	Lexer::isConstantInteger()
	{
		return is(TokenType_e::ConstantInteger);
	}

	Bool
	Lexer::isConstantFp32()
	{
		return is(TokenType_e::ConstantFp32);
	}

	Bool
	Lexer::isConstantFp64()
	{
		return is(TokenType_e::ConstantFp64);
	}

	Bool
	Lexer::isConstantChar()
	{
		return is(TokenType_e::ConstantChar);
	}

	Bool
	Lexer::isConstantString()
	{
		return is(TokenType_e::ConstantString);
	}

	Bool
	Lexer::isConstantTrue()
	{
		return is(TokenType_e::True);
	}

	Bool
	Lexer::isConstantFalse()
	{
		return is(TokenType_e::False);
	}

	Bool
	Lexer::isInclude()
	{
		return is(TokenType_e::Include);
	}

	Bool
	Lexer::isExport()
	{
		return is(TokenType_e::Export);
	}

	Bool
	Lexer::isNamespace()
	{
		return is(TokenType_e::Namespace);
	}

	Bool
	Lexer::isClass()
	{
		return is(TokenType_e::Class);
	}

	Bool
	Lexer::isExtends()
	{
		return is(TokenType_e::Extends);
	}

	Bool
	Lexer::isWhere()
	{
		return is(TokenType_e::Where);
	}

	Bool
	Lexer::isImplements()
	{
		return is(TokenType_e::Implements);
	}

	Bool
	Lexer::isConstructor()
	{
		return is(TokenType_e::Constructor);
	}

	Bool
	Lexer::isDestructor()
	{
		return is(TokenType_e::Destructor);
	}

	Bool
	Lexer::isSuper()
	{
		return is(TokenType_e::Super);
	}

	Bool
	Lexer::isPublic()
	{
		return is(TokenType_e::Public);
	}

	Bool
	Lexer::isProtected()
	{
		return is(TokenType_e::Protected);
	}

	Bool
	Lexer::isPrivate()
	{
		return is(TokenType_e::Private);
	}

	Bool
	Lexer::isFinal()
	{
		return is(TokenType_e::Final);
	}

	Bool
	Lexer::isOverride()
	{
		return is(TokenType_e::Override);
	}

	Bool
	Lexer::isAbstract()
	{
		return is(TokenType_e::Abstract);
	}

	Bool
	Lexer::isVirtual()
	{
		return is(TokenType_e::Virtual);
	}

	Bool
	Lexer::isInterface()
	{
		return is(TokenType_e::Interface);
	}

	Bool
	Lexer::isStruct()
	{
		return is(TokenType_e::Struct);
	}

	Bool
	Lexer::isEnum()
	{
		return is(TokenType_e::Enum);
	}

	Bool
	Lexer::isMatch()
	{
		return is(TokenType_e::Match);
	}

	Bool
	Lexer::isWhen()
	{
		return is(TokenType_e::When);
	}

	Bool
	Lexer::isTrait()
	{
		return is(TokenType_e::Trait);
	}

	Bool
	Lexer::isStatic()
	{
		return is(TokenType_e::Static);
	}

	Bool
	Lexer::isConst()
	{
		return is(TokenType_e::Const);
	}

	Bool
	Lexer::isTrue()
	{
		return is(TokenType_e::True);
	}

	Bool
	Lexer::isFalse()
	{
		return is(TokenType_e::False);
	}

	Bool
	Lexer::isVoid()
	{
		return is(TokenType_e::Void);
	}

	Bool
	Lexer::isBool()
	{
		return is(TokenType_e::Bool);
	}

	Bool
	Lexer::isI8()
	{
		return is(TokenType_e::I8);
	}

	Bool
	Lexer::isU8()
	{
		return is(TokenType_e::U8);
	}

	Bool
	Lexer::isI16()
	{
		return is(TokenType_e::I16);
	}

	Bool
	Lexer::isU16()
	{
		return is(TokenType_e::U16);
	}

	Bool
	Lexer::isI32()
	{
		return is(TokenType_e::I32);
	}

	Bool
	Lexer::isU32()
	{
		return is(TokenType_e::U32);
	}

	Bool
	Lexer::isI64()
	{
		return is(TokenType_e::I64);
	}

	Bool
	Lexer::isU64()
	{
		return is(TokenType_e::U64);
	}

	Bool
	Lexer::isFp32()
	{
		return is(TokenType_e::Fp32);
	}

	Bool
	Lexer::isFp64()
	{
		return is(TokenType_e::Fp64);
	}

	Bool
	Lexer::isString()
	{
		return is(TokenType_e::String);
	}

	Bool
	Lexer::isObject()
	{
		return is(TokenType_e::Object);
	}

	Bool
	Lexer::isFn()
	{
		return is(TokenType_e::Fn);
	}

	Bool
	Lexer::isLet()
	{
		return is(TokenType_e::Let);
	}

	Bool
	Lexer::isThis()
	{
		return is(TokenType_e::This);
	}

	Bool
	Lexer::isShared()
	{
		return is(TokenType_e::Shared);
	}

	Bool
	Lexer::isUnique()
	{
		return is(TokenType_e::Unique);
	}

	Bool
	Lexer::isRef()
	{
		return is(TokenType_e::Ref);
	}

	Bool Lexer::isSelf()
	{
		return is(TokenType_e::Self);
	}

	Bool
	Lexer::isNew()
	{
		return is(TokenType_e::New);
	}

	Bool
	Lexer::isAs()
	{
		return is(TokenType_e::As);
	}

	Bool
	Lexer::isIs()
	{
		return is(TokenType_e::Is);
	}

	Bool
	Lexer::isGoto()
	{
		return is(TokenType_e::Goto);
	}

	Bool Lexer::isIf()
	{
		return is(TokenType_e::If);
	}

	Bool
	Lexer::isElse()
	{
		return is(TokenType_e::Else);
	}

	Bool
	Lexer::isContinue()
	{
		return is(TokenType_e::Continue);
	}

	Bool
	Lexer::isBreak()
	{
		return is(TokenType_e::Break);
	}

	Bool
	Lexer::isFor()
	{
		return is(TokenType_e::For);
	}

	Bool
	Lexer::isIn()
	{
		return is(TokenType_e::In);
	}

	Bool
	Lexer::isWhile()
	{
		return is(TokenType_e::While);
	}

	Bool
	Lexer::isDo()
	{
		return is(TokenType_e::Do);
	}

	Bool
	Lexer::isTry()
	{
		return is(TokenType_e::Try);
	}

	Bool
	Lexer::isCatch()
	{
		return is(TokenType_e::Catch);
	}

	Bool
	Lexer::isFinally()
	{
		return is(TokenType_e::Finally);
	}

	Bool
	Lexer::isPanic()
	{
		return is(TokenType_e::Panic);
	}

	Bool
	Lexer::isReturn()
	{
		return is(TokenType_e::Return);
	}

	Bool
	Lexer::isNull()
	{
		return is(TokenType_e::Null);
	}

	Bool
	Lexer::isScopeResolution()
	{
		return is(TokenType_e::ScopeResolution);
	}

	Bool
	Lexer::isIncrement()
	{
		return is(TokenType_e::Increment);
	}

	Bool
	Lexer::isDecrement()
	{
		return is(TokenType_e::Decrement);
	}

	Bool
	Lexer::isArrow()
	{
		return is(TokenType_e::Arrow);
	}

	Bool
	Lexer::isLeftParBracket()
	{
		return is(TokenType_e::LParBracket);
	}

	Bool
	Lexer::isRightParBracket()
	{
		return is(TokenType_e::RParBracket);
	}

	Bool
	Lexer::isLeftSquBracket()
	{
		return is(TokenType_e::LSquBracket);
	}

	Bool
	Lexer::isRightSquBracket()
	{
		return is(TokenType_e::RSquBracket);
	}

	Bool
	Lexer::isLeftBracket()
	{
		return is(TokenType_e::LBracket);
	}

	Bool
	Lexer::isRightBracket()
	{
		return is(TokenType_e::RBracket);
	}

	Bool
	Lexer::isPlus()
	{
		return is(TokenType_e::Plus);
	}

	Bool
	Lexer::isMinus()
	{
		return is(TokenType_e::Minus);
	}

	Bool
	Lexer::isDivision()
	{
		return is(TokenType_e::Division);
	}

	Bool
	Lexer::isMultiplication()
	{
		return is(TokenType_e::Multiplication);
	}

	Bool
	Lexer::isModulo()
	{
		return is(TokenType_e::Modulo);
	}

	Bool
	Lexer::isAssign()
	{
		return is(TokenType_e::Assign);
	}

	Bool
	Lexer::isPlusAssign()
	{
		return is(TokenType_e::PlusAssign);
	}

	Bool
	Lexer::isMinusAssign()
	{
		return is(TokenType_e::MinusAssign);
	}

	Bool
	Lexer::isMultAssign()
	{
		return is(TokenType_e::MultAssign);
	}

	Bool
	Lexer::isDivAssign()
	{
		return is(TokenType_e::DivAssign);
	}

	Bool
	Lexer::isBitWiseAndAssign()
	{
		return is(TokenType_e::BitWiseAndAssign);
	}

	Bool
	Lexer::isBitWiseOrAssign()
	{
		return is(TokenType_e::BitWiseOrAssign);
	}

	Bool
	Lexer::isBitWiseXorAssign()
	{
		return is(TokenType_e::BitWiseXorAssign);
	}

	Bool
	Lexer::isModAssign()
	{
		return is(TokenType_e::ModAssign);
	}

	Bool
	Lexer::isBitWiseLeftShiftAssign()
	{
		return is(TokenType_e::BitWiseLShiftAssign);
	}

	Bool
	Lexer::isBitWiseRightShiftAssign()
	{
		return is(TokenType_e::BitWiseRShiftAssign);
	}

	Bool
	Lexer::isGreaterThan()
	{
		return is(TokenType_e::GreaterThan);
	}

	Bool
	Lexer::isLessThan()
	{
		return is(TokenType_e::LessThan);
	}

	Bool
	Lexer::isGreaterThanOrEqual()
	{
		return is(TokenType_e::GreaterThanOrEqual);
	}

	Bool
	Lexer::isLessThanOrEqual()
	{
		return is(TokenType_e::LessThanOrEqual);
	}

	Bool
	Lexer::isEqual()
	{
		return is(TokenType_e::Equal);
	}

	Bool
	Lexer::isNotEqual()
	{
		return is(TokenType_e::NotEqual);
	}

	Bool
	Lexer::isBitWiseLeftShift()
	{
		return is(TokenType_e::BitWiseLShift);
	}

	Bool
	Lexer::isBitWiseRightShift()
	{
		return is(TokenType_e::BitWiseRShift);
	}

	Bool
	Lexer::isBitWiseAnd()
	{
		return is(TokenType_e::BitWiseAnd);
	}

	Bool
	Lexer::isBitWiseOr()
	{
		return is(TokenType_e::BitWiseOr);
	}

	Bool
	Lexer::isBitWiseXor()
	{
		return is(TokenType_e::BitWiseXor);
	}

	Bool
	Lexer::isBitWiseNot()
	{
		return is(TokenType_e::BitWiseNot);
	}

	Bool
	Lexer::isColon()
	{
		return is(TokenType_e::Colon);
	}

	Bool
	Lexer::isSemiColon()
	{
		return is(TokenType_e::SemiColon);
	}

	Bool
	Lexer::isLogicalAnd()
	{
		return is(TokenType_e::LogicalAnd);
	}

	Bool
	Lexer::isLogicalOr()
	{
		return is(TokenType_e::LogicalOr);
	}

	Bool
	Lexer::isLogicalNot()
	{
		return is(TokenType_e::LogicalNot);
	}

	Bool Lexer::isInterrogation()
	{
		return is(TokenType_e::Interrogation);
	}

	Bool
	Lexer::isComma()
	{
		return is(TokenType_e::Comma);
	}

	Bool
	Lexer::isDot()
	{
		return is(TokenType_e::Dot);
	}

	Bool
	Lexer::isSafeDot()
	{
		return is(TokenType_e::SafeDot);
	}

	Bool
	Lexer::isEllipsis()
	{
		return is(TokenType_e::Ellipsis);
	}

	void
	Lexer::parse()
	{
		// Salta: espacos, tabulacoes, carriages, novas linhas.
		skip();

		if (!m_eof) {
			const I8 ch = readChar();

			// Armazena inicio do m_tokenen e nome do arquivo ou bloco.
			m_token.type = TokenType_e::Unknown;
			m_token.line = m_line;
			m_token.column = m_column;
			m_token.position = m_buffer->getPosition();
			m_token.filename = m_filename;
			m_token.value.clear();

			// Processa m_tokenen.
			if (isidentifier(ch)) {
				parseIdentifier();
				return;
			}
			if (issymbol(ch)) {
				parseSymbols();
				return;
			}
			if (isnumber(ch)) {
				parseNumbers();
				return;
			}
			if (ischar(ch)) {
				parseChar();
				return;
			}
			if (isstring(ch)) {
				parseString();
				return;
			}
			throw exceptions::unexpected_token_exception(ch, m_token.line, m_token.column);
		}
		else
		{
			m_token.line = m_line;
			m_token.column = m_column;
			m_token.filename = m_filename;
			m_token.value = "<eof>";
			m_token.type = TokenType_e::Eof;
		}
	}

	I8
	Lexer::readChar(U32 offset)
	{
		return m_buffer->readByte(offset);
	}

	I8
	Lexer::readCharAndAdv()
	{
		const I8 ch = m_buffer->readByte();
		nextChar();
		return ch;
	}

	void
	Lexer::nextChar()
	{
		m_buffer->nextByte();
		const U8 ch = readChar();
		{
			if (ch == '\0') {
				m_eof = true;
			}
			if (isNewLine(ch)) {
				m_line++;
				m_column = 0;
				return;
			}
			if (isTab(ch)) {
				m_column += m_tabSpaces;
				return;
			}
		}
		m_column++;
	}

	void
	Lexer::skip()
	{
		while (true) {
			const I8 ch = readChar();

			// Sai se o primeiro for o fim do arquivo.
			if (ch == '\0') {
				m_eof = true;
				return;
			}

			const I8 ch2 = readChar(1);

			// Comentario de linha
			if (ch == '/' && ch2 == '/') {
				while (true) {
					const I8 nch = readChar();
					if (nch == '\n') {
						break;
					}
					if (m_eof) {
						return;
					}
					nextChar();
				}
				nextChar();
				continue;
			}

			// Comentario de bloco
			if (ch == '/' && ch2 == '*') {
				while (true) {
					const I8 nch = readChar();

					if (m_eof) {
						throw exceptions::unexpected_end_of_file_exception();
					}

					const I8 nch2 = readChar(1);

					if (nch == '*' && nch2 == '/') {
						break;
					}
					nextChar();
				}
				nextChar();
				nextChar();
				continue;
			}

			// Espacos em branco, tabulacao, nova linha, carriage.
			if (isSpace(ch) || isNewLine(ch) || isCarriage(ch) || isTab(ch)) {
				nextChar();
				continue;
			}
			break;
		}
	}

	void
	Lexer::parseIdentifier()
	{
		while (true)
		{
			const U8 ch = readChar();
			if (!isidentifier(ch) && !isdigit(ch)) {
				break;
			}
			m_token.value.push_back(readCharAndAdv());
		}

		// Verifica se o m_tokenen e uma palavra reservada.
		for (auto& keyword : keywords) {
			if (std::get<0>(keyword) == m_token.value) {
				m_token.type = std::get<1>(keyword);
				return;
			}
		}
		m_token.type = TokenType_e::Identifier;
	}

	void
	Lexer::parseSymbols()
	{
		const I8 ch = readChar();

		// Process symbol
		switch (ch)
		{
		case '>':
			{
				m_token.type = TokenType_e::GreaterThan;
				m_token.value.push_back(readCharAndAdv());

				if (readChar() == '=') {
					m_token.type = TokenType_e::GreaterThanOrEqual;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
				if (readChar() == '>') {
					m_token.type = TokenType_e::BitWiseRShift;
					m_token.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						m_token.type = TokenType_e::BitWiseRShiftAssign;
						m_token.value.push_back(readCharAndAdv());
						return;
					}
					return;
				}
			}
			break;
		case '<':
			{
				m_token.type = TokenType_e::LessThan;
				m_token.value.push_back(readCharAndAdv());

				if (readChar() == '=') {
					m_token.type = TokenType_e::LessThanOrEqual;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
				if (readChar() == '<') {
					m_token.type = TokenType_e::BitWiseLShift;
					m_token.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						m_token.type = TokenType_e::BitWiseLShiftAssign;
						m_token.value.push_back(readCharAndAdv());
						return;
					}
					return;
				}
			}
			break;
		case ':':
			{
				m_token.type = TokenType_e::Colon;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == ':') {
					m_token.type = TokenType_e::ScopeResolution;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '+':
			{
				m_token.type = TokenType_e::Plus;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '+') {
					m_token.type = TokenType_e::Increment;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
				if (readChar() == '=') {
					m_token.type = TokenType_e::PlusAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '-':
			{
				m_token.type = TokenType_e::Minus;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '-') {
					m_token.type = TokenType_e::Decrement;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
				if (readChar() == '=') {
					m_token.type = TokenType_e::MinusAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
				if (readChar() == '>') {
					m_token.type = TokenType_e::Arrow;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '=':
			{
				m_token.type = TokenType_e::Assign;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '=') {
					m_token.type = TokenType_e::Equal;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '!':
			{
				m_token.type = TokenType_e::LogicalNot;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '=') {
					m_token.type = TokenType_e::NotEqual;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '&':
			{
				m_token.type = TokenType_e::BitWiseAnd;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '&') {
					m_token.type = TokenType_e::LogicalAnd;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
				if (readChar() == '=') {
					m_token.type = TokenType_e::BitWiseAndAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '|':
			{
				m_token.type = TokenType_e::BitWiseOr;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '|') {
					m_token.type = TokenType_e::LogicalOr;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
				if (readChar() == '=') {
					m_token.type = TokenType_e::BitWiseOrAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '.':
			{
				m_token.type = TokenType_e::Dot;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '.') {
					m_token.value.push_back(readCharAndAdv());
					if (readChar() == '.') {
						m_token.type = TokenType_e::Ellipsis;
						m_token.value.push_back(readCharAndAdv());
						return;
					}
					throw exceptions::unexpected_token_exception(m_token.value, m_token.line, m_token.column);
				}
			}
			break;
		case '(':
			{
				m_token.type = TokenType_e::LParBracket;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case ')':
			{
				m_token.type = TokenType_e::RParBracket;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case '[':
			{
				m_token.type = TokenType_e::LSquBracket;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case ']':
			{
				m_token.type = TokenType_e::RSquBracket;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case '{':
			{
				m_token.type = TokenType_e::LBracket;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case '}':
			{
				m_token.type = TokenType_e::RBracket;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case '/':
			{
				m_token.type = TokenType_e::Division;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '=') {
					m_token.type = TokenType_e::DivAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '*':
			{
				m_token.type = TokenType_e::Multiplication;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '=') {
					m_token.type = TokenType_e::MultAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '%':
			{
				m_token.type = TokenType_e::Modulo;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '=') {
					m_token.type = TokenType_e::ModAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case '~':
			{
				m_token.type = TokenType_e::BitWiseNot;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case '^':
			{
				m_token.type = TokenType_e::BitWiseXor;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '=') {
					m_token.type = TokenType_e::BitWiseXorAssign;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		case ';':
			{
				m_token.type = TokenType_e::SemiColon;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case ',':
			{
				m_token.type = TokenType_e::Comma;
				m_token.value.push_back(readCharAndAdv());
			}
			break;
		case '?':
			{
				m_token.type = TokenType_e::Interrogation;
				m_token.value.push_back(readCharAndAdv());
				if (readChar() == '.') {
					m_token.type = TokenType_e::SafeDot;
					m_token.value.push_back(readCharAndAdv());
					return;
				}
			}
			break;
		default:
			throw exceptions::unexpected_token_exception(m_token.value, m_token.line, m_token.column);
		}
	}

	void
	Lexer::parseNumbers()
	{
		I8 ch = readChar();
		I8 ch2 = readChar(1);

		// Processa hexadecimal.
		if (ch == '0' && (ch2 == 'x' || ch2 == 'X'))
		{
			nextChar(); // Consome 0
			nextChar(); // Consome x ou X

			m_token.type = TokenType_e::ConstantHex;

			bool isValid = false;
			while (true)
			{
				ch = readChar();
				if (!ishex(ch)) {
					if (!isValid) {
						throw exceptions::malformed_number_exception(m_token.line, m_token.column);
					}
					return;
				}
				isValid = true;
				m_token.value.push_back(ch);
				nextChar();
			}
			return;
		}

		// Processa binario.
		if (readChar() == '0' && (readChar(1) == 'b' || readChar(1) == 'B'))
		{
			nextChar(); // Consome 0
			nextChar(); // Consome b ou B

			m_token.type = TokenType_e::ConstantBin;

			Bool isValid = false;
			while (true)
			{
				ch = readChar();
				if (!isbin(ch)) {
					if (!isValid) {
						throw exceptions::malformed_number_exception(m_token.line, m_token.column);
					}
					return;
				}
				isValid = true;
				m_token.value.push_back(ch);
				nextChar();
			}
			return;
		}

		// Inteiro ou real.
		Bool isReal = false;
		while (true)
		{
			ch = readChar();

			if (!isnumber(ch) && ch != '.')
			{
				break;
			}
			if (ch == '.') {
				if (isReal) {
					m_token.type = TokenType_e::ConstantFp64;
					return;
				}
				isReal = true;
			}
			m_token.value.push_back(ch);
			nextChar();
		}

		if (isReal) {
			ch = readChar();

			if (ch == 'f' || ch == 'F') {
				m_token.type = TokenType_e::ConstantFp32;
				nextChar();
			}
			else {
				m_token.type = TokenType_e::ConstantFp64;
			}
			return;
		}		

		// Precisao padrao: inteiro 32bits sinalizado.
		m_token.type = TokenType_e::ConstantInteger;
	}

	void
	Lexer::parseChar()
	{
		m_token.type = TokenType_e::ConstantChar;

		nextChar(); // Consome '
		if (readChar() == '\\') {
			m_token.value.push_back(readCharAndAdv());
			m_token.value.push_back(readCharAndAdv());
		}
		else if (readChar() != '\'') {
			m_token.value.push_back(readCharAndAdv());
		}
		nextChar(); // Consome '
	}

	void
	Lexer::parseString()
	{
		m_token.type = TokenType_e::ConstantString;

		// Verifica se e uma string com multiplas linhas.
		nextChar(); // Consome "
		if (readChar() == '\"')
		{
			nextChar(); // Consome "
			if (readChar() == '\"')
			{
				nextChar(); // Consome "
				while (true)
				{
					const I8 ch = readChar();

					if (ch == '\0') {
						throw exceptions::malformed_string_constant_exception(m_token.line, m_token.column);
					}
					if (ch == '\"') {
						nextChar(); // Consome "
						if (readChar() == '\"')
						{
							nextChar(); // Consome "
							if (readChar() == '\"')
							{
								nextChar(); // Consome "
								return;
							}
							m_token.value.push_back(readCharAndAdv());
						}
						m_token.value.push_back(readCharAndAdv());
					}
					m_token.value.push_back(readCharAndAdv());
				}
			}
			return; // Quanto reconhece "", ou seja a string esta vazia.
		}

		while (true)
		{
			const I8 ch = readChar();

			if (ch == '\\')
			{
				m_token.value.push_back(readCharAndAdv());
			}
			if (ch == '\n' || ch == '\0') {
				throw exceptions::malformed_string_constant_exception(m_token.line, m_token.column);
			}
			if (ch == '\"') {
				break;
			}
			m_token.value.push_back(readCharAndAdv());
		}
		nextChar(); // Consome "
	}
} }