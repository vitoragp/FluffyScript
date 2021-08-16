#include <tuple>
#include <algorithm>

#include "fl_lex.h"
#include "fl_exceptions.h"

namespace fluffy {
	static const std::tuple<String, TokenSubType_e> keywords[] = {
		std::make_tuple("include", 		TokenSubType_e::eTST_Include),		// ok
		std::make_tuple("from", 		TokenSubType_e::eTST_From),			// ok
		std::make_tuple("export", 		TokenSubType_e::eTST_Export),		// ok
		std::make_tuple("namespace",	TokenSubType_e::eTST_Namespace),	// ok
		std::make_tuple("class", 		TokenSubType_e::eTST_Class),		// ok
		std::make_tuple("extends", 		TokenSubType_e::eTST_Extends),		// ok
		std::make_tuple("implements",	TokenSubType_e::eTST_Implements),	// ok
		std::make_tuple("where", 		TokenSubType_e::eTST_Where),		// ok
		std::make_tuple("constructor",	TokenSubType_e::eTST_Constructor),	// ok
		std::make_tuple("destructor",	TokenSubType_e::eTST_Destructor),	// ok
		std::make_tuple("super",		TokenSubType_e::eTST_Super),		// ok
		std::make_tuple("public", 		TokenSubType_e::eTST_Public),		// ok
		std::make_tuple("protected",	TokenSubType_e::eTST_Protected),	// ok
		std::make_tuple("private", 		TokenSubType_e::eTST_Private),		// ok
		std::make_tuple("final",		TokenSubType_e::eTST_Final),		// ok
		std::make_tuple("override", 	TokenSubType_e::eTST_Override),		// ok
		std::make_tuple("abstract", 	TokenSubType_e::eTST_Abstract),		// ok
		std::make_tuple("virtual", 		TokenSubType_e::eTST_Virtual),		// ok
		std::make_tuple("interface",	TokenSubType_e::eTST_Interface),	// ok
		std::make_tuple("struct",		TokenSubType_e::eTST_Struct),		// ok
		std::make_tuple("enum", 		TokenSubType_e::eTST_Enum),			// ok
		std::make_tuple("match", 		TokenSubType_e::eTST_Match),		// ok
		std::make_tuple("when", 		TokenSubType_e::eTST_When),			// ok
		std::make_tuple("trait", 		TokenSubType_e::eTST_Trait),		// ok
		std::make_tuple("static", 		TokenSubType_e::eTST_Static),		// ok
		std::make_tuple("const", 		TokenSubType_e::eTST_Const),		// ok
		std::make_tuple("true", 		TokenSubType_e::eTST_True),			// ok
		std::make_tuple("false", 		TokenSubType_e::eTST_False),		// ok
		std::make_tuple("void", 		TokenSubType_e::eTST_Void),			// ok
		std::make_tuple("bool", 		TokenSubType_e::eTST_Bool),			// ok
		std::make_tuple("i8", 			TokenSubType_e::eTST_I8),			// ok
		std::make_tuple("u8", 			TokenSubType_e::eTST_U8),			// ok
		std::make_tuple("i16", 			TokenSubType_e::eTST_I16),			// ok
		std::make_tuple("u16", 			TokenSubType_e::eTST_U16),			// ok
		std::make_tuple("i32", 			TokenSubType_e::eTST_I32),			// ok
		std::make_tuple("u32", 			TokenSubType_e::eTST_U32),			// ok
		std::make_tuple("i64", 			TokenSubType_e::eTST_I64),			// ok
		std::make_tuple("u64", 			TokenSubType_e::eTST_U64),			// ok
		std::make_tuple("fp32",			TokenSubType_e::eTST_Fp32),			// ok
		std::make_tuple("fp64",			TokenSubType_e::eTST_Fp64),			// ok
		std::make_tuple("string", 		TokenSubType_e::eTST_String),		// ok
		std::make_tuple("vector",		TokenSubType_e::eTST_Vector),		// ok
		std::make_tuple("set", 			TokenSubType_e::eTST_Set),			// ok
		std::make_tuple("map", 			TokenSubType_e::eTST_Map),			// ok
		std::make_tuple("object", 		TokenSubType_e::eTST_Object),		// ok
		std::make_tuple("fn", 			TokenSubType_e::eTST_Fn),			// ok
		std::make_tuple("let", 			TokenSubType_e::eTST_Let),			// ok
		std::make_tuple("this", 		TokenSubType_e::eTST_This),			// ok
		std::make_tuple("self", 		TokenSubType_e::eTST_Self),			// ok
		std::make_tuple("sizeof", 		TokenSubType_e::eTST_SizeOf),		// ok
		std::make_tuple("new", 			TokenSubType_e::eTST_New),			// ok
		std::make_tuple("as", 			TokenSubType_e::eTST_As),			// ok
		std::make_tuple("is", 			TokenSubType_e::eTST_Is),			// ok
		std::make_tuple("goto", 		TokenSubType_e::eTST_Goto),			// ok
		std::make_tuple("if", 			TokenSubType_e::eTST_If),			// ok
		std::make_tuple("else", 		TokenSubType_e::eTST_Else),			// ok
		std::make_tuple("continue", 	TokenSubType_e::eTST_Continue),		// ok
		std::make_tuple("break", 		TokenSubType_e::eTST_Break),		// ok
		std::make_tuple("for", 			TokenSubType_e::eTST_For),			// ok
		std::make_tuple("foreach", 		TokenSubType_e::eTST_Foreach),		// ok
		std::make_tuple("in",			TokenSubType_e::eTST_In),			// ok
		std::make_tuple("while", 		TokenSubType_e::eTST_While),		// ok
		std::make_tuple("do", 			TokenSubType_e::eTST_Do),			// ok
		std::make_tuple("try", 			TokenSubType_e::eTST_Try),			// ok
		std::make_tuple("catch",		TokenSubType_e::eTST_Catch),		// ok
		std::make_tuple("finally",		TokenSubType_e::eTST_Finally),		// ok
		std::make_tuple("panic", 		TokenSubType_e::eTST_Panic),		// ok
		std::make_tuple("return", 		TokenSubType_e::eTST_Return),		// ok
		std::make_tuple("null", 		TokenSubType_e::eTST_Null)			// ok
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
				tok.type = TokenType_e::eTT_Unknown;
				tok.subType = TokenSubType_e::eTST_Unknown;
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
				tok.line = 0;
				tok.column = 0;
				tok.filename = m_filename;
				tok.value.clear();
				tok.type = TokenType_e::eTT_EOF;
				tok.subType = TokenSubType_e::eTST_Unknown;
			}
		}

		void Lexer::setTabSpaces(U32 newTabSpaces)
		{
			m_tabSpaces = newTabSpaces;
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
					tok.type = TokenType_e::eTT_Keyword;
					tok.subType = std::get<1>(keyword);
					return;
				}
			}
			tok.type = TokenType_e::eTT_Identifier;
			tok.subType = TokenSubType_e::eTST_Unknown;
		}

		void Lexer::parseSymbols(Token_s& tok)
		{
			const I8 ch = readChar();

			tok.type = TokenType_e::eTT_Symbol;

			// Process symbol
			switch (ch)
			{
			case '>':
				{
					tok.subType = TokenSubType_e::eTST_GreaterThan;
					tok.value.push_back(readCharAndAdv());

					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_GreaterThanOrEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '>') {
						tok.subType = TokenSubType_e::eTST_BitWiseRShift;
						tok.value.push_back(readCharAndAdv());
						if (readChar() == '=') {
							tok.subType = TokenSubType_e::eTST_BitWiseRShiftAssign;
							tok.value.push_back(readCharAndAdv());
							return;
						}
						return;
					}
				}
				break;
			case '<':
				{
					tok.subType = TokenSubType_e::eTST_LessThan;
					tok.value.push_back(readCharAndAdv());

					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_LessThanOrEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '<') {
						tok.subType = TokenSubType_e::eTST_BitWiseLShift;
						tok.value.push_back(readCharAndAdv());
						if (readChar() == '=') {
							tok.subType = TokenSubType_e::eTST_BitWiseLShiftAssign;
							tok.value.push_back(readCharAndAdv());
							return;
						}
						return;
					}
				}
				break;
			case ':':
				{
					tok.subType = TokenSubType_e::eTST_Colon;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == ':') {
						tok.subType = TokenSubType_e::eTST_ScopeResolution;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '+':
				{
					tok.subType = TokenSubType_e::eTST_Plus;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '+') {
						tok.subType = TokenSubType_e::eTST_Increment;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_PlusAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '-':
				{
					tok.subType = TokenSubType_e::eTST_Minus;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '-') {
						tok.subType = TokenSubType_e::eTST_Decrement;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_MinusAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '>') {
						tok.subType = TokenSubType_e::eTST_Arrow;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '=':
				{
					tok.subType = TokenSubType_e::eTST_Assign;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_Equal;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '!':
				{
					tok.subType = TokenSubType_e::eTST_LogicalNot;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_NotEqual;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '&':
				{
					tok.subType = TokenSubType_e::eTST_BitWiseAnd;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '&') {
						tok.subType = TokenSubType_e::eTST_LogicalAnd;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_BitWiseAndAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '|':
				{
					tok.subType = TokenSubType_e::eTST_BitWiseOr;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '|') {
						tok.subType = TokenSubType_e::eTST_LogicalOr;
						tok.value.push_back(readCharAndAdv());
						return;
					}
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_BitWiseOrAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '.':
				{
					tok.subType = TokenSubType_e::eTST_Dot;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '(':
				{
					tok.subType = TokenSubType_e::eTST_LParBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ')':
				{
					tok.subType = TokenSubType_e::eTST_RParBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '[':
				{
					tok.subType = TokenSubType_e::eTST_LSquBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ']':
				{
					tok.subType = TokenSubType_e::eTST_RSquBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '{':
				{
					tok.subType = TokenSubType_e::eTST_LBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '}':
				{
					tok.subType = TokenSubType_e::eTST_RBracket;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '/':
				{
					tok.subType = TokenSubType_e::eTST_Division;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_DivAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '*':
				{
					tok.subType = TokenSubType_e::eTST_Multiplication;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_MultAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '%':
				{
					tok.subType = TokenSubType_e::eTST_Modulo;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_ModAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case '~':
				{
					tok.subType = TokenSubType_e::eTST_BitWiseNot;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '^':
				{
					tok.subType = TokenSubType_e::eTST_BitWiseXor;
					tok.value.push_back(readCharAndAdv());
					if (readChar() == '=') {
						tok.subType = TokenSubType_e::eTST_BitWiseXorAssign;
						tok.value.push_back(readCharAndAdv());
						return;
					}
				}
				break;
			case ';':
				{
					tok.subType = TokenSubType_e::eTST_SemiColon;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case ',':
				{
					tok.subType = TokenSubType_e::eTST_Comma;
					tok.value.push_back(readCharAndAdv());
				}
				break;
			case '?':
				{
					tok.subType = TokenSubType_e::eTST_Interrogation;
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

			tok.type = TokenType_e::eTT_Constant;

			// Processa hexadecimal.
			if (ch == '0' && (ch2 == 'x' || ch2 == 'X'))
			{
				nextChar(); // Consome 0
				nextChar(); // Consome x ou X

				tok.subType = TokenSubType_e::eTST_ConstantHex;

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

				tok.subType = TokenSubType_e::eTST_ConstantBin;

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
						tok.subType = TokenSubType_e::eTST_ConstantFp64;
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
					tok.subType = TokenSubType_e::eTST_ConstantFp32;
					nextChar();
				}
				else {
					tok.subType = TokenSubType_e::eTST_ConstantFp64;
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
						tok.subType = TokenSubType_e::eTST_ConstantI8;
						return;
					}
					nextChar();
					ch2 = readChar();

					if (ch == '1' && ch2 == '6') {
						nextChar();
						tok.subType = TokenSubType_e::eTST_ConstantI16;
						return;
					}
					if (ch == '3' && ch2 == '2') {
						nextChar();
						tok.subType = TokenSubType_e::eTST_ConstantI32;
						return;
					}
					if (ch == '6' && ch2 == '4') {
						nextChar();
						tok.subType = TokenSubType_e::eTST_ConstantI64;
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
						tok.subType = TokenSubType_e::eTST_ConstantU8;
						return;
					}
					nextChar();
					ch2 = readChar();

					if (ch == '1' && ch2 == '6') {
						nextChar();
						tok.subType = TokenSubType_e::eTST_ConstantU16;
						return;
					}
					if (ch == '3' && ch2 == '2') {
						nextChar();
						tok.subType = TokenSubType_e::eTST_ConstantU32;
						return;
					}
					if (ch == '6' && ch2 == '4') {
						nextChar();
						tok.subType = TokenSubType_e::eTST_ConstantU64;
						return;
					}
					throw exceptions::malformed_number_exception(tok.line, tok.column);
				}

				// Precisao padrao: inteiro 32bits sinalizado.
				tok.subType = TokenSubType_e::eTST_ConstantI32;
				return;
			}
		}

		void Lexer::parseChar(Token_s& tok)
		{
			tok.type = TokenType_e::eTT_Constant;
			tok.subType = TokenSubType_e::eTST_ConstantChar;

			nextChar(); // Consome '
			tok.value.push_back(readCharAndAdv());
			if (!ischar(readChar())) {
				throw exceptions::malformed_character_constant_exception(tok.line, tok.column);
			}
			nextChar(); // Consome '
		}

		void Lexer::parseString(Token_s& tok)
		{
			tok.type = TokenType_e::eTT_Constant;
			tok.subType = TokenSubType_e::eTST_ConstantString;

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