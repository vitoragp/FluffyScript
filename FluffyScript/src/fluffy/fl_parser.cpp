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

	const Bool Parser::expectConstantBool()
	{
		if (!LexUtils::isTrue(m_tok) && !LexUtils::isFalse(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Bool value = LexUtils::isTrue(m_tok) ? true : false;
		nextToken();
		return value;
	}

	const I8 Parser::expectConstantI8()
	{
		if (!LexUtils::isConstantI8(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I8 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U8 Parser::expectConstantU8()
	{
		if (!LexUtils::isConstantU8(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U8 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I16 Parser::expectConstantI16()
	{
		if (!LexUtils::isConstantI16(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I16 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U16 Parser::expectConstantU16()
	{
		if (!LexUtils::isConstantU16(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U16 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I32 Parser::expectConstantI32()
	{
		if (!LexUtils::isConstantI32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I32 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U32 Parser::expectConstantU32()
	{
		if (!LexUtils::isConstantU32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I32 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I64 Parser::expectConstantI64()
	{
		if (!LexUtils::isConstantI64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I64 value = std::stol(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const U64 Parser::expectConstantU64()
	{
		if (!LexUtils::isConstantU64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const U64 value = std::stoi(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const Fp32 Parser::expectConstantFp32()
	{
		if (!LexUtils::isConstantFp32(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Fp32 value = std::stof(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const Fp64 Parser::expectConstantFp64()
	{
		if (!LexUtils::isConstantFp64(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const Fp64 value = std::stod(m_tok.value, nullptr);
		nextToken();
		return value;
	}

	const I8 Parser::expectConstantChar()
	{
		if (!LexUtils::isConstantChar(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		const I8 value = m_tok.value[0];
		nextToken();
		return value;
	}

	String Parser::expectConstantString()
	{
		if (!LexUtils::isConstantString(m_tok)) {
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}
		String value = m_tok.value;
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
		includeDecl->fromNamespace = parseScopedIdentifierDecl();

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
		Bool hasExport = false;

		// Verifica se houve a declararao para exportar o elemento.
		if (LexUtils::isExport(m_tok))
		{
			expectToken([this]() { return LexUtils::isExport(m_tok);  });
			hasExport = true;
		}

		// Verifica qual declaracao processar.
		switch (m_tok.subType)
		{
		case eTST_Class:
			throw exceptions::not_implemented_feature_exception("parseClassDecl");
		case eTST_Interface:
			throw exceptions::not_implemented_feature_exception("parseInterfaceDecl");
		case eTST_Struct:
			throw exceptions::not_implemented_feature_exception("parseStructDecl");
		case eTST_Enum:
			throw exceptions::not_implemented_feature_exception("parseEnumDecl");
		case eTST_Trait:
			throw exceptions::not_implemented_feature_exception("parseTraitDecl");
		case eTST_Let:
			throw exceptions::not_implemented_feature_exception("parseVariableDecl");
		case eTST_Fn:			
			throw exceptions::not_implemented_feature_exception("parseFunctionDecl");
		default:
			throw exceptions::unexpected_token_exception(m_tok.value, m_tok.line, m_tok.column);
		}

		return nullptr;
	}

	ScopedIdentifierDeclPtr Parser::parseScopedIdentifierDecl()
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>();

		// Consome o identificador.
		scopedIdentifierDecl->identifier = expectIdentifier();

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
			scopedIdentifierDecl->tailDecl = parseScopedIdentifierDecl();
		}
		return scopedIdentifierDecl;
	}
} }