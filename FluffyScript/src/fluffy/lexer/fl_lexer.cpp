#include <tuple>
#include <algorithm>

#include "fl_lexer.h"
#include "..\fl_buffer.h"
#include "..\fl_exceptions.h"

namespace fluffy {
	static const std::tuple<const I8*, TokenType_e> keywords[] = {
		std::make_tuple("include", 		TokenType_e::Include),		// ok
		std::make_tuple("from", 		TokenType_e::From),			// ok
		std::make_tuple("export", 		TokenType_e::Export),		// ok
		std::make_tuple("namespace",	TokenType_e::Namespace),		// ok
		std::make_tuple("class", 		TokenType_e::Class),			// ok
		std::make_tuple("extends", 		TokenType_e::Extends),		// ok
		std::make_tuple("implements",	TokenType_e::Implements),	// ok
		std::make_tuple("where", 		TokenType_e::Where),			// ok
		std::make_tuple("constructor",	TokenType_e::Constructor),	// ok
		std::make_tuple("destructor",	TokenType_e::Destructor),	// ok
		std::make_tuple("super",		TokenType_e::Super),			// ok
		std::make_tuple("public", 		TokenType_e::Public),		// ok
		std::make_tuple("protected",	TokenType_e::Protected),		// ok
		std::make_tuple("private", 		TokenType_e::Private),		// ok
		std::make_tuple("final",		TokenType_e::Final),			// ok
		std::make_tuple("override", 	TokenType_e::Override),		// ok
		std::make_tuple("abstract", 	TokenType_e::Abstract),		// ok
		std::make_tuple("virtual", 		TokenType_e::Virtual),		// ok
		std::make_tuple("interface",	TokenType_e::Interface),		// ok
		std::make_tuple("struct",		TokenType_e::Struct),		// ok
		std::make_tuple("enum", 		TokenType_e::Enum),			// ok
		std::make_tuple("match", 		TokenType_e::Match),			// ok
		std::make_tuple("when", 		TokenType_e::When),			// ok
		std::make_tuple("trait", 		TokenType_e::Trait),			// ok
		std::make_tuple("static", 		TokenType_e::Static),		// ok
		std::make_tuple("const", 		TokenType_e::Const),			// ok
		std::make_tuple("true", 		TokenType_e::True),			// ok
		std::make_tuple("false", 		TokenType_e::False),			// ok
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
		std::make_tuple("ref", 			TokenType_e::Ref),			// ok
		std::make_tuple("self", 		TokenType_e::Self),			// ok
		std::make_tuple("new", 			TokenType_e::New),			// ok
		std::make_tuple("as", 			TokenType_e::As),			// ok
		std::make_tuple("is", 			TokenType_e::Is),			// ok
		std::make_tuple("goto", 		TokenType_e::Goto),			// ok
		std::make_tuple("if", 			TokenType_e::If),			// ok
		std::make_tuple("else", 		TokenType_e::Else),			// ok
		std::make_tuple("continue", 	TokenType_e::Continue),		// ok
		std::make_tuple("break", 		TokenType_e::Break),			// ok
		std::make_tuple("for", 			TokenType_e::For),			// ok
		std::make_tuple("in",			TokenType_e::In),			// ok
		std::make_tuple("while", 		TokenType_e::While),			// ok
		std::make_tuple("do", 			TokenType_e::Do),			// ok
		std::make_tuple("try", 			TokenType_e::Try),			// ok
		std::make_tuple("catch",		TokenType_e::Catch),			// ok
		std::make_tuple("finally",		TokenType_e::Finally),		// ok
		std::make_tuple("panic", 		TokenType_e::Panic),			// ok
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
			}
		}

		void Lexer::setTabSpaces(U32 newTabSpaces)
		{
			m_tabSpaces = newTabSpaces;
		}

		const I8* Lexer::getTokenString(TokenType_e tokentype)
		{
			// Busca nas palavras chaves.
			for (auto& token : keywords)
			{
				if (std::get<1>(token) == tokentype)
				{
					return std::get<0>(token);
				}
			}

			// Busca nos simbolos.
			switch (tokentype)
			{
				case TokenType_e::ScopeResolution: 			return "::";
				case TokenType_e::Increment:					return "++";
				case TokenType_e::Decrement:					return "--";
				case TokenType_e::Arrow:						return "->";
				case TokenType_e::LParBracket:				return "(";
				case TokenType_e::RParBracket:				return ")";
				case TokenType_e::LSquBracket:				return "[";
				case TokenType_e::RSquBracket:				return "]";
				case TokenType_e::LBracket:					return "{";
				case TokenType_e::RBracket:					return "}";
				case TokenType_e::Plus:						return "+";
				case TokenType_e::Minus:						return "-";
				case TokenType_e::Division:					return "/";
				case TokenType_e::Multiplication:			return "*";
				case TokenType_e::Modulo:					return "%";
				case TokenType_e::Assign:					return "=";
				case TokenType_e::PlusAssign:				return "+=";
				case TokenType_e::MinusAssign:				return "-=";
				case TokenType_e::DivAssign:					return "/=";
				case TokenType_e::MultAssign:				return "*=";
				case TokenType_e::BitWiseAndAssign:			return "&=";
				case TokenType_e::BitWiseOrAssign:			return "|=";
				case TokenType_e::BitWiseXorAssign:			return "^=";
				case TokenType_e::ModAssign:					return "%=";
				case TokenType_e::BitWiseLShiftAssign:		return "<<=";
				case TokenType_e::BitWiseRShiftAssign:		return ">>=";
				case TokenType_e::GreaterThan:				return ">";
				case TokenType_e::LessThan:					return "<";
				case TokenType_e::GreaterThanOrEqual:		return ">=";
				case TokenType_e::LessThanOrEqual:			return "<=";
				case TokenType_e::Equal:						return "==";
				case TokenType_e::NotEqual:					return "!=";
				case TokenType_e::BitWiseLShift:				return "<<";
				case TokenType_e::BitWiseRShift:				return ">>";
				case TokenType_e::BitWiseAnd:				return "&";
				case TokenType_e::BitWiseOr:					return "|";
				case TokenType_e::BitWiseXor:				return "^";
				case TokenType_e::BitWiseNot:				return "~";
				case TokenType_e::Colon:						return ":";
				case TokenType_e::SemiColon:					return ";";
				case TokenType_e::LogicalAnd:				return "&&";
				case TokenType_e::LogicalOr:					return "||";
				case TokenType_e::LogicalNot:				return "!";
				case TokenType_e::Interrogation:				return "?";
				case TokenType_e::Comma:						return ":";
				case TokenType_e::Dot:						return ".";
				case TokenType_e::ConstantInteger:			return "integer constant";
				case TokenType_e::ConstantFp32:				return "fp32 constant";
				case TokenType_e::ConstantFp64:				return "fp64 constant";
				case TokenType_e::ConstantBin:				return "binary constant";
				case TokenType_e::ConstantHex:				return "hex constant";
				case TokenType_e::ConstantChar:				return "char constant";
				case TokenType_e::ConstantString:			return "string constant";
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
					tok.type = std::get<1>(keyword);
					return;
				}
			}
			tok.type = TokenType_e::Identifier;
		}

		void Lexer::parseSymbols(Token_s& tok)
		{
			const I8 ch = readChar();

			// Process symbol
			switch (ch)
			{
			case '>':
				{
					tok.type = TokenType_e::GreaterThan;
					tok.value.push_back(readCharAndAdv());

					if (readChar() == '=') {
						tok.type = TokenType_e::GreaterThanOrEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '>') {
						tok.type = TokenType_e::BitWiseRShift;
						tok.value.push_back(readCharAndAdv());
						if (readChar() == '=') {
							tok.type = TokenType_e::BitWiseRShiftAssign;
							tok.value.push_back(readCharAndAdv());
							return;
						}
						return;
					}
				}
				break;
			case '<':
				{
					tok.type = TokenType_e::LessThan;
					tok.value.push_back(readCharAndAdv());

					if (readChar() == '=') {
						tok.type = TokenType_e::LessThanOrEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '<') {
						tok.type = TokenType_e::BitWiseLShift;
						tok.value.push_back(readCharAndAdv());
						if (readChar() == '=') {
							tok.type = TokenType_e::BitWiseLShiftAssign;
							tok.value.push_back(readCharAndAdv());
							return;
						}
						return;
					}
				}
				break;
			case ':':
				{
					tok.type = TokenType_e::Colon;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == ':') {
						tok.type = TokenType_e::ScopeResolution;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '+':
				{
					tok.type = TokenType_e::Plus;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '+') {
						tok.type = TokenType_e::Increment;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.type = TokenType_e::PlusAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '-':
				{
					tok.type = TokenType_e::Minus;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '-') {
						tok.type = TokenType_e::Decrement;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.type = TokenType_e::MinusAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '>') {
						tok.type = TokenType_e::Arrow;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '=':
				{
					tok.type = TokenType_e::Assign;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.type = TokenType_e::Equal;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '!':
				{
					tok.type = TokenType_e::LogicalNot;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.type = TokenType_e::NotEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '&':
				{
					tok.type = TokenType_e::BitWiseAnd;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '&') {
						tok.type = TokenType_e::LogicalAnd;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.type = TokenType_e::BitWiseAndAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '|':
				{
					tok.type = TokenType_e::BitWiseOr;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '|') {
						tok.type = TokenType_e::LogicalOr;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.type = TokenType_e::BitWiseOrAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '.':
				{
					tok.type = TokenType_e::Dot;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '.') {
						tok.value.push_back(readCharAndAdv());
						if (readChar() == '.') {
							tok.type = TokenType_e::Ellipsis;
							tok.value.push_back(readCharAndAdv());
							return;
						}
						throw exceptions::unexpected_token_exception(tok.value, tok.line, tok.column);
					}
				}
				break;
			case '(':
				{
					tok.type = TokenType_e::LParBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ')':
				{
					tok.type = TokenType_e::RParBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '[':
				{
					tok.type = TokenType_e::LSquBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ']':
				{
					tok.type = TokenType_e::RSquBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '{':
				{
					tok.type = TokenType_e::LBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '}':
				{
					tok.type = TokenType_e::RBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '/':
				{
					tok.type = TokenType_e::Division;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.type = TokenType_e::DivAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '*':
				{
					tok.type = TokenType_e::Multiplication;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.type = TokenType_e::MultAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '%':
				{
					tok.type = TokenType_e::Modulo;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.type = TokenType_e::ModAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '~':
				{
					tok.type = TokenType_e::BitWiseNot;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '^':
				{
					tok.type = TokenType_e::BitWiseXor;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.type = TokenType_e::BitWiseXorAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case ';':
				{
					tok.type = TokenType_e::SemiColon;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ',':
				{
					tok.type = TokenType_e::Comma;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '?':
				{
					tok.type = TokenType_e::Interrogation;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			default:
				throw exceptions::unexpected_token_exception(tok.value, tok.line, tok.column);
			}
		}

		void Lexer::parseNumbers(Token_s& tok)
		{
			I8 ch = readChar();
			I8 ch2 = readChar(1);

			// Processa hexadecimal.
			if (ch == '0' && (ch2 == 'x' || ch2 == 'X'))
			{
				nextChar(); // Consome 0
				nextChar(); // Consome x ou X

				tok.type = TokenType_e::ConstantHex;

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

				tok.type = TokenType_e::ConstantBin;

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
						tok.type = TokenType_e::ConstantFp64;
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
					tok.type = TokenType_e::ConstantFp32;
					nextChar();
				}
				else {
					tok.type = TokenType_e::ConstantFp64;
				}
				return;
			}		

			// Precisao padrao: inteiro 32bits sinalizado.
			tok.type = TokenType_e::ConstantInteger;
		}

		void Lexer::parseChar(Token_s& tok)
		{
			tok.type = TokenType_e::ConstantChar;

			nextChar(); // Consome '
			if (readChar() == '\\') {
				tok.value.push_back(readCharAndAdv());
				tok.value.push_back(readCharAndAdv());
			}
			else if (readChar() != '\'') {
				tok.value.push_back(readCharAndAdv());
			}
			nextChar(); // Consome '
		}

		void Lexer::parseString(Token_s& tok)
		{
			tok.type = TokenType_e::ConstantString;

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
							throw exceptions::malformed_string_constant_exception(tok.line, tok.column);
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
								tok.value.push_back(readCharAndAdv());
							}
							tok.value.push_back(readCharAndAdv());
						}
						tok.value.push_back(readCharAndAdv());
					}
				}
				return; // Quanto reconhece "", ou seja a string esta vazia.
			}

			while (true)
			{
				const I8 ch = readChar();

				if (ch == '\\')
				{
					tok.value.push_back(readCharAndAdv());
				}
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