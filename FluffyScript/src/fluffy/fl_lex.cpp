/*
 * fl_lex.cpp
 *
 *  Created on: 11 de ago. de 2021
 *      Author: NPShinigami
 */

#include <tuple>
#include <fstream>
#include <algorithm>
#include <streambuf>

#include "fl_lex.h"
#include "fl_exceptions.h"

namespace fluffy {
	static const std::tuple<String, TokenSubType_e> keywords[] = {
		std::make_tuple("include", 		eTST_Include),		// ok
		std::make_tuple("from", 		eTST_From),			// ok
		std::make_tuple("namespace",	eTST_Namespace),	// ok
		std::make_tuple("class", 		eTST_Class),		// ok
		std::make_tuple("extends", 		eTST_Extends),		// ok
		std::make_tuple("implements",	eTST_Implements),	// ok
		std::make_tuple("constructor",	eTST_Constructor),	// ok
		std::make_tuple("destructor",	eTST_Destructor),	// ok
		std::make_tuple("super",		eTST_Super),		// ok
		std::make_tuple("public", 		eTST_Public),		// ok
		std::make_tuple("protected",	eTST_Protected),	// ok
		std::make_tuple("private", 		eTST_Private),		// ok
		std::make_tuple("final",		eTST_Final),		// ok
		std::make_tuple("override", 	eTST_Override),		// ok
		std::make_tuple("abstract", 	eTST_Abstract),		// ok
		std::make_tuple("virtual", 		eTST_Virtual),		// ok
		std::make_tuple("interface",	eTST_Interface),	// ok
		std::make_tuple("struct",		eTST_Struct),		// ok
		std::make_tuple("enum", 		eTST_Enum),			// ok
		std::make_tuple("trait", 		eTST_Trait),		// ok
		std::make_tuple("static", 		eTST_Static),		// ok
		std::make_tuple("const", 		eTST_Const),		// ok
		std::make_tuple("true", 		eTST_True),			// ok
		std::make_tuple("false", 		eTST_False),		// ok
		std::make_tuple("void", 		eTST_Void),			// ok
		std::make_tuple("bool", 		eTST_Bool),			// ok
		std::make_tuple("i8", 			eTST_i8),			// ok
		std::make_tuple("u8", 			eTST_u8),			// ok
		std::make_tuple("i16", 			eTST_i16),			// ok
		std::make_tuple("u16", 			eTST_u16),			// ok
		std::make_tuple("i32", 			eTST_i32),			// ok
		std::make_tuple("u32", 			eTST_u32),			// ok
		std::make_tuple("i64", 			eTST_i64),			// ok
		std::make_tuple("u64", 			eTST_u64),			// ok
		std::make_tuple("fp32",			eTST_fp32),			// ok
		std::make_tuple("fp64",			eTST_fp64),			// ok
		std::make_tuple("string", 		eTST_String),		// ok
		std::make_tuple("map", 			eTST_Map),			// ok
		std::make_tuple("object", 		eTST_Object),		// ok
		std::make_tuple("fn", 			eTST_Fn),			// ok
		std::make_tuple("let", 			eTST_Let),			// ok
		std::make_tuple("this", 		eTST_This),			// ok
		std::make_tuple("self", 		eTST_Self),			// ok
		std::make_tuple("sizeof", 		eTST_SizeOf),		// ok
		std::make_tuple("new", 			eTST_New),			// ok
		std::make_tuple("as", 			eTST_As),			// ok
		std::make_tuple("is", 			eTST_Is),			// ok
		std::make_tuple("goto", 		eTST_Goto),			// ok
		std::make_tuple("if", 			eTST_If),			// ok
		std::make_tuple("else", 		eTST_Else),			// ok
		std::make_tuple("switch", 		eTST_Switch),		// ok
		std::make_tuple("case", 		eTST_Case),			// ok
		std::make_tuple("default", 		eTST_Default),		// ok
		std::make_tuple("continue", 	eTST_Continue),		// ok
		std::make_tuple("break", 		eTST_Break),		// ok
		std::make_tuple("for", 			eTST_For),			// ok
		std::make_tuple("foreach", 		eTST_Foreach),		// ok
		std::make_tuple("in",			eTST_In),			// ok
		std::make_tuple("while", 		eTST_While),		// ok
		std::make_tuple("do", 			eTST_Do),			// ok
		std::make_tuple("try", 			eTST_Try),			// ok
		std::make_tuple("catch",		eTST_Catch),		// ok
		std::make_tuple("finally",		eTST_Finally),		// ok
		std::make_tuple("panic", 		eTST_Panic),		// ok
		std::make_tuple("return", 		eTST_Return),		// ok
		std::make_tuple("null", 		eTST_Null)			// ok
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
	Lexer::Lexer()
		: m_cursor(0)
		, m_line(1)
		, m_column(1)
		, m_tabSpaces(4)
		, m_eof(false)
	{}

	Lexer::~Lexer()
	{}

	void Lexer::loadSource(String source)
	{
		m_source = source;
		m_filename = "anom_block";
	}

	void Lexer::loadFromSource(String sourceFile)
	{
		m_filename = sourceFile;
		{
			std::ifstream fileStream(m_filename, std::ifstream::binary);
			if (!fileStream.is_open()) {
				throw exceptions::file_not_found_exception(m_filename);
			}

			// Copia o conteudo do arquivo para a string.
			m_source.assign(
				std::istreambuf_iterator<char>(fileStream),
				std::istreambuf_iterator<char>()
			);

			fileStream.close();
		}
	}

	void Lexer::parse(Token_s& tok)
	{
		// Salta: espacos, tabulacoes, carriages, novas linhas.
		skip();

		if (!m_eof) {
			const I8 ch = readChar();

			// Armazena inicio do token e nome do arquivo ou bloco.
			tok.type = eTT_Unknown;
			tok.subType = eTST_Unknown;
			tok.line = m_line;
			tok.column = m_column;
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
			throw exceptions::unexpected_token_exception(tok.line, tok.column);
		}
		else
		{
			tok.line = 0;
			tok.column = 0;
			tok.filename = m_filename;
			tok.value.clear();
			tok.type = eTT_EOF;
			tok.subType = eTST_Unknown;
		}
	}

	void Lexer::setTabSpaces(U32 newTabSpaces)
	{
		m_tabSpaces = newTabSpaces;
	}

	I8 Lexer::readChar(U32 offset)
	{
		return m_source[m_cursor + offset];
	}

	void Lexer::nextChar()
	{
		m_cursor++;

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
					const I8 nch2 = readChar(1);

					if (nch == '*' && nch2 == '/') {
						break;
					}
					if (m_eof) {
						throw exceptions::unexpected_end_of_file_exception();
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
			tok.value.push_back(ch);
			nextChar();
		}

		// Verifica se o token e uma palavra reservada.
		for (auto& keyword : keywords) {
			if (std::get<0>(keyword) == tok.value) {
				tok.type = eTT_Keyword;
				tok.subType = std::get<1>(keyword);
				return;
			}
		}
		tok.type = eTT_Identifier;
		tok.subType = eTST_Unknown;
	}

	void Lexer::parseSymbols(Token_s& tok)
	{
		const I8 ch = readChar();

		tok.type = eTT_Symbol;

		// Process symbol
		switch (ch)
		{
		case '>':
			{
				tok.subType = eTST_GreaterThan;
				tok.value.push_back(ch);
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_GreaterThanOrEqual;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
				if (readChar() == '>') {
					tok.subType = eTST_BitWiseRShift;
					tok.value.push_back(readChar());
					nextChar();
					if (readChar() == '=') {
						tok.subType = eTST_BitWiseRShiftAssign;
						tok.value.push_back(readChar());
						nextChar();
						return;
					}
					return;
				}
			}
			break;
		case '<':
			{
				tok.subType = eTST_LessThan;
				tok.value.push_back(ch);
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_LessThanOrEqual;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
				if (readChar() == '<') {
					tok.subType = eTST_BitWiseLShift;
					tok.value.push_back(readChar());
					nextChar();
					if (readChar() == '=') {
						tok.subType = eTST_BitWiseLShiftAssign;
						tok.value.push_back(readChar());
						nextChar();
						return;
					}
					return;
				}
			}
			break;
		case ':':
			{
				tok.subType = eTST_Colon;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == ':') {
					tok.subType = eTST_ScopeResolution;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '+':
			{
				tok.subType = eTST_Plus;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '+') {
					tok.subType = eTST_Increment;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
				if (readChar() == '=') {
					tok.subType = eTST_PlusAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '-':
			{
				tok.subType = eTST_Minus;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '-') {
					tok.subType = eTST_Decrement;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
				if (readChar() == '=') {
					tok.subType = eTST_MinusAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
				if (readChar() == '>') {
					tok.subType = eTST_ReturnSet;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '=':
			{
				tok.subType = eTST_Assign;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_Equal;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '!':
			{
				tok.subType = eTST_LogicalNot;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_NotEqual;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '&':
			{
				tok.subType = eTST_BitWiseAnd;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '&') {
					tok.subType = eTST_LogicalAnd;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
				if (readChar() == '=') {
					tok.subType = eTST_BitWiseAndAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '|':
			{
				tok.subType = eTST_BitWiseOr;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '|') {
					tok.subType = eTST_LogicalOr;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
				if (readChar() == '=') {
					tok.subType = eTST_BitWiseOrAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '.':
			{
				tok.subType = eTST_Dot;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case '(':
			{
				tok.subType = eTST_LParBracket;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case ')':
			{
				tok.subType = eTST_RParBracket;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case '[':
			{
				tok.subType = eTST_LSquBracket;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case ']':
			{
				tok.subType = eTST_RSquBracket;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case '{':
			{
				tok.subType = eTST_LBracket;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case '}':
			{
				tok.subType = eTST_RBracket;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case '/':
			{
				tok.subType = eTST_Division;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_DivAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '*':
			{
				tok.subType = eTST_Multiplication;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_MultAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '%':
			{
				tok.subType = eTST_Modulo;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_ModAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case '~':
			{
				tok.subType = eTST_BitWiseNot;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case '^':
			{
				tok.subType = eTST_BitWiseXor;
				tok.value.push_back(readChar());
				nextChar();
				if (readChar() == '=') {
					tok.subType = eTST_BitWiseXorAssign;
					tok.value.push_back(readChar());
					nextChar();
					return;
				}
			}
			break;
		case ';':
			{
				tok.subType = eTST_SemiColon;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case ',':
			{
				tok.subType = eTST_Comma;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		case '?':
			{
				tok.subType = eTST_Interrogation;
				tok.value.push_back(readChar());
				nextChar();
			}
			break;
		default:
			throw exceptions::unexpected_token_exception(tok.line, tok.column);
		}
	}

	void Lexer::parseNumbers(Token_s& tok)
	{
		tok.type = eTT_Constant;

		// Processa hexadecimal.
		if (readChar() == '0' && (readChar(1) == 'x' || readChar(1) == 'X'))
		{
			nextChar(); // Consome 0
			nextChar(); // Consome x ou X

			tok.subType = eTST_ConstantHex;

			bool isValid = false;
			while (true)
			{
				const I8 ch = readChar();
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

			tok.subType = eTST_ConstantBin;

			Bool isValid = false;
			while (true)
			{
				const I8 ch = readChar();
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
			const I8 ch = readChar();

			if (!isnumber(ch) && ch != '.')
			{
				break;
			}
			if (ch == '.') {
				if (isReal) {
					tok.subType = eTST_ConstantFp64;
					return;
				}
				isReal = true;
			}
			tok.value.push_back(ch);
			nextChar();
		}

		if (isReal) {
			const I8 ch = readChar();

			if (ch == 'f' || ch == 'F') {
				tok.subType = eTST_ConstantFp32;
				nextChar();
			} else {
				tok.subType = eTST_ConstantFp64;
			}
			return;
		} else {
			const I8 ch = readChar();

			// Processa pos fixo.
			if (ch == 'i' || ch == 'I')
			{
				nextChar();
				const I8 ch1 = readChar();

				if (ch1 == '8') {
					nextChar();
					tok.subType = eTST_ConstantI8;
					return;
				}
				nextChar();
				const I8 ch2 = readChar();

				if (ch1 == '1' && ch2 == '6') {
					nextChar();
					tok.subType = eTST_ConstantI16;
					return;
				}
				if (ch1 == '3' && ch2 == '2') {
					nextChar();
					tok.subType = eTST_ConstantI32;
					return;
				}
				if (ch1 == '6' && ch2 == '4') {
					nextChar();
					tok.subType = eTST_ConstantI64;
					return;
				}
				throw exceptions::malformed_number_exception(tok.line, tok.column);
			}

			// Processa pos fixo.
			if (ch == 'u' || ch == 'U')
			{
				nextChar();
				const I8 ch1 = readChar();

				if (ch1 == '8') {
					nextChar();
					tok.subType = eTST_ConstantU8;
					return;
				}
				nextChar();
				const I8 ch2 = readChar();

				if (ch1 == '1' && ch2 == '6') {
					nextChar();
					tok.subType = eTST_ConstantU16;
					return;
				}
				if (ch1 == '3' && ch2 == '2') {
					nextChar();
					tok.subType = eTST_ConstantU32;
					return;
				}
				if (ch1 == '6' && ch2 == '4') {
					nextChar();
					tok.subType = eTST_ConstantU64;
					return;
				}
				throw exceptions::malformed_number_exception(tok.line, tok.column);
			}

			// Precisao padrao: inteiro 32bits sinalizado.
			tok.subType = eTST_ConstantI32;
			return;
		}
	}

	void Lexer::parseChar(Token_s& tok)
	{
		tok.type = eTT_Constant;
		tok.subType = eTST_ConstantChar;

		nextChar(); // Consome '
		tok.value.push_back(readChar());
		nextChar();
		if (!ischar(readChar())) {
			throw exceptions::malformed_character_constant_exception(tok.line, tok.column);
		}
		nextChar(); // Consome '
	}

	void Lexer::parseString(Token_s& tok)
	{
		tok.type = eTT_Constant;
		tok.subType = eTST_ConstantString;

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
			tok.value.push_back(readChar());
			nextChar();
		}
		nextChar(); // Consome "
	}
} }

