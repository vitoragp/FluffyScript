#include <cassert>
#include "fl_exceptions.h"
#include "fl_parser.h"
#include "fl_lex_utils.h"
namespace fluffy { namespace parser {
	using utils::LexUtils;

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
		return std::move(parseProgram());
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

	String Parser::expectConstantString()
	{
		String value = m_tok.value;
		if (!LexUtils::isConstantString(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		nextToken();
		return value;
	}

	ProgramPtr Parser::parseProgram()
	{
		auto program = std::make_unique<ast::Program>();

		// Faz a analise do primeiro token.
		nextToken();

		if (LexUtils::isEof(m_tok)) {
			return program;
		}

		// Processa declaracoes de include.
		while (true)
		{
			if (LexUtils::isEof(m_tok)) {
				return program;
			}
			if (LexUtils::isInclude(m_tok)) {
				program->includeList.push_back(parseInclude());
				continue;
			}
			break;
		}

		while (true)
		{
			if (LexUtils::isEof(m_tok)) {
				return program;
			}

			// Processa namespaces.
			if (LexUtils::isNamespace(m_tok)) {
				program->namespaceList.push_back(parseNamespace());
			}
		}
		return nullptr;
	}

	IncludePtr Parser::parseInclude()
	{
		auto includeDecl = std::make_unique<ast::Include>();

		// Consome 'include'.
		expectToken([this]() { return LexUtils::isInclude(m_tok); });

		// Consome '{'
		expectToken([this]() { return LexUtils::isLeftBracket(m_tok); });

		// Consome Identificadores.
		while (true)
		{
			if (LexUtils::isMultiplication(m_tok)) {
				// Se existe identificaores declarados, o caractere coringa
				// nao pode ser usado.
				if (includeDecl->includedItemList.size()) {
					throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
				}

				// Consome '*'
				expectToken([this]() { return LexUtils::isMultiplication(m_tok);  });

				includeDecl->allFlag = true;
				break;
			}

			// Consome identificador.
			includeDecl->includedItemList.push_back(expectIdentifier());

			// Verifica se ha mais declaracoes.
			if (!LexUtils::isComma(m_tok)) {
				break;
			}

			// Consome ','
			expectToken([this]() { return LexUtils::isComma(m_tok);  });
		}

		// Consome '}'
		expectToken([this]() { return LexUtils::isRightBracket(m_tok); });

		// Consome 'from'
		expectToken([this]() { return LexUtils::isFrom(m_tok); });

		// Consome o identificador do namespace.
		includeDecl->fromNamespace = parseNamedDecl();

		// Consome ';'
		expectToken([this]() { return LexUtils::isSemiColon(m_tok); });

		return includeDecl;
	}

	NamespacePtr Parser::parseNamespace()
	{
		auto namespaceDecl = std::make_unique<ast::Namespace>();

		// Consome 'namespace'.
		expectToken([this]() { return LexUtils::isNamespace(m_tok); });

		// Consome o identficador com o nome do namespace.
		namespaceDecl->name = expectIdentifier();

		// Consome '{'.
		expectToken([this]() { return LexUtils::isLeftBracket(m_tok); });

		while (true)
		{
			// Verifica se chegou ao fim do namespace.
			if (LexUtils::isRightBracket(m_tok))
			{
				break;
			}

			// Processa namespace.
			if (LexUtils::isNamespace(m_tok))
			{
				namespaceDecl->namespaceList.push_back(parseNamespace());
				continue;
			}

			// Processa declaracao geral.
			namespaceDecl->generalDeclList.push_back(parseGeneralStmt());
		}

		// Consome '}'.
		expectToken([this]() { return LexUtils::isRightBracket(m_tok); });

		return namespaceDecl;
	}

	GeneralStmtPtr Parser::parseGeneralStmt()
	{
		// Verifica se houve a declararao para exportar o elemento.
		return nullptr;
	}

	NamedDeclPtr Parser::parseNamedDecl()
	{
		auto namedDecl = std::make_unique<ast::NamedDecl>();

		// Consome o identificador.
		namedDecl->identifier = expectIdentifier();

		while (true)
		{
			// Verifica se a resolucao de escopo.
			if (!LexUtils::isScopeResolution(m_tok))
			{
				break;
			}

			// Consome '::'.
			expectToken([this]() { return LexUtils::isScopeResolution(m_tok); });

			// Consome o tailDecl.
			namedDecl->tailDecl = parseNamedDecl();
		}
		return namedDecl;
	}
} }