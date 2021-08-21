#include <tuple>
#include <algorithm>

#include "fl_lexer.h"
#include "..\fl_buffer.h"
#include "..\fl_exceptions.h"

namespace fluffy {
	static const std::tuple<const I8*, TokenSubType_e> keywords[] = {
		std::make_tuple("include", 		TokenSubType_e::Include),		// ok
		std::make_tuple("from", 		TokenSubType_e::From),			// ok
		std::make_tuple("export", 		TokenSubType_e::Export),		// ok
		std::make_tuple("namespace",	TokenSubType_e::Namespace),		// ok
		std::make_tuple("class", 		TokenSubType_e::Class),			// ok
		std::make_tuple("extends", 		TokenSubType_e::Extends),		// ok
		std::make_tuple("implements",	TokenSubType_e::Implements),	// ok
		std::make_tuple("where", 		TokenSubType_e::Where),			// ok
		std::make_tuple("constructor",	TokenSubType_e::Constructor),	// ok
		std::make_tuple("destructor",	TokenSubType_e::Destructor),	// ok
		std::make_tuple("super",		TokenSubType_e::Super),			// ok
		std::make_tuple("public", 		TokenSubType_e::Public),		// ok
		std::make_tuple("protected",	TokenSubType_e::Protected),		// ok
		std::make_tuple("private", 		TokenSubType_e::Private),		// ok
		std::make_tuple("final",		TokenSubType_e::Final),			// ok
		std::make_tuple("override", 	TokenSubType_e::Override),		// ok
		std::make_tuple("abstract", 	TokenSubType_e::Abstract),		// ok
		std::make_tuple("virtual", 		TokenSubType_e::Virtual),		// ok
		std::make_tuple("interface",	TokenSubType_e::Interface),		// ok
		std::make_tuple("struct",		TokenSubType_e::Struct),		// ok
		std::make_tuple("enum", 		TokenSubType_e::Enum),			// ok
		std::make_tuple("match", 		TokenSubType_e::Match),			// ok
		std::make_tuple("when", 		TokenSubType_e::When),			// ok
		std::make_tuple("trait", 		TokenSubType_e::Trait),			// ok
		std::make_tuple("static", 		TokenSubType_e::Static),		// ok
		std::make_tuple("const", 		TokenSubType_e::Const),			// ok
		std::make_tuple("true", 		TokenSubType_e::True),			// ok
		std::make_tuple("false", 		TokenSubType_e::False),			// ok
		std::make_tuple("void", 		TokenSubType_e::Void),			// ok
		std::make_tuple("bool", 		TokenSubType_e::Bool),			// ok
		std::make_tuple("i8", 			TokenSubType_e::I8),			// ok
		std::make_tuple("u8", 			TokenSubType_e::U8),			// ok
		std::make_tuple("i16", 			TokenSubType_e::I16),			// ok
		std::make_tuple("u16", 			TokenSubType_e::U16),			// ok
		std::make_tuple("i32", 			TokenSubType_e::I32),			// ok
		std::make_tuple("u32", 			TokenSubType_e::U32),			// ok
		std::make_tuple("i64", 			TokenSubType_e::I64),			// ok
		std::make_tuple("u64", 			TokenSubType_e::U64),			// ok
		std::make_tuple("fp32",			TokenSubType_e::Fp32),			// ok
		std::make_tuple("fp64",			TokenSubType_e::Fp64),			// ok
		std::make_tuple("string", 		TokenSubType_e::String),		// ok
		std::make_tuple("vector",		TokenSubType_e::Vector),		// ok
		std::make_tuple("set", 			TokenSubType_e::Set),			// ok
		std::make_tuple("map", 			TokenSubType_e::Map),			// ok
		std::make_tuple("object", 		TokenSubType_e::Object),		// ok
		std::make_tuple("fn", 			TokenSubType_e::Fn),			// ok
		std::make_tuple("let", 			TokenSubType_e::Let),			// ok
		std::make_tuple("this", 		TokenSubType_e::This),			// ok
		std::make_tuple("ref", 			TokenSubType_e::Ref),			// ok
		std::make_tuple("self", 		TokenSubType_e::Self),			// ok
		std::make_tuple("new", 			TokenSubType_e::New),			// ok
		std::make_tuple("as", 			TokenSubType_e::As),			// ok
		std::make_tuple("is", 			TokenSubType_e::Is),			// ok
		std::make_tuple("goto", 		TokenSubType_e::Goto),			// ok
		std::make_tuple("if", 			TokenSubType_e::If),			// ok
		std::make_tuple("else", 		TokenSubType_e::Else),			// ok
		std::make_tuple("continue", 	TokenSubType_e::Continue),		// ok
		std::make_tuple("break", 		TokenSubType_e::Break),			// ok
		std::make_tuple("for", 			TokenSubType_e::For),			// ok
		std::make_tuple("in",			TokenSubType_e::In),			// ok
		std::make_tuple("while", 		TokenSubType_e::While),			// ok
		std::make_tuple("do", 			TokenSubType_e::Do),			// ok
		std::make_tuple("try", 			TokenSubType_e::Try),			// ok
		std::make_tuple("catch",		TokenSubType_e::Catch),			// ok
		std::make_tuple("finally",		TokenSubType_e::Finally),		// ok
		std::make_tuple("panic", 		TokenSubType_e::Panic),			// ok
		std::make_tuple("return", 		TokenSubType_e::Return),		// ok
		std::make_tuple("null", 		TokenSubType_e::Null)			// ok
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

namespace fluffy {
	namespace lexer {
		Lexer::Lexer(BufferBase* const buffer)
			: m_buffer(buffer)
			, m_line(1)
			, m_column(1)
			, m_tabSpaces(4)
			, m_eof(false)
		{}

		Lexer::~Lexer()
		{}

		void Lexer::loadSource(String source)
		{
			m_buffer->load(source.c_str(), static_cast<U32>(source.length()));
			m_filename = "anom_block";
		}

		void Lexer::loadFromSource(String sourceFile)
		{
			m_buffer->loadFromFile(sourceFile.c_str());
			m_filename = sourceFile;
		}

		void Lexer::parse(Token_s& tok)
		{
			// Salta: espacos, tabulacoes, carriages, novas linhas.
			skip();

			if (!m_eof) {
				const I8 ch = readChar();

				// Armazena inicio do token e nome do arquivo ou bloco.
				tok.type = TokenType_e::Unknown;
				tok.subType = TokenSubType_e::Unknown;
				tok.line = m_line;
				tok.column = m_column;
				tok.position = m_buffer->getPosition();
				tok.filename = m_filename;
				tok.value.clear();

				// Processa token.
				if (isidentifier(ch)) {
					parseIdentifier(tok);
					return;
				}
				if (issymbol(ch)) {
					parseSymbols(tok);
					return;
				}
				if (isnumber(ch)) {
					parseNumbers(tok);
					return;
				}
				if (ischar(ch)) {
					parseChar(tok);
					return;
				}
				if (isstring(ch)) {
					parseString(tok);
					return;
				}
				throw exceptions::unexpected_token_exception(ch, tok.line, tok.column);
			}
			else
			{
				tok.line = m_line;
				tok.column = m_column;
				tok.filename = m_filename;
				tok.value = "<eof>";
				tok.type = TokenType_e::Eof;
				tok.subType = TokenSubType_e::Unknown;
			}
		}

		void Lexer::setTabSpaces(U32 newTabSpaces)
		{
			m_tabSpaces = newTabSpaces;
		}

		const I8* Lexer::getTokenString(TokenSubType_e tokenSubType)
		{
			// Busca nas palavras chaves.
			for (auto& token : keywords)
			{
				if (std::get<1>(token) == tokenSubType)
				{
					return std::get<0>(token);
				}
			}

			// Busca nos simbolos.
			switch (tokenSubType)
			{
				case TokenSubType_e::ScopeResolution: 			return "::";
				case TokenSubType_e::Increment:					return "++";
				case TokenSubType_e::Decrement:					return "--";
				case TokenSubType_e::Arrow:						return "->";
				case TokenSubType_e::LParBracket:				return "(";
				case TokenSubType_e::RParBracket:				return ")";
				case TokenSubType_e::LSquBracket:				return "[";
				case TokenSubType_e::RSquBracket:				return "]";
				case TokenSubType_e::LBracket:					return "{";
				case TokenSubType_e::RBracket:					return "}";
				case TokenSubType_e::Plus:						return "+";
				case TokenSubType_e::Minus:						return "-";
				case TokenSubType_e::Division:					return "/";
				case TokenSubType_e::Multiplication:			return "*";
				case TokenSubType_e::Modulo:					return "%";
				case TokenSubType_e::Assign:					return "=";
				case TokenSubType_e::PlusAssign:				return "+=";
				case TokenSubType_e::MinusAssign:				return "-=";
				case TokenSubType_e::DivAssign:					return "/=";
				case TokenSubType_e::MultAssign:				return "*=";
				case TokenSubType_e::BitWiseAndAssign:			return "&=";
				case TokenSubType_e::BitWiseOrAssign:			return "|=";
				case TokenSubType_e::BitWiseXorAssign:			return "^=";
				case TokenSubType_e::ModAssign:					return "%=";
				case TokenSubType_e::BitWiseLShiftAssign:		return "<<=";
				case TokenSubType_e::BitWiseRShiftAssign:		return ">>=";
				case TokenSubType_e::GreaterThan:				return ">";
				case TokenSubType_e::LessThan:					return "<";
				case TokenSubType_e::GreaterThanOrEqual:		return ">=";
				case TokenSubType_e::LessThanOrEqual:			return "<=";
				case TokenSubType_e::Equal:						return "==";
				case TokenSubType_e::NotEqual:					return "!=";
				case TokenSubType_e::BitWiseLShift:				return "<<";
				case TokenSubType_e::BitWiseRShift:				return ">>";
				case TokenSubType_e::BitWiseAnd:				return "&";
				case TokenSubType_e::BitWiseOr:					return "|";
				case TokenSubType_e::BitWiseXor:				return "^";
				case TokenSubType_e::BitWiseNot:				return "~";
				case TokenSubType_e::Colon:						return ":";
				case TokenSubType_e::SemiColon:					return ";";
				case TokenSubType_e::LogicalAnd:				return "&&";
				case TokenSubType_e::LogicalOr:					return "||";
				case TokenSubType_e::LogicalNot:				return "!";
				case TokenSubType_e::Interrogation:				return "?";
				case TokenSubType_e::Comma:						return ":";
				case TokenSubType_e::Dot:						return ".";
				case TokenSubType_e::ConstantI8:				return "i8 constant";
				case TokenSubType_e::ConstantU8:				return "u8 constant";
				case TokenSubType_e::ConstantI16:				return "i16 constant";
				case TokenSubType_e::ConstantU16:				return "u16 constant";
				case TokenSubType_e::ConstantI32:				return "i32 constant";
				case TokenSubType_e::ConstantU32:				return "u32 constant";
				case TokenSubType_e::ConstantI64:				return "i64 constant";
				case TokenSubType_e::ConstantU64:				return "u64 constant";
				case TokenSubType_e::ConstantFp32:				return "fp32 constant";
				case TokenSubType_e::ConstantFp64:				return "fp64 constant";
				case TokenSubType_e::ConstantBin:				return "binary constant";
				case TokenSubType_e::ConstantHex:				return "hex constant";
				case TokenSubType_e::ConstantChar:				return "char constant";
				case TokenSubType_e::ConstantString:			return "string constant";
				default:
					return "Unknown token";
			}
		}

		U32 Lexer::getPosition()
		{
			return m_buffer->getPosition();
		}

		void Lexer::setPosition(U32 position)
		{
			m_buffer->reset(position);
		}

		I8 Lexer::readChar(U32 offset)
		{
			return m_buffer->readByte(offset);
		}

		I8 Lexer::readCharAndAdv()
		{
			const I8 ch = readChar();
			nextChar();
			return ch;
		}

		void Lexer::nextChar()
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

		void Lexer::skip()
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

		void Lexer::parseIdentifier(Token_s& tok)
		{
			while (true)
			{
				const U8 ch = readChar();
				if (!isidentifier(ch) && !isdigit(ch)) {
					break;
				}
				tok.value.push_back(readCharAndAdv());
			}

			// Verifica se o token e uma palavra reservada.
			for (auto& keyword : keywords) {
				if (std::get<0>(keyword) == tok.value) {
					tok.type = TokenType_e::Keyword;
					tok.subType = std::get<1>(keyword);
					return;
				}
			}
			tok.type = TokenType_e::Identifier;
			tok.subType = TokenSubType_e::Unknown;
		}

		void Lexer::parseSymbols(Token_s& tok)
		{
			const I8 ch = readChar();

			tok.type = TokenType_e::Symbol;

			// Process symbol
			switch (ch)
			{
			case '>':
				{
					tok.subType = TokenSubType_e::GreaterThan;
					tok.value.push_back(readCharAndAdv());

					if (readChar() == '=') {
						tok.subType = TokenSubType_e::GreaterThanOrEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '>') {
						tok.subType = TokenSubType_e::BitWiseRShift;
						tok.value.push_back(readCharAndAdv());
						if (readChar() == '=') {
							tok.subType = TokenSubType_e::BitWiseRShiftAssign;
							tok.value.push_back(readCharAndAdv());
							return;
						}
						return;
					}
				}
				break;
			case '<':
				{
					tok.subType = TokenSubType_e::LessThan;
					tok.value.push_back(readCharAndAdv());

					if (readChar() == '=') {
						tok.subType = TokenSubType_e::LessThanOrEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '<') {
						tok.subType = TokenSubType_e::BitWiseLShift;
						tok.value.push_back(readCharAndAdv());
						if (readChar() == '=') {
							tok.subType = TokenSubType_e::BitWiseLShiftAssign;
							tok.value.push_back(readCharAndAdv());
							return;
						}
						return;
					}
				}
				break;
			case ':':
				{
					tok.subType = TokenSubType_e::Colon;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == ':') {
						tok.subType = TokenSubType_e::ScopeResolution;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '+':
				{
					tok.subType = TokenSubType_e::Plus;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '+') {
						tok.subType = TokenSubType_e::Increment;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::PlusAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '-':
				{
					tok.subType = TokenSubType_e::Minus;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '-') {
						tok.subType = TokenSubType_e::Decrement;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::MinusAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '>') {
						tok.subType = TokenSubType_e::Arrow;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '=':
				{
					tok.subType = TokenSubType_e::Assign;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::Equal;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '!':
				{
					tok.subType = TokenSubType_e::LogicalNot;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::NotEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '&':
				{
					tok.subType = TokenSubType_e::BitWiseAnd;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '&') {
						tok.subType = TokenSubType_e::LogicalAnd;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::BitWiseAndAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '|':
				{
					tok.subType = TokenSubType_e::BitWiseOr;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '|') {
						tok.subType = TokenSubType_e::LogicalOr;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::BitWiseOrAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '.':
				{
					tok.subType = TokenSubType_e::Dot;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '(':
				{
					tok.subType = TokenSubType_e::LParBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ')':
				{
					tok.subType = TokenSubType_e::RParBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '[':
				{
					tok.subType = TokenSubType_e::LSquBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ']':
				{
					tok.subType = TokenSubType_e::RSquBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '{':
				{
					tok.subType = TokenSubType_e::LBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '}':
				{
					tok.subType = TokenSubType_e::RBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '/':
				{
					tok.subType = TokenSubType_e::Division;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::DivAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '*':
				{
					tok.subType = TokenSubType_e::Multiplication;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::MultAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '%':
				{
					tok.subType = TokenSubType_e::Modulo;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::ModAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '~':
				{
					tok.subType = TokenSubType_e::BitWiseNot;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '^':
				{
					tok.subType = TokenSubType_e::BitWiseXor;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::BitWiseXorAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case ';':
				{
					tok.subType = TokenSubType_e::SemiColon;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ',':
				{
					tok.subType = TokenSubType_e::Comma;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '?':
				{
					tok.subType = TokenSubType_e::Interrogation;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			default:
				throw exceptions::unexpected_token_exception(readChar(), tok.line, tok.column);
			}
		}

		void Lexer::parseNumbers(Token_s& tok)
		{
			I8 ch = readChar();
			I8 ch2 = readChar(1);

			tok.type = TokenType_e::Constant;

			// Processa hexadecimal.
			if (ch == '0' && (ch2 == 'x' || ch2 == 'X'))
			{
				nextChar(); // Consome 0
				nextChar(); // Consome x ou X

				tok.subType = TokenSubType_e::ConstantHex;

				bool isValid = false;
				while (true)
				{
					ch = readChar();
					if (!ishex(ch)) {
						if (!isValid) {
							throw exceptions::malformed_number_exception(tok.line, tok.column);
						}
						return;
					}
					isValid = true;
					tok.value.push_back(ch);
					nextChar();
				}
				return;
			}

			// Processa binario.
			if (readChar() == '0' && (readChar(1) == 'b' || readChar(1) == 'B'))
			{
				nextChar(); // Consome 0
				nextChar(); // Consome b ou B

				tok.subType = TokenSubType_e::ConstantBin;

				Bool isValid = false;
				while (true)
				{
					ch = readChar();
					if (!isbin(ch)) {
						if (!isValid) {
							throw exceptions::malformed_number_exception(tok.line, tok.column);
						}
						return;
					}
					isValid = true;
					tok.value.push_back(ch);
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
						tok.subType = TokenSubType_e::ConstantFp64;
						return;
					}
					isReal = true;
				}
				tok.value.push_back(ch);
				nextChar();
			}

			if (isReal) {
				ch = readChar();

				if (ch == 'f' || ch == 'F') {
					tok.subType = TokenSubType_e::ConstantFp32;
					nextChar();
				}
				else {
					tok.subType = TokenSubType_e::ConstantFp64;
				}
				return;
			}
			else {
				ch = readChar();

				// Processa pos fixo.
				if (ch == 'i' || ch == 'I')
				{
					nextChar();
					ch = readChar();

					if (ch == '8') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantI8;
						return;
					}
					nextChar();
					ch2 = readChar();

					if (ch == '1' && ch2 == '6') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantI16;
						return;
					}
					if (ch == '3' && ch2 == '2') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantI32;
						return;
					}
					if (ch == '6' && ch2 == '4') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantI64;
						return;
					}
					throw exceptions::malformed_number_exception(tok.line, tok.column);
				}

				// Processa pos fixo.
				if (ch == 'u' || ch == 'U')
				{
					nextChar();
					ch = readChar();

					if (ch == '8') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantU8;
						return;
					}
					nextChar();
					ch2 = readChar();

					if (ch == '1' && ch2 == '6') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantU16;
						return;
					}
					if (ch == '3' && ch2 == '2') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantU32;
						return;
					}
					if (ch == '6' && ch2 == '4') {
						nextChar();
						tok.subType = TokenSubType_e::ConstantU64;
						return;
					}
					throw exceptions::malformed_number_exception(tok.line, tok.column);
				}

				// Precisao padrao: inteiro 32bits sinalizado.
				tok.subType = TokenSubType_e::ConstantI32;
				return;
			}
		}

		void Lexer::parseChar(Token_s& tok)
		{
			tok.type = TokenType_e::Constant;
			tok.subType = TokenSubType_e::ConstantChar;

			nextChar(); // Consome '
			tok.value.push_back(readCharAndAdv());
			if (!ischar(readChar())) {
				throw exceptions::malformed_character_constant_exception(tok.line, tok.column);
			}
			nextChar(); // Consome '
		}

		void Lexer::parseString(Token_s& tok)
		{
			tok.type = TokenType_e::Constant;
			tok.subType = TokenSubType_e::ConstantString;

			nextChar(); // Consome "
			while (true)
			{
				const I8 ch = readChar();

				if (ch == '\n' || ch == '\0') {
					throw exceptions::malformed_string_constant_exception(tok.line, tok.column);
				}
				if (ch == '\"') {
					break;
				}
				tok.value.push_back(readCharAndAdv());
			}
			nextChar(); // Consome "
		}
	}
}