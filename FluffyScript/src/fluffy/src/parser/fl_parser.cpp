#include "lexer\fl_lexer.h"
#include "parser\fl_parser.h"
#include "fl_exceptions.h"

namespace fluffy {
	/**
	 * isExprOperator
	 */

	bool isExprOperator(TokenType_e type)
	{
		switch (type) {
		case TokenType_e::Comma:
		case TokenType_e::Assign:
		case TokenType_e::LogicalOr:
		case TokenType_e::LogicalAnd:
		case TokenType_e::BitWiseOr:
		case TokenType_e::BitWiseXor:
		case TokenType_e::BitWiseAnd:
		case TokenType_e::BitWiseOrAssign:
		case TokenType_e::BitWiseXorAssign:
		case TokenType_e::BitWiseAndAssign:
		case TokenType_e::BitWiseRShiftAssign:
		case TokenType_e::BitWiseLShiftAssign:
		case TokenType_e::Equal:
		case TokenType_e::NotEqual:
		case TokenType_e::GreaterThan:
		case TokenType_e::LessThan:
		case TokenType_e::GreaterThanOrEqual:
		case TokenType_e::LessThanOrEqual:
		case TokenType_e::BitWiseLShift:
		case TokenType_e::BitWiseRShift:
		case TokenType_e::Plus:
		case TokenType_e::Minus:
		case TokenType_e::Multiplication:
		case TokenType_e::Division:
		case TokenType_e::Modulo:
		case TokenType_e::PlusAssign:
		case TokenType_e::MinusAssign:
		case TokenType_e::MultAssign:
		case TokenType_e::DivAssign:
		case TokenType_e::ModAssign:
		case TokenType_e::Increment:
		case TokenType_e::Decrement:
		case TokenType_e::As:
		case TokenType_e::Is:
		case TokenType_e::LSquBracket:
		case TokenType_e::LParBracket:
		case TokenType_e::Dot:
		case TokenType_e::BitWiseNot:
		case TokenType_e::LogicalNot:
		case TokenType_e::ScopeResolution:
		case TokenType_e::Interrogation:
		case TokenType_e::Match:
		case TokenType_e::Shared:
		case TokenType_e::Ref:
			return true;
		default:
			break;
		}
		return false;
	}

	/**
	 * TokenInfo_t
	 */

	struct TokenInfo_t
	{
		TokenType_e		op;
		U32				prec;
		Bool			left;
	};

	static const TokenInfo_t op_info[] = {
		{ TokenType_e::Comma,				1, true },
		{ TokenType_e::Interrogation,		2, false },
		{ TokenType_e::Assign,				2, false },
		{ TokenType_e::PlusAssign,			2, false },
		{ TokenType_e::MinusAssign,			2, false },
		{ TokenType_e::MultAssign,			2, false },
		{ TokenType_e::DivAssign,			2, false },
		{ TokenType_e::ModAssign,			2, false },
		{ TokenType_e::BitWiseOrAssign,		2, false },
		{ TokenType_e::BitWiseXorAssign,	2, false },
		{ TokenType_e::BitWiseAndAssign,	2, false },
		{ TokenType_e::BitWiseLShiftAssign,	2, false },
		{ TokenType_e::BitWiseRShiftAssign,	2, false },
		{ TokenType_e::LogicalOr,			3, true },
		{ TokenType_e::LogicalAnd,			4, true },
		{ TokenType_e::BitWiseOr,			5, true },
		{ TokenType_e::BitWiseXor,			6, true },
		{ TokenType_e::BitWiseAnd,			7, true },
		{ TokenType_e::Equal,			    8, true },
		{ TokenType_e::NotEqual,			8, true },
		{ TokenType_e::GreaterThan,		    9, true },
		{ TokenType_e::LessThan,			9, true },
		{ TokenType_e::GreaterThanOrEqual,  9, true },
		{ TokenType_e::LessThanOrEqual,		9, true },
		{ TokenType_e::BitWiseLShift,		10, true },
		{ TokenType_e::BitWiseRShift,		10, true },
		{ TokenType_e::Plus,				11, true },   // Add
		{ TokenType_e::Minus,				11, true },   // Sub
		{ TokenType_e::Multiplication,		12, true },
		{ TokenType_e::Division,			12, true },
		{ TokenType_e::Modulo,				12, true },
		{ TokenType_e::Increment,			13, false },  // Prefix
		{ TokenType_e::Decrement,			13, false },  // Prefix
		{ TokenType_e::Ref,					13, false },  // Prefix
		{ TokenType_e::Shared,				13, false },  // Prefix
		{ TokenType_e::Plus,				13, false },  // Unary positive
		{ TokenType_e::Minus,				13, false },  // Unary negative
		{ TokenType_e::Match,				13, false },
		{ TokenType_e::As,					13, true },
		{ TokenType_e::Is,					13, true },
		{ TokenType_e::LSquBracket,			14, true },   // Array
		{ TokenType_e::LParBracket,			14, true },   // Function Call
		{ TokenType_e::Increment,			14, true },   // Postfix
		{ TokenType_e::Decrement,			14, true },   // Postfix
		{ TokenType_e::Dot,					14, true },
		{ TokenType_e::BitWiseNot,			14, true },
		{ TokenType_e::LogicalNot,			14, true },
		{ TokenType_e::ScopeResolution,		15, true },
		{ TokenType_e::Unknown,				0, true }
	};

	// Retorna informacoes sobre os operadores.
	const TokenInfo_t& getOperatorInfo(TokenType_e op)
	{
		U32 current_op = 0;
		while (true)
		{
			if (op_info[current_op].op == op)
			{
				return op_info[current_op];
			}
			if (op_info[current_op].op == TokenType_e::Unknown)
			{
				return op_info[current_op];
			}
			current_op++;
		}
	}
}

namespace fluffy { namespace parser {
	///
	/// Parser
	///
	
	Parser::Parser(BufferBase* const buffer)
		: m_lexer(new lexer::Lexer(buffer))
	{}

	Parser::~Parser()
	{}

	void
	Parser::loadSource(const I8* sourceCode)
	{
		m_lexer->loadSource(sourceCode);
	}

	void
	Parser::loadSourceFromFile(const I8* sourceFilename)
	{
		m_lexer->loadSourceFromFile(sourceFilename);
	}

	Bool
	Parser::finished()
	{
		return m_lexer->isEof();
	}

	std::unique_ptr<ast::CodeUnit>
	Parser::parseCodeUnit(ParserContext_s& ctx)
	{
		auto codeUnit = std::make_unique<ast::CodeUnit>(
			m_lexer->getFilename()
		);

		if (m_lexer->isEof()) {
			return codeUnit;
		}

		// Processa declaracoes de include.
		while (true)
		{
			if (m_lexer->isEof()) {
				return codeUnit;
			}
			if (m_lexer->isInclude()) {
				codeUnit->includeDeclList.push_back(parseInclude(ctx));
				continue;
			}
			break;
		}

		while (true)
		{
			if (m_lexer->isEof()) {
				break;
			}

			// Processa namespaces.
			if (m_lexer->isNamespace()) {
				codeUnit->namespaceDeclList.push_back(parseNamespace(ctx));
				continue;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{
					TokenType_e::Namespace
				},
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}
		return codeUnit;
	}

	std::unique_ptr<ast::IncludeDecl>
	Parser::parseInclude(ParserContext_s& ctx)
	{
		auto includeDecl = std::make_unique<ast::IncludeDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'include'.
		m_lexer->expectToken(TokenType_e::Include);

		// Consome '{'
		m_lexer->expectToken(TokenType_e::LBracket);

		// Consome Identificadores.
		while (true)
		{
			if (m_lexer->isMultiplication()) {
				// Se existe identificaores declarados, o caractere coringa
				// nao pode ser usado.
				if (includeDecl->includedItemList.size()) {
					throw exceptions::unexpected_token_exception(
						m_lexer->getToken().value,
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}

				// Consome '*'
				m_lexer->expectToken(TokenType_e::Multiplication);
				break;
			}

			// Consome identificador.
			includeDecl->includedItemList.push_back(m_lexer->expectIdentifier());

			// Verifica se ha mais declaracoes.
			if (!m_lexer->isComma()) {
				break;
			}

			// Consome ','
			m_lexer->expectToken(TokenType_e::Comma);
		}

		// Consome '}'
		m_lexer->expectToken(TokenType_e::RBracket);

		// Consome 'from'
		m_lexer->expectToken(TokenType_e::From);

		// Consome o identificador do namespace.
		includeDecl->fromNamespace = parseScopedIdentifier(ctx);

		// Consome ';'
		m_lexer->expectToken(TokenType_e::SemiColon);

		return includeDecl;
	}

	std::unique_ptr<ast::NamespaceDecl>
	Parser::parseNamespace(ParserContext_s& ctx)
	{
		auto namespaceDecl = std::make_unique<ast::NamespaceDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'namespace'.
		m_lexer->expectToken(TokenType_e::Namespace);

		// Consome o identficador com o nome do namespace.
		namespaceDecl->identifier = m_lexer->expectIdentifier();

		// Consome '{'.
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			// Verifica se chegou ao fim do namespace.
			if (m_lexer->isRightBracket())
			{
				break;
			}

			// Processa namespace.
			if (m_lexer->isNamespace())
			{
				namespaceDecl->namespaceDeclList.push_back(parseNamespace(ctx));
				continue;
			}

			// Processa declaracao geral.
			namespaceDecl->generalDeclList.push_back(parseGeneralStmt(ctx));
		}

		// Consome '}'.
		m_lexer->expectToken(TokenType_e::RBracket);

		return namespaceDecl;
	}

	std::unique_ptr<ast::ClassDecl>
	Parser::parseClass(ParserContext_s& ctx, Bool hasExport, Bool hasAbstract)
	{
		auto classDecl = std::make_unique<ast::ClassDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		classDecl->isExported = hasExport;
		classDecl->isAbstract = hasAbstract;

		// Consome 'class'.
		m_lexer->expectToken(TokenType_e::Class);

		// Consome o nome da classe.
		classDecl->identifier = m_lexer->expectIdentifier();

		// Verifica se a declaracao de generic.
		if (m_lexer->isLessThan())
		{
			classDecl->genericDecl = parseGenericDecl(ctx);
		}

		// Verifica se a declaracao de extends.
		if (m_lexer->isExtends())
		{
			// Consome 'extends'.
			m_lexer->nextToken();

			// Processa tipo.
			classDecl->baseClass = parseType(ctx);

			// Valida a base class.
			if (classDecl->baseClass->typeID != TypeDeclID_e::Named)
			{
				throw exceptions::custom_exception(
					"class '%s' extends must be a class element",
					m_lexer->getToken().line,
					m_lexer->getToken().column,
					classDecl->identifier.str()
				);
			}
		}

		// Verifica se a declaracao de interfaces.
		if (m_lexer->isImplements())
		{
			// Consome 'implements'.
			m_lexer->nextToken();

			while (true)
			{
				classDecl->interfaceList.push_back(parseType(ctx));

				// Valida a interface.
				if (classDecl->interfaceList.back()->typeID != TypeDeclID_e::Named)
				{
					throw exceptions::custom_exception(
						"class '%s' implements must be a interface element",
						m_lexer->getToken().line,
						m_lexer->getToken().column,
						classDecl->identifier.str()
					);
				}

				// Verifica se ha mais declaracoes.
				if (m_lexer->isComma())
				{
					m_lexer->nextToken();
					continue;
				}
				break;
			}
		}

		// Consome '{'.
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			Bool hasAccessModifier = false;
			TokenType_e accessModifier = TokenType_e::Unknown;
			Bool staticModifier = false;

			// Finaliza a declaracao do bloco.
			if (m_lexer->isRightBracket())
			{
				break;
			}

			// Processa declaracao de modificador de acesso.
			if (m_lexer->isPublic() || m_lexer->isProtected() || m_lexer->isPrivate())
			{
				hasAccessModifier = true;
				accessModifier = m_lexer->getToken().type;
				m_lexer->nextToken();
			}

			// Processa modificador static
			if (m_lexer->isStatic())
			{
				staticModifier = true;
				m_lexer->nextToken();
			}

			switch (m_lexer->getToken().type)
			{
			case TokenType_e::Abstract:
				if (staticModifier)
				{
					throw exceptions::custom_exception(
						"Static function can't be abstract",
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}
				goto processFunction;

			case TokenType_e::Virtual:
				if (staticModifier)
				{
					throw exceptions::custom_exception(
						"Static function can't be virtual",
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}
				goto processFunction;

			case TokenType_e::Fn:
			processFunction:
				classDecl->functionList.push_back(parseClassFunction(ctx, accessModifier, staticModifier));
				break;

			case TokenType_e::Const:
			case TokenType_e::Let:
				classDecl->variableList.push_back(parseClassVariable(ctx, accessModifier, staticModifier));
				break;

			case TokenType_e::Constructor:
				{
					if (staticModifier)
					{
						throw exceptions::custom_exception(
							"Constructors can't be static",
							m_lexer->getToken().line,
							m_lexer->getToken().column
						);
					}
					classDecl->constructorList.push_back(parseClassConstructor(ctx, accessModifier));
				}
				break;

			case TokenType_e::Destructor:
				{
					if (staticModifier)
					{
						throw exceptions::custom_exception(
							"Destructor can't be static",
							m_lexer->getToken().line,
							m_lexer->getToken().column
						);
					}

					if (hasAccessModifier)
					{
						throw exceptions::custom_exception(
							"Destructors are public, can't have any access modifier",
							m_lexer->getToken().line,
							m_lexer->getToken().column
						);
					}
					classDecl->destructorDecl = parseClassDestructor(ctx);
				}
				break;

			default:
				throw exceptions::unexpected_with_possibilities_token_exception(
					m_lexer->getToken().value,
					{
						TokenType_e::Public, TokenType_e::Protected, TokenType_e::Private,
						TokenType_e::Let, TokenType_e::Const,
						TokenType_e::Virtual, TokenType_e::Abstract, TokenType_e::Fn,
						TokenType_e::LBracket
					},
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}
		}

		// Consome '}'.
		m_lexer->expectToken(TokenType_e::RBracket);

		return classDecl;
	}

	std::unique_ptr<ast::InterfaceDecl>
	Parser::parseInterface(ParserContext_s& ctx, Bool hasExport)
	{
		auto interfaceDecl = std::make_unique<ast::InterfaceDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		interfaceDecl->isExported = hasExport;

		// Consome 'interface'.
		m_lexer->expectToken(TokenType_e::Interface);

		// Consome o identificador.
		interfaceDecl->identifier = m_lexer->expectIdentifier();

		// Consome generic se houver.
		if (m_lexer->isLessThan())
		{
			interfaceDecl->genericDecl = parseGenericDecl(ctx);
		}

		// Consome '{'.
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (m_lexer->isRightBracket())
			{
				break;
			}

		parseFunctionLabel:
			if (m_lexer->isFn())
			{				
				// Consome a funcao.
				interfaceDecl->functionDeclList.push_back(parserInterfaceFunction(ctx));
				goto parseFunctionLabel;
			}

			if (m_lexer->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{ TokenType_e::Fn, TokenType_e::RBracket },
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome '}'.
		m_lexer->expectToken(TokenType_e::RBracket);

		return interfaceDecl;
	}

	std::unique_ptr<ast::StructDecl>
	Parser::parseStruct(ParserContext_s& ctx, Bool hasExport)
	{
		auto structDecl = std::make_unique<ast::StructDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		structDecl->isExported = hasExport;

		// Consome 'struct'.
		m_lexer->expectToken(TokenType_e::Struct);

		// Consome o identificador.
		structDecl->identifier = m_lexer->expectIdentifier();

		// Consome generic se houver.
		if (m_lexer->isLessThan())
		{
			structDecl->genericDecl = parseGenericDecl(ctx);
		}

		// Consome '{'.
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (m_lexer->isRightBracket())
			{
				break;
			}

		parseVariableLabel:
			if (m_lexer->isLet() || m_lexer->isConst())
			{
				// Consome a funcao.
				structDecl->variableList.push_back(parseStructVariable(ctx));				
				goto parseVariableLabel;
			}

			if (m_lexer->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{ TokenType_e::Let, TokenType_e::Const, TokenType_e::RBracket },
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome '}'.
		m_lexer->expectToken(TokenType_e::RBracket);

		return structDecl;
	}

	std::unique_ptr<ast::TraitDecl>
	Parser::parseTrait(ParserContext_s& ctx, Bool hasExport)
	{
		Bool isDefinition = false;

		auto traitDecl = std::make_unique<ast::TraitDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		traitDecl->isExported = hasExport;

		// Consome 'trait'.
		m_lexer->expectToken(TokenType_e::Trait);

		// Consome o identificador.
		traitDecl->identifier = m_lexer->expectIdentifier();

		// Consome generic se houver.
		if (m_lexer->isLessThan())
		{
			traitDecl->genericDecl = parseGenericDecl(ctx);
		}

		// Verifica se e um definicao de trait.
		if (m_lexer->isFor())
		{
			traitDecl->isDefinition = true;

			// Consome 'for'.
			m_lexer->expectToken(TokenType_e::For);

			ctx.insideTrait = true;

			// Consome o tipo para qual o trait esta sendo implementado.
			traitDecl->typeDefinitionDecl = parseType(ctx);

			ctx.insideTrait = false;

			// Indica que e uma definicao, permitindo assim que tenho um bloco de codigo.
			isDefinition = true;
		}

		// Consome '{'.
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (m_lexer->isRightBracket())
			{
				break;
			}

		parseFunctionLabel:
			Bool isStatic = false;

			// Consome 'static'.
			if (m_lexer->isStatic())
			{
				m_lexer->expectToken(TokenType_e::Static);
				isStatic = true;
			}

			if (m_lexer->isFn())
			{
				// Consome a funcao.
				traitDecl->functionDeclList.push_back(parseTraitFunction(ctx, isDefinition, isStatic));
				goto parseFunctionLabel;
			}

			if (m_lexer->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{ TokenType_e::Fn, TokenType_e::Static, TokenType_e::RBracket },
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome '}'.
		m_lexer->expectToken(TokenType_e::RBracket);

		return traitDecl;
	}

	std::unique_ptr<ast::EnumDecl>
	Parser::parseEnum(ParserContext_s& ctx, Bool hasExport)
	{
		auto enumDecl = std::make_unique<ast::EnumDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		enumDecl->isExported = hasExport;

		// Consome 'enum'.
		m_lexer->expectToken(TokenType_e::Enum);

		// Consome o identificador.
		enumDecl->identifier = m_lexer->expectIdentifier();

		// Consome generic se houver.
		if (m_lexer->isLessThan())
		{
			enumDecl->genericDecl = parseGenericDecl(ctx);
		}

		// Consome '{'.
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (m_lexer->isRightBracket())
			{
				break;
			}

		parseEnumLabel:
			// Consome enum.
			enumDecl->enumItemDeclList.push_back(parseEnumItem(ctx));

			if (m_lexer->isComma())
			{				
				// Consome ','.
				m_lexer->expectToken(TokenType_e::Comma);
				goto parseEnumLabel;
			}

			if (m_lexer->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_token_exception(
				m_lexer->getToken().value,
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome '}'.
		m_lexer->expectToken(TokenType_e::RBracket);

		return enumDecl;
	}

	std::unique_ptr<ast::FunctionDecl>
	Parser::parseFunction(ParserContext_s& ctx, Bool hasExport)
	{
		auto functionPtr = std::make_unique<ast::FunctionDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		functionPtr->isExported = hasExport;

		// Consome 'fn'
		m_lexer->expectToken(TokenType_e::Fn);

		// Consome o identificador.
		functionPtr->identifier = m_lexer->expectIdentifier();

		// Consome o Generic.
		if (m_lexer->isLessThan())
		{
			functionPtr->genericDecl = parseGenericDecl(ctx);
		}

		// Consome os parametros.
		functionPtr->parameterList = parseFunctionParameters(ctx);

		// Consome o retorno se houver.
		if (m_lexer->isArrow())
		{
			// Consome '->'
			m_lexer->expectToken(TokenType_e::Arrow);

			// Consome o tipo retorno.
			functionPtr->returnType = parseType(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			functionPtr->returnType = std::make_unique<ast::TypeDeclVoid>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome bloco se houver.
		functionPtr->blockDecl = parseBlock(ctx);
		
		return functionPtr;
	}

	std::unique_ptr<ast::VariableDecl>
	Parser::parseVariable(ParserContext_s& ctx, Bool hasExport)
	{
		auto variableDecl = std::make_unique<ast::VariableDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		variableDecl->isExported = hasExport;

		// Consome 'let' ou 'const'
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Let:
			m_lexer->expectToken(TokenType_e::Let);
			variableDecl->isConst = false;
			break;
		case TokenType_e::Const:
			m_lexer->expectToken(TokenType_e::Const);
			variableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{ TokenType_e::Let, TokenType_e::Const },
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Verifica se o modificador 'ref' foi declarado.
		if (m_lexer->isRef())
		{
			// Consome 'ref'.
			m_lexer->expectToken(TokenType_e::Ref);
			variableDecl->isReference = true;
		}
		else if (m_lexer->isShared())
		{
			// Consome 'shared'.
			m_lexer->expectToken(TokenType_e::Shared);
			variableDecl->isShared = true;
		}

		// Consome identificador.
		variableDecl->identifier = m_lexer->expectIdentifier();

		Bool mustHaveInitExpression = variableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (m_lexer->isColon())
		{
			// Consome ':'.
			m_lexer->expectToken(TokenType_e::Colon);

			const U32 line = m_lexer->getToken().line;
			const U32 column = m_lexer->getToken().column;

			// Consome o tipo.
			variableDecl->typeDecl = parseType(ctx);

			// Verifica se o tipo e valido.
			if (variableDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Variables or constant can't have void type",
					line,
					column
				);
			}
		} else {
			// Se o tipo nao e declarado explicitamente a variavel deve ser iniciado.
			mustHaveInitExpression = true;
		}

		// Consome a expressao de inicializacao.
		if (mustHaveInitExpression)
		{
			// Consome '='.
			m_lexer->expectToken(TokenType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			variableDecl->initExpression = parseExpression(ctx, OperatorPrecLevel_e::Interrogation);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return variableDecl;
	}

	std::unique_ptr<ast::GenericDecl>
	Parser::parseGenericDecl(ParserContext_s& ctx)
	{
		std::unique_ptr<ast::GenericDecl> templateDecl = std::make_unique<ast::GenericDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome '<'
		m_lexer->expectToken(TokenType_e::LessThan);

		while (true)
		{
			auto genericDecl = std::make_unique<ast::GenericItemDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Consome o identificador.
			genericDecl->identifier = m_lexer->expectIdentifier();

			// Processa a clausula where.
			if (m_lexer->isColon())
			{
				// Consome ':'
				m_lexer->expectToken(TokenType_e::Colon);

				// Consome 'where'
				m_lexer->expectToken(TokenType_e::Where);

				const U32 line = m_lexer->getToken().line;
				const U32 column = m_lexer->getToken().column;

				// Consome o identificador
				auto identifier = m_lexer->expectIdentifier();

				// Verifica se a declaracao esta correta, o identificador
				// apos o where deve ser igual o identificador declarado no generic.
				if (genericDecl->identifier != identifier)
				{
					throw exceptions::custom_exception(
						"Where identifier must be like generic identifier: %s",
						line,
						column,
						genericDecl->identifier.str()
					);
				}

				// Consome 'is'
				m_lexer->expectToken(TokenType_e::Is);

				while (true)
				{
					auto whereTypeDecl = parseType(ctx);

					if (whereTypeDecl->nullable)
					{
						throw exceptions::custom_exception(
							"Where types can't be nullable",
							line,
							column
						);
					}

					genericDecl->whereTypeList.push_back(std::move(whereTypeDecl));

					// Verifica se a outras declaracoes.
					if (m_lexer->isBitWiseOr())
					{
						// Consome '|'
						m_lexer->expectToken(TokenType_e::BitWiseOr);
						continue;
					}
					break;
				}
			}

			// Adiciona o generic a lista.
			templateDecl->genericDeclItemList.push_back(
				std::move(genericDecl)
			);

			// Consome ',' e processa proxima declaracao.
			if (m_lexer->isComma())
			{
				m_lexer->expectToken(TokenType_e::Comma);
				continue;
			}

			// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
			// serao quebrados em tokens menores.
			if (m_lexer->isBitWiseRightShift() || m_lexer->isBitWiseRightShiftAssign())
			{
				switch (m_lexer->getToken().type)
				{
				case TokenType_e::BitWiseRShift:		// >>
				case TokenType_e::GreaterThanOrEqual:	// >=
				case TokenType_e::BitWiseRShiftAssign:	// >>=
					m_lexer->reinterpretToken(
						TokenType_e::GreaterThan,
						1
					);
					break;
				default:
					break;
				}
			}

			// Consome '>'
			m_lexer->expectToken(TokenType_e::GreaterThan);
			break;
		}
		return templateDecl;
	}

	std::unique_ptr<ast::TypeDecl>
	Parser::parseType(ParserContext_s& ctx)
	{
		std::unique_ptr<ast::TypeDecl> typeDecl;

		const U32 line = m_lexer->getToken().line;
		const U32 column = m_lexer->getToken().column;

		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		TokenType_e type = TokenType_e::Unknown;

		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Void:
			type = TokenType_e::Void;
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclVoid>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
			break;
		case TokenType_e::I8:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI8>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::U8:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU8>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::I16:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI16>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::U16:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU16>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::I32:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI32>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::U32:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU32>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::I64:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI64>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::U64:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU64>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::Fp32:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclFp32>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::Fp64:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclFp64>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::String:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclString>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
				);
			break;
		case TokenType_e::Object:
			m_lexer->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclObject>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
			break;
		case TokenType_e::Fn:
			if (skipOnly) {
				parseFunctionType(ctx);
				break;
			}
			typeDecl = parseFunctionType(ctx);
			break;
		case TokenType_e::LParBracket:
			if (skipOnly) {
				parseTupleType(ctx);
				break;
			}
			typeDecl = parseTupleType(ctx);
			break;
		case TokenType_e::Self:
			if (ctx.insideTrait)
			{
				m_lexer->nextToken();
				if (skipOnly) { break; }
				typeDecl = std::make_unique<ast::SelfTypeDecl>(
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}
			else
			{
				throw exceptions::custom_exception(
					"Self type only can be declared in traits",
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}
			break;
		default:
			// NamedType
			if (m_lexer->isIdentifier() || m_lexer->isScopeResolution())
			{
				if (skipOnly) {
					parseNamedType(ctx);
					break;
				}
				typeDecl = parseNamedType(ctx);
				break;
			}

			throw exceptions::unexpected_token_exception(
				m_lexer->getToken().value,
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Verifica se o tipo e anulavel.
		if (m_lexer->isInterrogation())
		{
			if (skipOnly)
			{
				if (type == TokenType_e::Void)
				{
					throw exceptions::custom_exception("'void' type can't be nullable",
						line,
						column
					);
				}
			}

			if (typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception("'void' type can't be nullable",
					line,
					column
				);
			}

			// Consome '?'
			m_lexer->expectToken(TokenType_e::Interrogation);

			typeDecl->nullable = true;
		}

		// Verifica se e um array
		if (m_lexer->isLeftSquBracket())
		{
			if (skipOnly)
			{
				while (true)
				{
					if (!m_lexer->isLeftSquBracket())
					{
						break;
					}
					parseArrayDecl(ctx);
				}
				if (m_lexer->isInterrogation())
				{
					// Consome '?'
					m_lexer->expectToken(TokenType_e::Interrogation);
				}
				return nullptr;
			}

			auto arrayType = std::make_unique<ast::TypeDeclArray>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Adiciona o tipo base ao tipo array
			arrayType->valueType = std::move(typeDecl);

			while (true)
			{
				if (!m_lexer->isLeftSquBracket())
				{
					break;
				}
				arrayType->arrayDeclList.push_back(parseArrayDecl(ctx));
			}

			// Verifica se o array e anulavel.
			if (m_lexer->isInterrogation())
			{
				// Consome '?'
				m_lexer->expectToken(TokenType_e::Interrogation);

				arrayType->nullable = true;
			}
			typeDecl = std::move(arrayType);
		}
		if (skipOnly) {
			return nullptr;
		}
		return typeDecl;
	}

	std::unique_ptr<ast::BlockDecl>
	Parser::parseBlock(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly) {
			const U32 line = m_lexer->getToken().line;
			const U32 column = m_lexer->getToken().column;

			const U32 beginPosition = m_lexer->getToken().position;

			// Consome '{'
			m_lexer->expectToken(TokenType_e::LBracket);

			while (true)
			{
				if (m_lexer->isRightBracket())
				{
					break;
				}

				parseStmtDecl(ctx);
			}

			// Consome '}'
			m_lexer->expectToken(TokenType_e::RBracket);

			const U32 endPosition = m_lexer->getToken().position;

			return std::make_unique<ast::BlockDecl>(
				beginPosition,
				endPosition,
				line,
				column
			);
		}

		auto blockDecl = std::make_unique<ast::BlockDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome '{'
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (m_lexer->isRightBracket())
			{
				break;
			}

			blockDecl->stmtList.push_back(parseStmtDecl(ctx));
		}

		// Consome '}'
		m_lexer->expectToken(TokenType_e::RBracket);

		return blockDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseStmtDecl(ParserContext_s& ctx)
	{
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::If:
			{
				// Verifica se e um 'if' ou um 'if let'.
				if (m_lexer->predictNextToken().type == TokenType_e::Let)
				{
					return parseIfLet(ctx);
				}
				return parseIf(ctx);
			}
			break;
		case TokenType_e::For:
			return parseFor(ctx);
		case TokenType_e::While:
			return parseWhile(ctx);
		case TokenType_e::Do:
			return parseDoWhile(ctx);
		case TokenType_e::Match:
			return parseMatch(ctx);
		case TokenType_e::Return:
			return parseReturn(ctx);
		case TokenType_e::Continue:
			return parseContinue(ctx);
		case TokenType_e::Break:
			return parseBreak(ctx);
		case TokenType_e::Goto:
			return parseGoto(ctx);
		case TokenType_e::Try:
			return parseTry(ctx);
		case TokenType_e::Panic:
			return parsePanic(ctx);
		case TokenType_e::Let:
		case TokenType_e::Const:
			return parseVariable(ctx);
		default:	// Label, Expr
			{
				// Parse label.
				if (m_lexer->isIdentifier())
				{
					if (m_lexer->predictNextToken().type == TokenType_e::Colon)
					{
						return parseLabel(ctx);
					}
				}
				return parseExprStmt(ctx);
			}
			break;
		}
		return nullptr;
	}

	std::unique_ptr<ast::expr::ExpressionDecl>
	Parser::parseExpression(ParserContext_s& ctx, OperatorPrecLevel_e prec)
	{
		std::unique_ptr<ast::expr::ExpressionDecl> expr;

		ctx.insideExpr = true;

		if (ctx.isFirstPass)
		{
			const U32 line = m_lexer->getToken().line;
			const U32 column = m_lexer->getToken().column;
			const U32 beginPosition = m_lexer->getToken().position;
			parseExpressionImp(ctx, prec);
			const U32 endPosition = m_lexer->getToken().position;
			expr = std::make_unique<ast::expr::ExpressionDeclMark>(line, column, beginPosition, endPosition);
		}
		else
		{
			expr = parseExpressionImp(ctx, prec);
		}

		ctx.insideExpr = false;

		return expr;
	}

	std::unique_ptr<ast::pattern::PatternDecl>
	Parser::parsePattern(ParserContext_s& ctx)
	{
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::ConstantInteger:
			return parseLiteralPattern(ctx);
		case TokenType_e::Identifier:
			{
				U32 position = m_lexer->getToken().position;

				// Verifica se pode ser um enum destructuring.
				while (true)
				{
					if (m_lexer->isScopeResolution())
					{
						m_lexer->nextToken();
						continue;
					}

					if (m_lexer->isIdentifier())
					{
						m_lexer->nextToken();
						continue;
					}

					if (m_lexer->isLeftParBracket())
					{
						m_lexer->resetToPosition(position);
						return parseEnumPattern(ctx);
					}
					m_lexer->resetToPosition(position);
					break;
				}
				return parseLiteralPattern(ctx);
			}
			break;
		case TokenType_e::LParBracket:
			return parseTuplePattern(ctx);
		case TokenType_e::LBracket:
			return parseClassOrStructPattern(ctx);
		default:
			throw exceptions::not_implemented_feature_exception("pattern matching");
		}
		return nullptr;
	}

	std::vector<std::unique_ptr<ast::FunctionParameterDecl>>
	Parser::parseFunctionParameters(ParserContext_s& ctx)
	{
		std::vector<std::unique_ptr<ast::FunctionParameterDecl>> functionParameters;

		// Consome '('
		m_lexer->expectToken(TokenType_e::LParBracket);

		// Finaliza declaracao da lista de parametros.
		if (m_lexer->isRightParBracket())
		{
			// Consome ')'
			m_lexer->expectToken(TokenType_e::RParBracket);
			return functionParameters;
		}

		while (true)
		{
			Bool isReference = false;
			Bool isShared = false;

			if (m_lexer->isRef())
			{
				m_lexer->expectToken(TokenType_e::Ref);
				isReference = true;
			}
			else if (m_lexer->isShared())
			{
				m_lexer->expectToken(TokenType_e::Shared);
				isShared = true;
			}

			if (m_lexer->isIdentifier())
			{
				auto parameterDecl = std::make_unique<ast::FunctionParameterDecl>(
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);

				parameterDecl->isReference = isReference;
				parameterDecl->isShared = isShared;
				parameterDecl->identifier = m_lexer->expectIdentifier();

				// Verifica se e uma ellipsis
				if (m_lexer->isEllipsis())
				{
					parameterDecl->isEllipsis = true;
					m_lexer->expectToken(TokenType_e::Ellipsis);
					break;
				}

				// Consome ':'
				m_lexer->expectToken(TokenType_e::Colon);

				// Consome tipo
				parameterDecl->typeDecl = parseType(ctx);

				// Parametros nao podem ser do tipo void.
				if (parameterDecl->typeDecl->typeID == TypeDeclID_e::Void)
				{
					throw exceptions::custom_exception(
						"Parameter '%s' can't have void type",
						parameterDecl->typeDecl->line,
						parameterDecl->typeDecl->column,
						parameterDecl->identifier.str()
					);
				}

				// Adiciona o parametro a lista.
				functionParameters.push_back(std::move(parameterDecl));
			}
			else if (m_lexer->isLeftBracket())
			{
				auto parameterDecl = std::make_unique<ast::FunctionParameterDecl>(
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);

				parameterDecl->isReference = isReference;

				// Consome destructuring.
				parameterDecl->destructuringPatternDecl = parsePattern(ctx);

				// Consome ':'
				m_lexer->expectToken(TokenType_e::Colon);

				// Consome tipo
				// TODO: Habilitar o modo trait.
				parameterDecl->typeDecl = parseType(ctx);

				// Parametros nao podem ser do tipo void.
				if (parameterDecl->typeDecl->typeID == TypeDeclID_e::Void)
				{
					throw exceptions::custom_exception(
						"Parameter can't have void type",
						parameterDecl->typeDecl->line,
						parameterDecl->typeDecl->column
					);
				}

				// Adiciona o parametro a lista.
				functionParameters.push_back(std::move(parameterDecl));
			}

			// Consome ','
			if (m_lexer->isComma())
			{
				m_lexer->expectToken(TokenType_e::Comma);
				continue;
			}

			// Finaliza declaracao da lista de parametros.
			if (m_lexer->isRightParBracket())
			{
				break;
			}

			throw exceptions::custom_exception("Expected ',' or ')' token",
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome ')'
		m_lexer->expectToken(TokenType_e::RParBracket);

		return functionParameters;
	}

	std::unique_ptr<ast::ScopedIdentifierDecl>
	Parser::parseScopedIdentifier(ParserContext_s& ctx)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Se o escopo vem antes identificador
		if (m_lexer->isScopeResolution())
		{
			// Consome '::'.
			m_lexer->expectToken(TokenType_e::ScopeResolution);
			scopedIdentifierDecl->startFromRoot = true;
		}

		// Consome o identificador.
		scopedIdentifierDecl->identifier = m_lexer->expectIdentifier();

		// Verifica se a mais declaracoes de escopo.
		if (m_lexer->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = parseChildScopedIdentifiers(ctx);
		}
		return scopedIdentifierDecl;
	}

	std::unique_ptr<ast::GeneralStmtDecl>
	Parser::parseGeneralStmt(ParserContext_s& ctx)
	{
		Bool hasExport = false;
		Bool hasAbtract = false;

		// Verifica se houve a declararao para exportar o elemento.
		if (m_lexer->isExport())
		{
			// Consome 'export'.
			m_lexer->expectToken(TokenType_e::Export);
			hasExport = true;
		}

		// Verifica se a declaracao para a classe ser abstrata.
		if (m_lexer->isAbstract())
		{
			// Consome 'abtract'.
			m_lexer->expectToken(TokenType_e::Abstract);
			hasAbtract = true;

			// Obrigatoriamente 'abstract' deve se referir a uma classe.
			if (!m_lexer->isClass()) {
				throw exceptions::unexpected_token_exception(
					m_lexer->getToken().value,
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}
		}

		// export, abstract, class, interface, struct, enum, trait, let, fn

		// Verifica qual declaracao processar.
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Class:
			return parseClass(ctx, hasExport, hasAbtract);
		case TokenType_e::Interface:
			return parseInterface(ctx, hasExport);
		case TokenType_e::Struct:
			return parseStruct(ctx, hasExport);
		case TokenType_e::Enum:
			return parseEnum(ctx, hasExport);
		case TokenType_e::Trait:
			return parseTrait(ctx, hasExport);
		case TokenType_e::Fn:
			return parseFunction(ctx, hasExport);
		case TokenType_e::Const:
		case TokenType_e::Let:
			return parseVariable(ctx, hasExport);
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{
					TokenType_e::Class,
					TokenType_e::Interface,
					TokenType_e::Struct,
					TokenType_e::Enum,
					TokenType_e::Trait,
					TokenType_e::Let,
					TokenType_e::Const,
					TokenType_e::Fn
				},
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		return nullptr;
	}

	std::unique_ptr<ast::ClassFunctionDecl>
	Parser::parseClassFunction(ParserContext_s& ctx, TokenType_e accessModifier, Bool staticModifier)
	{
		auto classFunctionDecl = std::make_unique<ast::ClassFunctionDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Atribui modificador estatico.
		classFunctionDecl->isStatic = staticModifier;

		// Processa modificador 'abtract' ou 'virtual'.
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Abstract:
			m_lexer->expectToken(TokenType_e::Abstract);
			classFunctionDecl->isAbstract = true;
			break;
		case TokenType_e::Virtual:
			m_lexer->expectToken(TokenType_e::Virtual);
			classFunctionDecl->isVirtual = true;
			break;
		default:
			break;
		}

		// Atribui modificadores de acesso.
		switch (accessModifier) {
		case TokenType_e::Protected:
			classFunctionDecl->accessModifier = ClassMemberAccessModifier_e::Protected;
			break;
		case TokenType_e::Private:
		case TokenType_e::Unknown:
			classFunctionDecl->accessModifier = ClassMemberAccessModifier_e::Private;
			break;
		case TokenType_e::Public:
			classFunctionDecl->accessModifier = ClassMemberAccessModifier_e::Public;
			break;
		default:
			break;
		}

		// Consome 'fn'
		m_lexer->expectToken(TokenType_e::Fn);

		// Verifica se ate o token '(' existe algum '.', se existir significa que se trata
		// de uma especificacao de desambiguicao ou seja indica onde a fonte da declaracao da funcao
		// e segue a seguint sintaxe: <tipo> '.'
		Bool	hasSourceTypeDecl = false;
		U32		position = m_lexer->getToken().position;

		while (!m_lexer->isLeftParBracket()) {
			if (m_lexer->isDot())
			{
				hasSourceTypeDecl = true;
				break;
			}
			m_lexer->nextToken();
		}
		m_lexer->resetToPosition(position);

		// Consome o <tipo> seguido de '.'
		if (hasSourceTypeDecl)
		{
			classFunctionDecl->sourceTypeDecl = parseType(ctx);
			m_lexer->expectToken(TokenType_e::Dot);
		}

		// Consome o identificador.
		classFunctionDecl->identifier = m_lexer->expectIdentifier();

		// Consome o Generic.
		if (m_lexer->isLessThan())
		{
			classFunctionDecl->genericDecl = parseGenericDecl(ctx);
		}

		// Consome os parametros.
		classFunctionDecl->parameterList = parseFunctionParameters(ctx);

		// Consome o retorno se houver.
		if (m_lexer->isArrow())
		{
			// Consome '->'
			m_lexer->expectToken(TokenType_e::Arrow);

			// Consome o tipo retorno.
			classFunctionDecl->returnType = parseType(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			classFunctionDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Override:
			// Funcoes abstratas nao podem ser override ou final
			if (classFunctionDecl->isAbstract)
			{
				throw exceptions::custom_exception(
					"Abstract function '%s' can't have override or final modifiers",
					m_lexer->getToken().line,
					m_lexer->getToken().column,
					classFunctionDecl->identifier.str()
				);
			}
			m_lexer->expectToken(TokenType_e::Override);
			classFunctionDecl->isOverride = true;
			break;

		case TokenType_e::Final:
			// Funcoes abstratas nao podem ser override ou final
			if (classFunctionDecl->isAbstract)
			{
				throw exceptions::custom_exception(
					"Abstract function '%s' can't have override or final modifiers",
					m_lexer->getToken().line,
					m_lexer->getToken().column,
					classFunctionDecl->identifier.str()
				);
			}
			m_lexer->expectToken(TokenType_e::Final);
			classFunctionDecl->isFinal = true;
			break;
		default:
			break;
		}

		if (classFunctionDecl->isAbstract)
		{
			// Consome ';'
			m_lexer->expectToken(TokenType_e::SemiColon);
			return classFunctionDecl;
		}

		classFunctionDecl->blockDecl = parseBlock(ctx);

		return classFunctionDecl;
	}

	std::unique_ptr<ast::ClassVariableDecl>
	Parser::parseClassVariable(ParserContext_s& ctx, TokenType_e accessModifier, Bool staticModifier)
	{
		auto classVariableDecl = std::make_unique<ast::ClassVariableDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Define a variavel como estatica.
		classVariableDecl->isStatic = staticModifier;

		// Atribui modificadores de acesso.
		switch (accessModifier) {
		case TokenType_e::Protected:
			classVariableDecl->accessModifier = ClassMemberAccessModifier_e::Protected;
			break;
		case TokenType_e::Private:
		case TokenType_e::Unknown:
			classVariableDecl->accessModifier = ClassMemberAccessModifier_e::Private;
			break;
		case TokenType_e::Public:
			classVariableDecl->accessModifier = ClassMemberAccessModifier_e::Public;
			break;
		default:
			break;
		}

		// Consome 'let' ou 'const'
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Let:
			m_lexer->expectToken(TokenType_e::Let);
			classVariableDecl->isConst = false;
			break;
		case TokenType_e::Const:
			m_lexer->expectToken(TokenType_e::Const);
			classVariableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{ TokenType_e::Let, TokenType_e::Const },
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Verifica se o modificador 'ref' foi declarado.
		if (m_lexer->isRef())
		{
			// Consome 'ref'.
			m_lexer->expectToken(TokenType_e::Ref);
			classVariableDecl->isReference = true;
		}
		else if (m_lexer->isShared())
		{
			// Consome 'shared'.
			m_lexer->expectToken(TokenType_e::Shared);
			classVariableDecl->isShared = true;
		}

		// Consome identificador.
		classVariableDecl->identifier = m_lexer->expectIdentifier();

		Bool mustHaveInitExpression = classVariableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (m_lexer->isColon())
		{
			// Consome ':'.
			m_lexer->expectToken(TokenType_e::Colon);

			const U32 line = m_lexer->getToken().line;
			const U32 column = m_lexer->getToken().column;

			// Consome o tipo.
			classVariableDecl->typeDecl = parseType(ctx);

			// Verifica se o tipo e valido.
			if (classVariableDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Variables or constant can't have void type",
					line,
					column
				);
			}
		} else {
			// Se o tipo nao e declarado explicitamente a variavel deve ser iniciado.
			mustHaveInitExpression = true;
		}

		// Consome a expressao de inicializacao.
		if (mustHaveInitExpression)
		{
			// Consome '='.
			m_lexer->expectToken(TokenType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			classVariableDecl->initExpression = parseExpression(ctx, OperatorPrecLevel_e::Interrogation);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return classVariableDecl;
	}

	std::unique_ptr<ast::ClassConstructorDecl>
	Parser::parseClassConstructor(ParserContext_s& ctx, TokenType_e accessModifier)
	{
		auto classConstructorDecl = std::make_unique<ast::ClassConstructorDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);
		
		// Consome 'constructor'.
		m_lexer->expectToken(TokenType_e::Constructor);

		// Consome os parametros.
		classConstructorDecl->parameterList = parseFunctionParameters(ctx);

		// Verifica se a declaracao do construtor base.
		if (m_lexer->isColon())
		{
			// Consome ':'.
			m_lexer->expectToken(TokenType_e::Colon);

			if (m_lexer->isSuper())
			{
				// Consome 'super'.
				m_lexer->expectToken(TokenType_e::Super);

				// Consome '('.
				m_lexer->expectToken(TokenType_e::LParBracket);

				// Consome os parametros.
				classConstructorDecl->superParameters = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

				// Consome ')'.
				m_lexer->expectToken(TokenType_e::RParBracket);
			} else {
				auto variableInitDecl = std::make_unique<ast::ClassVariableInitDecl>(
					m_lexer->getToken().line,
					m_lexer->getToken().column
					);

				// Consome o identificador
				variableInitDecl->identifier = m_lexer->expectIdentifier();

				// Consome '('.
				m_lexer->expectToken(TokenType_e::LParBracket);

				// Consome expressao.
				variableInitDecl->initExpression = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

				// Consome ')'.
				m_lexer->expectToken(TokenType_e::RParBracket);

				// Adiciona inicializacao de variavel a lista.
				classConstructorDecl->variableInitDeclList.push_back(std::move(variableInitDecl));
			}

			// Consome inicializacao e variaveis.
			while (true)
			{
				if (!m_lexer->isComma())
				{
					break;
				}

				// Consome ','.
				m_lexer->expectToken(TokenType_e::Comma);

				auto variableInitDecl = std::make_unique<ast::ClassVariableInitDecl>(
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);

				// Consome o identificador
				variableInitDecl->identifier = m_lexer->expectIdentifier();

				// Consome '('.
				m_lexer->expectToken(TokenType_e::LParBracket);

				// Consome expressao.
				variableInitDecl->initExpression = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

				// Consome ')'.
				m_lexer->expectToken(TokenType_e::RParBracket);

				// Adiciona inicializacao de variavel a lista.
				classConstructorDecl->variableInitDeclList.push_back(std::move(variableInitDecl));
			}
		}

		// Console bloco.
		classConstructorDecl->blockDecl = parseBlock(ctx);

		return classConstructorDecl;
	}

	std::unique_ptr<ast::ClassDestructorDecl>
	Parser::parseClassDestructor(ParserContext_s& ctx)
	{
		auto classDestructorDecl = std::make_unique<ast::ClassDestructorDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'destructor'.
		m_lexer->expectToken(TokenType_e::Destructor);

		// Consome '('.
		m_lexer->expectToken(TokenType_e::LParBracket);

		// Consome ')'.
		m_lexer->expectToken(TokenType_e::RParBracket);

		// Console bloco.
		classDestructorDecl->blockDecl = parseBlock(ctx);

		return classDestructorDecl;
	}

	std::unique_ptr<ast::InterfaceFunctionDecl>
	Parser::parserInterfaceFunction(ParserContext_s& ctx)
	{
		auto interfaceFunctionDecl = std::make_unique<ast::InterfaceFunctionDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'fn'
		m_lexer->expectToken(TokenType_e::Fn);

		// Consome o identificador.
		interfaceFunctionDecl->identifier = m_lexer->expectIdentifier();

		// Consome o Generic.
		if (m_lexer->isLessThan())
		{
			interfaceFunctionDecl->genericDecl = parseGenericDecl(ctx);
		}

		// Consome os parametros.
		interfaceFunctionDecl->parameterList = parseFunctionParameters(ctx);

		// Consome o retorno se houver.
		if (m_lexer->isArrow())
		{
			// Consome '->'
			m_lexer->expectToken(TokenType_e::Arrow);

			// Consome o tipo retorno.
			interfaceFunctionDecl->returnType = parseType(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			interfaceFunctionDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome ';'
		m_lexer->expectToken(TokenType_e::SemiColon);

		return interfaceFunctionDecl;
	}

	std::unique_ptr<ast::StructVariableDecl>
	Parser::parseStructVariable(ParserContext_s& ctx)
	{
		auto structVariableDecl = std::make_unique<ast::StructVariableDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Let:
			m_lexer->expectToken(TokenType_e::Let);
			structVariableDecl->isConst = false;
			break;
		case TokenType_e::Const:
			m_lexer->expectToken(TokenType_e::Const);
			structVariableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{ TokenType_e::Let, TokenType_e::Const },
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Verifica se possui o identificador 'ref'.
		if (m_lexer->isRef())
		{
			// Consome 'ref'.
			m_lexer->expectToken(TokenType_e::Ref);
			structVariableDecl->isReference = true;
		}
		else if (m_lexer->isShared())
		{
			m_lexer->expectToken(TokenType_e::Shared);
			structVariableDecl->isShared = true;
		}

		// Consome identificador.
		structVariableDecl->identifier = m_lexer->expectIdentifier();

		Bool mustHaveInitExpression = structVariableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (m_lexer->isColon())
		{
			// Consome ':'.
			m_lexer->expectToken(TokenType_e::Colon);

			const U32 line = m_lexer->getToken().line;
			const U32 column = m_lexer->getToken().column;

			// Consome o tipo.
			structVariableDecl->typeDecl = parseType(ctx);

			// Verifica se o tipo e valido.
			if (structVariableDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Variables or constant can't have void type",
					line,
					column
				);
			}
		} else {
			// Se o tipo nao e declarado explicitamente a variavel deve ser iniciado.
			mustHaveInitExpression = true;
		}

		// Consome a expressao de inicializacao.
		if (mustHaveInitExpression)
		{
			// Consome '='.
			m_lexer->expectToken(TokenType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			structVariableDecl->initExpression = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return structVariableDecl;
	}

	std::unique_ptr<ast::TraitFunctionDecl>
	Parser::parseTraitFunction(ParserContext_s& ctx, Bool isDefinition, Bool isStatic)
	{
		auto traitFunctionDecl = std::make_unique<ast::TraitFunctionDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		traitFunctionDecl->isStatic = isStatic;

		// Consome 'fn'
		m_lexer->expectToken(TokenType_e::Fn);

		// Consome o identificador.
		traitFunctionDecl->identifier = m_lexer->expectIdentifier();

		// Consome o Generic.
		if (m_lexer->isLessThan())
		{
			traitFunctionDecl->genericDecl = parseGenericDecl(ctx);
		}

		ctx.insideTrait = true;

		// Consome os parametros.		
		traitFunctionDecl->parameterList = parseFunctionParameters(ctx);

		ctx.insideTrait = false;

		// Consome o retorno se houver.
		if (m_lexer->isArrow())
		{
			// Consome '->'
			m_lexer->expectToken(TokenType_e::Arrow);
			
			ctx.insideTrait = true;

			// Consome o tipo retorno.
			traitFunctionDecl->returnType = parseType(ctx);

			ctx.insideTrait = false;
		}
		else
		{
			// Consome o tipo retorno.
			traitFunctionDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// No modo de definicao e necessario implementar o bloco da funcao.
		if (isDefinition) {
			ctx.insideTrait = true;

			// Processa bloco.
			traitFunctionDecl->blockDecl = parseBlock(ctx);

			ctx.insideTrait = false;
		} else {
			// Consome ';'
			m_lexer->expectToken(TokenType_e::SemiColon);
		}
		return traitFunctionDecl;
	}

	std::unique_ptr<ast::EnumItemDecl>
	Parser::parseEnumItem(ParserContext_s& ctx)
	{
		auto enumItemDecl = std::make_unique<ast::EnumItemDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome o identificador.
		enumItemDecl->identifier = m_lexer->expectIdentifier();

		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Assign:
			{
				// Consome '='.
				m_lexer->expectToken(TokenType_e::Assign);
				enumItemDecl->hasValue = true;
				enumItemDecl->valueExpression = parseExpression(ctx, OperatorPrecLevel_e::EnumExpr);
			}
			break;
		case TokenType_e::LParBracket:
			{
				// Consome '('.
				m_lexer->expectToken(TokenType_e::LParBracket);

				// Indica presenca de dados.
				enumItemDecl->hasData = true;

				// Processa tipos.
				while (true)
				{
					// Consome Tipo
					enumItemDecl->dataTypeDeclList.push_back(parseType(ctx));

					if (m_lexer->isComma())
					{
						// Consome ','.
						m_lexer->expectToken(TokenType_e::Comma);
						continue;
					}

					if (m_lexer->isRightParBracket())
					{
						break;
					}

					throw exceptions::unexpected_with_possibilities_token_exception(
						m_lexer->getToken().value,
						{ TokenType_e::Comma, TokenType_e::RParBracket },
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}				

				// Consome ')'.
				m_lexer->expectToken(TokenType_e::RParBracket);
			}
			break;
		case TokenType_e::Comma:
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{ TokenType_e::Assign, TokenType_e::LParBracket, TokenType_e::Comma },
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		return enumItemDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseIf(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::If);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			if (m_lexer->isElse()) {
				m_lexer->expectToken(TokenType_e::Else);
				parseBlock(ctx);
			}
			return nullptr;
		}

		auto ifDecl = std::make_unique<ast::StmtIfDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'if'.
		m_lexer->expectToken(TokenType_e::If);

		// Consome expressao.
		ifDecl->conditionExprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome o block if.
		ifDecl->ifBlockDecl = parseBlock(ctx);

		// Verifica se existe o bloco else.
		if (m_lexer->isElse())
		{
			// Consome 'else'.
			m_lexer->expectToken(TokenType_e::Else);

			// Consome o block else.
			ifDecl->elseBlockDecl = parseBlock(ctx);
		}

		return ifDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseIfLet(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::If);
			m_lexer->expectToken(TokenType_e::Let);
			parsePattern(ctx);
			m_lexer->expectToken(TokenType_e::Assign);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			parseBlock(ctx);
			if (m_lexer->isElse()) {
				m_lexer->expectToken(TokenType_e::Else);
				parseBlock(ctx);
			}
			return nullptr;
		}

		auto ifLefDecl = std::make_unique<ast::StmtIfLetDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'if'.
		m_lexer->expectToken(TokenType_e::If);

		// Consome 'let'.
		m_lexer->expectToken(TokenType_e::Let);

		// Consome pattern.
		ifLefDecl->patternDecl = parsePattern(ctx);

		// Consome '='.
		m_lexer->expectToken(TokenType_e::Assign);

		// Consome expressao.
		ifLefDecl->expressionDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome o block if.
		ifLefDecl->ifBlockDecl = parseBlock(ctx);

		// Verifica se existe o bloco else.
		if (m_lexer->isElse())
		{
			// Consome 'else'.
			m_lexer->expectToken(TokenType_e::Else);

			// Consome o block else.
			ifLefDecl->elseBlockDecl = parseBlock(ctx);
		}
		return ifLefDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseFor(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::For);
			if (m_lexer->isLet())
			{
				m_lexer->expectIdentifier();
				m_lexer->expectToken(TokenType_e::Assign);
				parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			} else {
				parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			}
			m_lexer->expectToken(TokenType_e::SemiColon);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			m_lexer->expectToken(TokenType_e::SemiColon);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			parseBlock(ctx);
			return nullptr;
		}

		auto forDecl = std::make_unique<ast::StmtForDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);
		
		// Consome 'for'.
		m_lexer->expectToken(TokenType_e::For);

		// Verifica se e uma expressao e inicilizacao.
		if (m_lexer->isLet())
		{
			forDecl->initStmtDecl = std::make_unique<ast::StmtForInitDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Consome o identificador.
			forDecl->initStmtDecl->identifier = m_lexer->expectIdentifier();

			// Consome '='.
			m_lexer->expectToken(TokenType_e::Assign);

			// Consome expressao.
			forDecl->initStmtDecl->initExpression = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
		} else {
			// Consome expressao.
			forDecl->initExprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
		}

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		// Consome expressao.
		forDecl->conditionExprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		// Consome expressao.
		forDecl->updateExprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome bloco.
		forDecl->blockDecl = parseBlock(ctx);

		return forDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseWhile(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::While);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			parseBlock(ctx);
			return nullptr;
		}

		auto whileDecl = std::make_unique<ast::StmtWhileDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'while'.
		m_lexer->expectToken(TokenType_e::While);

		// Consome expressao
		whileDecl->conditionExprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome bloco.
		whileDecl->blockDecl = parseBlock(ctx);

		return whileDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseDoWhile(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Do);
			parseBlock(ctx);
			m_lexer->expectToken(TokenType_e::While);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			return nullptr;
		}

		auto doWhileDecl = std::make_unique<ast::StmtDoWhileDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'do'.
		m_lexer->expectToken(TokenType_e::Do);

		// Consome bloco.
		doWhileDecl->blockDecl = parseBlock(ctx);

		// Consome 'while'.
		m_lexer->expectToken(TokenType_e::While);

		// Consome expressao
		doWhileDecl->conditionExprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return doWhileDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseMatch(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Match);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			m_lexer->expectToken(TokenType_e::LBracket);
			while (true)
			{
				if (m_lexer->isRightBracket())
				{
					break;
				}

			parsePatternlabelSkip:
				m_lexer->expectToken(TokenType_e::When);
				parsePattern(ctx);
				m_lexer->expectToken(TokenType_e::Arrow);
				parseBlock(ctx);
				if (m_lexer->isComma())
				{
					m_lexer->expectToken(TokenType_e::Comma);
					if (m_lexer->isRightBracket()) {
						break;
					}
					goto parsePatternlabelSkip;
				}
				if (m_lexer->isRightBracket())
				{
					break;
				}
				throw exceptions::unexpected_with_possibilities_token_exception(
					m_lexer->getToken().value,
					{
						TokenType_e::When,
						TokenType_e::Comma,
						TokenType_e::RBracket
					},
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}
			m_lexer->expectToken(TokenType_e::RBracket);
			return nullptr;
		}

		auto matchDecl = std::make_unique<ast::StmtMatchDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'match'.
		m_lexer->expectToken(TokenType_e::Match);

		// Consome expressao.
		matchDecl->conditionExprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome '{'.
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (m_lexer->isRightBracket())
			{
				break;
			}

		parsePatternlabel:
			auto whenDecl = std::make_unique<ast::MatchWhenStmtDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Consome 'when'.
			m_lexer->expectToken(TokenType_e::When);

			// Consome pattern.
			whenDecl->patternDecl = parsePattern(ctx);

			// Consome '->'.
			m_lexer->expectToken(TokenType_e::Arrow);

			// Consome bloco.
			whenDecl->blockDecl = parseBlock(ctx);

			// Adiciona a declaracao when a lista.
			matchDecl->whenDeclList.push_back(std::move(whenDecl));

			if (m_lexer->isComma())
			{
				// Consome ','
				m_lexer->expectToken(TokenType_e::Comma);

				// Se for '}' sai do loop.
				if (m_lexer->isRightBracket()) {
					break;
				}
				goto parsePatternlabel;
			}

			if (m_lexer->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{
					TokenType_e::Comma,
					TokenType_e::RBracket
				},
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome '}'.
		m_lexer->expectToken(TokenType_e::RBracket);

		return matchDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseReturn(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Return);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			m_lexer->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto returnDecl = std::make_unique<ast::StmtReturnDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'return'.
		m_lexer->expectToken(TokenType_e::Return);

		// Consome bloco.
		returnDecl->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return returnDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseContinue(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Continue);
			m_lexer->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto continueDecl = std::make_unique<ast::StmtContinueDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'continue'.
		m_lexer->expectToken(TokenType_e::Continue);

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return continueDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseBreak(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Break);
			m_lexer->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto continueDecl = std::make_unique<ast::StmtBreakDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'break'.
		m_lexer->expectToken(TokenType_e::Break);

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return continueDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseGoto(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Goto);
			m_lexer->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto gotoDecl = std::make_unique<ast::StmtGotoDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'goto'.
		m_lexer->expectToken(TokenType_e::Goto);

		// Consome identificador.
		gotoDecl->labelIdentifier = m_lexer->expectIdentifier();

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return gotoDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseTry(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Do);
			parseBlock(ctx);
			m_lexer->expectToken(TokenType_e::Catch);
			parseType(ctx);
			m_lexer->expectIdentifier();
			parseBlock(ctx);
			while (true)
			{
				if (m_lexer->isCatch())
				{
					m_lexer->expectToken(TokenType_e::Catch);
					parseType(ctx);
					m_lexer->expectIdentifier();
					parseBlock(ctx);
					continue;
				}
				break;
			}
			return nullptr;
		}

		auto tryDecl = std::make_unique<ast::StmtTryDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'try'.
		m_lexer->expectToken(TokenType_e::Try);

		// Consome bloco.
		tryDecl->blockDecl = parseBlock(ctx);
		{
			auto catchDecl = std::make_unique<ast::stmt::StmtCatchBlockDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Consome 'catch'
			m_lexer->expectToken(TokenType_e::Catch);

			// Consome declaracao catch.
			catchDecl->typeDecl = parseType(ctx);

			// Consome identificador.
			catchDecl->identifier = m_lexer->expectIdentifier();

			// Consome bloco.
			catchDecl->blockDecl = parseBlock(ctx);

			// Adiciona catch a lista.
			tryDecl->catchDeclList.push_back(std::move(catchDecl));
		}

		while (true)
		{
			if (m_lexer->isCatch())
			{
				auto catchDecl = std::make_unique<ast::StmtCatchBlockDecl>(
					m_lexer->getToken().line,
					m_lexer->getToken().column
					);

				// Consome 'catch'
				m_lexer->expectToken(TokenType_e::Catch);

				// Consome declaracao catch.
				catchDecl->typeDecl = parseType(ctx);

				// Consome identificador.
				catchDecl->identifier = m_lexer->expectIdentifier();

				// Consome bloco.
				catchDecl->blockDecl = parseBlock(ctx);

				// Adiciona catch a lista.
				tryDecl->catchDeclList.push_back(std::move(catchDecl));
				continue;
			}
			break;
		}
		return tryDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parsePanic(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Panic);
			m_lexer->expectToken(TokenType_e::LParBracket);
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			m_lexer->expectToken(TokenType_e::RParBracket);
			m_lexer->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto panicDecl = std::make_unique<ast::StmtPanicDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'panic'.
		m_lexer->expectToken(TokenType_e::Panic);

		// Consome '('.
		m_lexer->expectToken(TokenType_e::LParBracket);

		// Consome identificador.
		panicDecl->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome ')'.
		m_lexer->expectToken(TokenType_e::RParBracket);

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return panicDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseVariable(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			Bool isConst = false;
			switch (m_lexer->getToken().type)
			{
			case TokenType_e::Let:
				isConst = false;
				m_lexer->expectToken(TokenType_e::Let);
				break;
			case TokenType_e::Const:
				isConst = true;
				m_lexer->expectToken(TokenType_e::Const);
				break;
			default:
				throw exceptions::unexpected_with_possibilities_token_exception(
					m_lexer->getToken().value,
					{
						TokenType_e::Let,
						TokenType_e::Const
					},
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}
			if (m_lexer->isRef())
			{
				m_lexer->expectToken(TokenType_e::Ref);
			}
			else if (m_lexer->isShared())
			{
				m_lexer->expectToken(TokenType_e::Shared);
			}

			if (m_lexer->isIdentifier())
			{
				m_lexer->expectIdentifier();
			}
			else if (m_lexer->isLeftBracket() || m_lexer->isLeftParBracket())
			{
				parsePattern(ctx);
			}

			Bool mustHaveInitExpression = isConst ? true : false;

			// Verifica se o tipo foi declarado.
			if (m_lexer->isColon())
			{
				m_lexer->expectToken(TokenType_e::Colon);
				if (m_lexer->isVoid())
				{
					throw exceptions::custom_exception(
						"Variables or constant can't have void type",
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}
				parseType(ctx);
			} else {
				mustHaveInitExpression = true;
			}
			if (mustHaveInitExpression)
			{
				m_lexer->expectToken(TokenType_e::Assign);
				parseExpression(ctx, OperatorPrecLevel_e::Interrogation);
			}

			// Consome ';'.
			m_lexer->expectToken(TokenType_e::SemiColon);

			return nullptr;
		}

		auto variableDecl = std::make_unique<ast::StmtVariableDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome o tipo de variavel.
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::Let:
			m_lexer->expectToken(TokenType_e::Let);
			variableDecl->isConst = false;
			break;
		case TokenType_e::Const:
			m_lexer->expectToken(TokenType_e::Const);
			variableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{
					TokenType_e::Let,
					TokenType_e::Const
				},
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome 'ref'.
		if (m_lexer->isRef())
		{
			m_lexer->expectToken(TokenType_e::Ref);
			variableDecl->isReference = true;
		}
		else if (m_lexer->isShared())
		{
			m_lexer->expectToken(TokenType_e::Shared);
			variableDecl->isShared = true;
		}

		// Consome identificador.
		if (m_lexer->isIdentifier())
		{
			variableDecl->identifier = m_lexer->expectIdentifier();
		}
		else if (m_lexer->isLeftBracket() || m_lexer->isLeftParBracket())
		{
			variableDecl->destructuringPatternDecl = parsePattern(ctx);
		}

		Bool mustHaveInitExpression = variableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (m_lexer->isColon())
		{
			// Consome ':'.
			m_lexer->expectToken(TokenType_e::Colon);

			const U32 line = m_lexer->getToken().line;
			const U32 column = m_lexer->getToken().column;

			// Consome o tipo.
			variableDecl->typeDecl = parseType(ctx);

			// Verifica se o tipo e valido.
			if (variableDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Variables or constant can't have void type",
					line,
					column
				);
			}
		}
		else {
			// Se o tipo nao e declarado explicitamente a variavel deve ser iniciado.
			mustHaveInitExpression = true;
		}

		// Consome a expressao de inicializacao.
		if (mustHaveInitExpression)
		{
			// Consome '='.
			m_lexer->expectToken(TokenType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			variableDecl->initExpression = parseExpression(ctx, OperatorPrecLevel_e::Interrogation);;
		}

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return variableDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseLabel(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectIdentifier();
			m_lexer->expectToken(TokenType_e::Colon);
			return nullptr;
		}

		auto labelDecl = std::make_unique<ast::StmtLabelDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome identificador.
		labelDecl->identifier = m_lexer->expectIdentifier();

		// Consome ':'.
		m_lexer->expectToken(TokenType_e::Colon);

		return labelDecl;
	}

	std::unique_ptr<ast::stmt::StmtDecl>
	Parser::parseExprStmt(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			parseExpression(ctx, OperatorPrecLevel_e::MinPrec);
			m_lexer->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto exprDecl = std::make_unique<ast::StmtExprDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome identificador.
		exprDecl->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec);

		// Consome ';'.
		m_lexer->expectToken(TokenType_e::SemiColon);

		return exprDecl;
	}

	std::unique_ptr<ast::expr::ExpressionDecl>
	Parser::parseExpressionImp(ParserContext_s& ctx, OperatorPrecLevel_e prec)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		std::unique_ptr<ast::expr::ExpressionDecl> lhs;
		std::unique_ptr<ast::expr::ExpressionDecl> rhs;

		const U32 line		= m_lexer->getToken().line;
		const U32 column	= m_lexer->getToken().column;

		// Processa operadores unarios Prefixo.
		if (prec > OperatorPrecLevel_e::Unary) {
			switch (m_lexer->getToken().type)
			{
			case TokenType_e::BitWiseNot:
			case TokenType_e::LogicalNot:
			case TokenType_e::Minus:
			case TokenType_e::Plus:
			case TokenType_e::Increment:
			case TokenType_e::Decrement:
			case TokenType_e::Shared:
			case TokenType_e::Ref:
				{
					// Processa superficialmente
					auto unaryExprDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionUnaryDecl>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					) : nullptr;

					if (!skipOnly) {
						unaryExprDecl->op = m_lexer->getToken().type;
						unaryExprDecl->unaryType = ExpressionUnaryType_e::Prefix;
					}

					// Consome operator.
					m_lexer->nextToken();

					// Processa expressao a direita.
					if (!skipOnly) {
						unaryExprDecl->exprDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::Unary);
					} else {
						parseExpressionImp(ctx, OperatorPrecLevel_e::Unary);
					}
					return unaryExprDecl;
				}
				break;
			case TokenType_e::Match:
				{
					auto matchExprDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionMatchDecl>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					) : nullptr;

					// Consome operator.
					m_lexer->nextToken();

					// Processa expressao a direita.
					if (!skipOnly) {
						matchExprDecl->exprDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::Unary);
					} else {
						parseExpressionImp(ctx, OperatorPrecLevel_e::Unary);
					}

					// Consome '{'
					m_lexer->expectToken(TokenType_e::LBracket);

					// Processa declaracao 'when'
					while (true)
					{
						auto whenDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionMatchWhenDecl>(
							m_lexer->getToken().line,
							m_lexer->getToken().column
						) : nullptr;

						// Consome 'when'
						m_lexer->expectToken(TokenType_e::When);

						// Consome pattern.
						if (!skipOnly) {
							whenDecl->patternDecl = parsePattern(ctx);
						} else {
							parsePattern(ctx);
						}

						// Consome '->'
						m_lexer->expectToken(TokenType_e::Arrow);

						// Consome o expressao.
						if (!skipOnly) {
							whenDecl->exprDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::Interrogation);
						} else {
							parseExpressionImp(ctx, OperatorPrecLevel_e::Interrogation);
						}

						if (m_lexer->isComma())
						{
							// Consome ','
							m_lexer->expectToken(TokenType_e::Comma);
						}

						if (m_lexer->isRightBracket())
						{
							break;
						}
					}

					// Consome '}'
					m_lexer->expectToken(TokenType_e::RBracket);

					return matchExprDecl;
				}
				break;
			default:
				break;
			}
		}

		// Processa atomos: operadores de precedencia maxima
		// como constantes e identificadores.
		if (prec > OperatorPrecLevel_e::Max) {
			return parseAtom(ctx);
		}

		// Processa expressao a esquerda.
		if (!skipOnly) {
			lhs = parseExpressionImp(ctx, static_cast<OperatorPrecLevel_e>(prec + 1));
		} else {
			parseExpressionImp(ctx, static_cast<OperatorPrecLevel_e>(prec + 1));
		}

		while (true)
		{
			const TokenType_e op = m_lexer->getToken().type;

			// Se nao e um operador de expressao sai do loop.
			if (!isExprOperator(m_lexer->getToken().type))
			{
				break;
			}

			// Processa operator unario posfixo.
			switch (m_lexer->getToken().type)
			{
			case TokenType_e::Increment:
			case TokenType_e::Decrement:
				{
					// Consome o token: '++' ou '--'
					m_lexer->nextToken();

					auto unaryExprDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionUnaryDecl>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					) : nullptr;

					if (!skipOnly) {
						unaryExprDecl->op = m_lexer->getToken().type;
						unaryExprDecl->unaryType = ExpressionUnaryType_e::Posfix;
						unaryExprDecl->exprDecl = std::move(lhs);

						lhs = std::move(unaryExprDecl);
					}
				}
				continue;
			default:
				break;
			}

			// Pega informacoes do operador.
			const auto& opInfo = getOperatorInfo(m_lexer->getToken().type);

			// Retorna se precedencia dele e menor que a atual da funcao.
			if (opInfo.prec < prec) {
				break;
			}

			// Calcula proxima precedencia minima.
			U32 nextMinPrec = opInfo.left ? opInfo.prec + 1 : opInfo.prec;
			
			// Consome o operador.
			m_lexer->nextToken();

			// Processa chamada de funcao ou indexacao de dados.
			switch (op)
			{
			case TokenType_e::LParBracket:
				{
					// Se nao ha parametros e uma expressao unaria posfixa f().
					if (m_lexer->isRightParBracket())
					{
						// Consome ')'
						m_lexer->expectToken(TokenType_e::RParBracket);

						auto functionCallExpr = !skipOnly ? std::make_unique<ast::expr::ExpressionFunctionCall>(
							line,
							column
						) : nullptr;

						if (!skipOnly) {
							functionCallExpr->leftDecl = std::move(lhs);
							lhs = std::move(functionCallExpr);
						}
						continue;
					}

					auto functionCallExpr = !skipOnly ? std::make_unique<ast::expr::ExpressionFunctionCall>(
						line,
						column
					) : nullptr;

					if (!skipOnly) {
						functionCallExpr->leftDecl = std::move(lhs);
						functionCallExpr->rightDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);
					} else {
						parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);
					}

					// Consome ')'
					m_lexer->expectToken(TokenType_e::RParBracket);

					if (!skipOnly) {
						lhs = std::move(functionCallExpr);
					}
					continue;
				}
				break;
			case TokenType_e::LSquBracket:
				{
					auto indexAddressExpr = !skipOnly ? std::make_unique<ast::expr::ExpressionIndexDecl>(
						line,
						column
					) : nullptr;

					if (!skipOnly) {
						indexAddressExpr->leftDecl = std::move(lhs);
						indexAddressExpr->rightDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);
					} else {
						parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);
					}

					// Consome ']'
					m_lexer->expectToken(TokenType_e::RSquBracket);

					if (!skipOnly) {
						lhs = std::move(indexAddressExpr);
					}
					continue;
				}
				break;
			case TokenType_e::LessThan:
				{
					auto exprGenericDef = !skipOnly ? std::make_unique<ast::expr::ExpressionGenericDecl>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					) : nullptr;

					Bool		parseGeneric = false;
					Bool		finishParse = false;
					Bool		expectType = false;
					const U32	position = m_lexer->getToken().position;

					while (true)
					{
						switch (m_lexer->getToken().type)
						{
						case TokenType_e::Void:
							{
								throw exceptions::custom_exception(
									"Generic item can't be 'void'",
									m_lexer->getToken().line,
									m_lexer->getToken().column
								);
							}
							break;
						case TokenType_e::Bool:
						case TokenType_e::I8:
						case TokenType_e::U8:
						case TokenType_e::I16:
						case TokenType_e::U16:
						case TokenType_e::I32:
						case TokenType_e::U32:
						case TokenType_e::I64:
						case TokenType_e::U64:
						case TokenType_e::Fp32:
						case TokenType_e::Fp64:
						case TokenType_e::String:
						case TokenType_e::Object:
						case TokenType_e::Identifier:
						case TokenType_e::ScopeResolution:
							{
								// Consome o tipo.
								expectType = false;
								auto typeDecl = parseType(ctx);

								// TODO: Verificar se e um tipo ou uma instancia de dados.

								// Adiciona o template a lista.
								exprGenericDef->genericTypeList.push_back(std::move(typeDecl));
								continue;
							}
							break;
						case TokenType_e::Comma:
							{
								expectType = true;
								m_lexer->nextToken();	// Consome ','
								continue;
							}
							break;
						case TokenType_e::GreaterThan:
							if (m_lexer->predictNextToken().type == TokenType_e::LParBracket) {
								m_lexer->nextToken();	// Consome '>'
								m_lexer->nextToken();	// Consome '('

								parseGeneric = true;
								finishParse = true;

								// Verfica se e esperado uma declaracao de tipo.
								if (expectType)
								{
									throw exceptions::custom_exception(
										"Expect type decl",
										m_lexer->getToken().line,
										m_lexer->getToken().column
									);
								}

								exprGenericDef->lhs = std::move(lhs);
								exprGenericDef->rhs = parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);

								// Consome ')'
								m_lexer->expectToken(TokenType_e::RParBracket);

								lhs = std::move(exprGenericDef);
							} else {
								finishParse = true;
								m_lexer->resetToPosition(position);
							}
							break;
						default:
							finishParse = true;
							m_lexer->resetToPosition(position);
							break;
						}

						// Quebra o loop.
						if (finishParse)
						{
							break;
						}
					}

					// Verifica se houve o processamento do generic.
					if (parseGeneric)
					{
						continue;
					}
				}
				break;
			default:
				break;
			}

			// Processa expressao a direita.
			if (!skipOnly) {
				rhs = parseExpressionImp(ctx, static_cast<OperatorPrecLevel_e>(nextMinPrec));
			} else {
				parseExpressionImp(ctx, static_cast<OperatorPrecLevel_e>(nextMinPrec));
			}

			// Verifica se a operacao e ternaria
			if (op == TokenType_e::Interrogation)
			{
				auto ternaryExprDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionTernaryDecl>(
					line,
					column
				) : nullptr;

				// Consome ':'
				m_lexer->expectToken(TokenType_e::Colon);

				if (!skipOnly) {
					ternaryExprDecl->conditionDecl = std::move(lhs);
					ternaryExprDecl->leftDecl = std::move(rhs);
					ternaryExprDecl->rightDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);

					lhs = std::move(ternaryExprDecl);
				}
				break;
			}

			// Processa operador binario.
			if (!skipOnly) {
				auto binaryExprDecl = std::make_unique<ast::expr::ExpressionBinaryDecl>(
					line,
					column
				);

				binaryExprDecl->op = op;
				binaryExprDecl->leftDecl = std::move(lhs);
				binaryExprDecl->rightDecl = std::move(rhs);

				lhs = std::move(binaryExprDecl);				
			}
		}
		return lhs;
	}

	std::unique_ptr<ast::expr::ExpressionDecl>
	Parser::parseAtom(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		const U32 line = m_lexer->getToken().line;
		const U32 column = m_lexer->getToken().column;

		// Processa expressao entre parenteses.
		if (m_lexer->isLeftParBracket())
		{
			std::unique_ptr<ast::expr::ExpressionDecl> expr;

			// Consome '('
			m_lexer->expectToken(TokenType_e::LParBracket);

			// Processa expressao.
			if (!skipOnly) {
				expr = parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);

				// Ajusta a posicao inicial
				expr->line = line;
				expr->column = column;

				// Consome ')'
				m_lexer->expectToken(TokenType_e::RParBracket);
			} else {
				parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);

				// Consome ')'
				m_lexer->expectToken(TokenType_e::RParBracket);
			}
			return expr;
		}

		// Processa declaracao de array.
		if (m_lexer->isLeftSquBracket())
		{
			auto arrayIniDecl = !skipOnly ? std::make_unique<ast::ExpressionArrayInitDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			) : nullptr;

			// Consome '['
			m_lexer->expectToken(TokenType_e::LSquBracket);

			while (true)
			{
				if (m_lexer->isRightSquBracket())
				{
					break;
				}

				// Consome elemento
				if (!skipOnly) {
					arrayIniDecl->arrayElementDeclList.push_back(parseExpressionImp(ctx, OperatorPrecLevel_e::Interrogation));
				} else {
					parseExpressionImp(ctx, OperatorPrecLevel_e::Interrogation);
				}

				if (m_lexer->isComma())
				{
					m_lexer->expectToken(TokenType_e::Comma);
				}
			}

			// Consome ']'
			m_lexer->expectToken(TokenType_e::RSquBracket);

			return arrayIniDecl;
		}

		// Processa declaracao de funcao anonima.
		if (m_lexer->isBitWiseOr() || m_lexer->isLogicalOr())
		{
			// Quebra token.
			if (m_lexer->isLogicalOr())
			{
				m_lexer->reinterpretToken(TokenType_e::BitWiseOr, 1);
			}

			auto functionDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionFunctionDecl>(
				line,
				column
			) : nullptr;

			// Consome '|'
			m_lexer->expectToken(TokenType_e::BitWiseOr);

			// Uma funcao anonima pode ter parametros tipados ou nao, depende do contexto.
			// Por exemplo se a funcao esta sendo passada como parametro, seus tipos podem ser
			// deduzidos com base no tipo do parametro, o mesmo pode ocorrer na declaracao de variaveis
			// ou ainda atribui-la a um membro de classe, struct.
			// Porem ha casos onde a tipagem de faz necessaria por exemplo ao usar uma declaracao de variavel
			// ou constate onde o tipo e omitido, logo, nao se poderia fazer a deducao.

			if (m_lexer->isIdentifier()) {
				if (m_lexer->predictNextToken().type == TokenType_e::Colon)
				{
					// Processa o restante do parametros se houver.
					while (true)
					{
						if (m_lexer->isBitWiseOr())
						{
							break;
						}

					parseParamterDecl:
						auto paramDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionFunctionParameterDecl>(
							m_lexer->getToken().line,
							m_lexer->getToken().column
						) : nullptr;

						// Consome identificador.
						if (!skipOnly) {
							paramDecl->identifier = m_lexer->expectIdentifier();
						} else {
							m_lexer->expectIdentifier();
						}

						// Consome ellipsis
						if (m_lexer->isEllipsis()) {
							if (!skipOnly) {
								paramDecl->isEllipsis = true;
							}
							m_lexer->expectToken(TokenType_e::Ellipsis);
							goto parseFunctionRetOrBody;
						}

						// Consome ':'
						m_lexer->expectToken(TokenType_e::Colon);

						// Consome o tipo do parametro.
						if (!skipOnly) {
							paramDecl->typeDecl = parseType(ctx);
						} else {
							parseType(ctx);
						}

						// Parametro nao podem ter tipo nulo.
						if (!skipOnly) {
							if (paramDecl->typeDecl->typeID == TypeDeclID_e::Void)
							{
								throw exceptions::custom_exception(
									"Parameter '%s' can't have void type",
									m_lexer->getToken().line,
									m_lexer->getToken().column,
									paramDecl->identifier.str()
								);
							}

							// Adiciona o parametro a lista
							functionDecl->parametersDecl.push_back(std::move(paramDecl));
						}

						// Consome ','
						if (m_lexer->isComma())
						{
							m_lexer->expectToken(TokenType_e::Comma);
							goto parseParamterDecl;
						}

						if (m_lexer->isBitWiseOr())
						{
							break;
						}

						throw exceptions::unexpected_with_possibilities_token_exception(
							m_lexer->getToken().value,
							{
								TokenType_e::Comma,
								TokenType_e::BitWiseOr
							},
							m_lexer->getToken().line,
							m_lexer->getToken().column
						);
					}
				} else {
					// Atribui nivel de inferencia.
					if (!skipOnly) {
						functionDecl->inferenceType = InferenceType_e::OnlyParams;
					}

					auto paramDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionFunctionParameterDecl>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					) : nullptr;

					if (!skipOnly) {
						paramDecl->identifier = m_lexer->expectIdentifier();
					} else {
						m_lexer->expectIdentifier();
					}

					// Consome ellipsis
					if (m_lexer->isEllipsis()) {
						if (!skipOnly) {
							paramDecl->isEllipsis = true;
						}
						m_lexer->expectToken(TokenType_e::Ellipsis);
						goto parseFunctionRetOrBody;
					}

					// Adiciona o parametro a lista
					if (!skipOnly) {
						functionDecl->parametersDecl.push_back(std::move(paramDecl));
					}

					// Processa o restante do parametros se houver.
					while (true)
					{
						if (m_lexer->isBitWiseOr())
						{
							break;
						}

						// Consome ','
						m_lexer->expectToken(TokenType_e::Comma);

						auto paramDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionFunctionParameterDecl>(
							m_lexer->getToken().line,
							m_lexer->getToken().column
						) : nullptr;

						if (!skipOnly) {
							paramDecl->identifier = m_lexer->expectIdentifier();
						} else {
							m_lexer->expectIdentifier();
						}

						// Consome ellipsis
						if (m_lexer->isEllipsis()) {
							if (!skipOnly) {
								paramDecl->isEllipsis = true;
								functionDecl->parametersDecl.push_back(std::move(paramDecl));
							}
							m_lexer->expectToken(TokenType_e::Ellipsis);
							goto parseFunctionRetOrBody;
						}
					}
				}
			}

		parseFunctionRetOrBody:

			// Consome '|'
			m_lexer->expectToken(TokenType_e::BitWiseOr);

			// Verifica se o tipo de retorno e explicito.
			if (m_lexer->isArrow()) {
				// Consome '->'
				m_lexer->expectToken(TokenType_e::Arrow);

				if (!skipOnly) {
					functionDecl->returnTypeDecl = parseType(ctx);
				} else {
					parseType(ctx);
				}
			} else {
				// Atribui nivel de inferencia.
				if (!skipOnly) {
					if (functionDecl->inferenceType == InferenceType_e::OnlyParams)	{
						functionDecl->inferenceType = InferenceType_e::Full;
					} else {
						functionDecl->inferenceType = InferenceType_e::OnlyReturn;
					}

					functionDecl->returnTypeDecl = std::make_unique<ast::TypeDeclVoid>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}
			}

			// Consome bloco.
			if (!skipOnly) {
				functionDecl->blockDecl = parseBlock(ctx);
			} else {
				parseBlock(ctx);
			}

			return functionDecl;
		}

		// Processa new.
		if (m_lexer->isNew())
		{
			auto newDecl = !skipOnly ? std::make_unique<ast::ExpressionNewDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			) : nullptr;

			// Consome 'new'
			m_lexer->expectToken(TokenType_e::New);

			// Consome o tipo.
			if (!skipOnly) {
				newDecl->objectTypeDecl = parseType(ctx);
			} else {
				parseType(ctx);
			}

			// Consome '('
			m_lexer->expectToken(TokenType_e::LParBracket);

			// Consome expressao no caso os parametros passados para o construtor.
			if (!m_lexer->isRightParBracket())
			{
				if (!skipOnly) {
					newDecl->expressionDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);
				} else {
					parseExpressionImp(ctx, OperatorPrecLevel_e::MinPrec);
				}
			}

			// Consome ')'
			m_lexer->expectToken(TokenType_e::RParBracket);

			// Processa o bloco de inicio.
			if (m_lexer->isLeftBracket())
			{
				// Consome '{'
				m_lexer->expectToken(TokenType_e::LBracket);
				
				if (!skipOnly) {
					newDecl->objectInitBlockDecl = std::make_unique<ast::ExpressionNewBlockDecl>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}

				while (true)
				{
					if (m_lexer->isRightBracket())
					{
						break;
					}

					auto itemDecl = !skipOnly ? std::make_unique<ast::expr::ExpressionNewItemDecl>(
						m_lexer->getToken().line,
						m_lexer->getToken().column
					) : nullptr;

					// Processa identificador.
					if (!skipOnly) {
						itemDecl->identifier = m_lexer->expectIdentifier();
					} else {
						m_lexer->expectIdentifier();
					}

					// Consome ':'
					if (m_lexer->isColon())
					{
						m_lexer->expectToken(TokenType_e::Colon);

						// Consome expressao.
						if (!skipOnly) {
							itemDecl->expressionDecl = parseExpressionImp(ctx, OperatorPrecLevel_e::EnumExpr);
						} else {
							parseExpressionImp(ctx, OperatorPrecLevel_e::EnumExpr);
						}
					}

					// Adiciona item ao bloco.
					if (!skipOnly) {
						newDecl->objectInitBlockDecl->itemDeclList.push_back(std::move(itemDecl));
					}

					// Verifica se existem mais itens.
					if (m_lexer->isComma())
					{
						// Consome ','
						m_lexer->expectToken(TokenType_e::Comma);
					}
				}

				// Consome '}'
				m_lexer->expectToken(TokenType_e::RBracket);
			}
			return newDecl;
		}

		// Processa this.
		if (m_lexer->isThis())
		{
			// Processa superficialmente.
			if (skipOnly) {
				m_lexer->expectToken(TokenType_e::This);
				return nullptr;
			}

			// Consome 'this'
			m_lexer->expectToken(TokenType_e::This);

			return std::make_unique<ast::expr::ExpressionThisDecl>(line,column);
		}

		// Processa super.
		if (m_lexer->isSuper())
		{
			// Processa superficialmente.
			if (skipOnly) {
				m_lexer->expectToken(TokenType_e::Super);
				return nullptr;
			}

			// Consome 'super'
			m_lexer->expectToken(TokenType_e::Super);

			return std::make_unique<ast::expr::ExpressionSuperDecl>(line, column);
		}

		// Processa null.
		if (m_lexer->isNull())
		{
			// Processa superficialmente.
			if (skipOnly) {
				m_lexer->expectToken(TokenType_e::Null);
				return nullptr;
			}

			// Consome 'null'
			m_lexer->expectToken(TokenType_e::Null);

			return std::make_unique<ast::expr::ExpressionConstantNullDecl>(line, column);
		}

		// Processa superficialmente.
		if (skipOnly) {
			// Processa algumas constantes
			switch (m_lexer->getToken().type)
			{
			case TokenType_e::False:
			case TokenType_e::True:
				{
					m_lexer->nextToken();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantInteger:
				{
					m_lexer->expectConstantInteger();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantFp32:
				{
					m_lexer->expectConstantFp32();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantFp64:
				{
					m_lexer->expectConstantFp64();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantString:
				{
					m_lexer->expectConstantString();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantChar:
				{
					m_lexer->expectConstantChar();
					return nullptr;
				}
				break;
			default:
				break;
			}

			// Resolucao de escopo unaria, indica acesso ao escopo global.
			if (m_lexer->isScopeResolution())
			{
				m_lexer->expectToken(TokenType_e::ScopeResolution);
				m_lexer->expectIdentifier();

				U32 position = m_lexer->getToken().position;
				if (m_lexer->isLessThan())
				{
					m_lexer->nextToken();

					while (true)
					{
						switch (m_lexer->getToken().type)
						{
						case TokenType_e::Bool:
						case TokenType_e::I8:
						case TokenType_e::U8:
						case TokenType_e::I16:
						case TokenType_e::U16:
						case TokenType_e::I32:
						case TokenType_e::U32:
						case TokenType_e::I64:
						case TokenType_e::U64:
						case TokenType_e::Fp32:
						case TokenType_e::Fp64:
						case TokenType_e::String:
						case TokenType_e::Object:
						case TokenType_e::Comma:
						case TokenType_e::Identifier:
							m_lexer->nextToken();
							break;
						case TokenType_e::ScopeResolution:
							if (m_lexer->predictNextToken().type == TokenType_e::Identifier) {
								m_lexer->nextToken();
								m_lexer->nextToken();
							}
							break;
						case TokenType_e::GreaterThan:
							if (m_lexer->predictNextToken().type == TokenType_e::LParBracket) {
								m_lexer->nextToken();
								m_lexer->nextToken();
								return nullptr;
							}
							m_lexer->resetToPosition(position);
							return nullptr;
						default:
							m_lexer->resetToPosition(position);
							return nullptr;
						}
					}
				}
				return nullptr;
			}

			// Processa expressao named
			if (m_lexer->isIdentifier())
			{
				m_lexer->expectIdentifier();

				U32 position = m_lexer->getToken().position;
				if (m_lexer->isLessThan())
				{
					m_lexer->nextToken();

					while (true)
					{
						switch (m_lexer->getToken().type)
						{
						case TokenType_e::Bool:
						case TokenType_e::I8:
						case TokenType_e::U8:
						case TokenType_e::I16:
						case TokenType_e::U16:
						case TokenType_e::I32:
						case TokenType_e::U32:
						case TokenType_e::I64:
						case TokenType_e::U64:
						case TokenType_e::Fp32:
						case TokenType_e::Fp64:
						case TokenType_e::String:
						case TokenType_e::Object:
						case TokenType_e::Comma:
						case TokenType_e::Identifier:
							m_lexer->nextToken();
							break;
						case TokenType_e::ScopeResolution:
							if (m_lexer->predictNextToken().type == TokenType_e::Identifier) {
								m_lexer->nextToken();
								m_lexer->nextToken();
							}
							break;
						case TokenType_e::GreaterThan:
							if (m_lexer->predictNextToken().type == TokenType_e::LParBracket) {
								m_lexer->nextToken();
								m_lexer->nextToken();
								return nullptr;
							}
							m_lexer->resetToPosition(position);
							return nullptr;
						default:
							m_lexer->resetToPosition(position);
							return nullptr;
						}
					}
				}
				return nullptr;
			}
		
			throw exceptions::unexpected_token_exception(
				m_lexer->getToken().value,
				line,
				column
			);
		}

		// Processa algumas constantes
		switch (m_lexer->getToken().type)
		{
		case TokenType_e::False:
		case TokenType_e::True:
			{
				auto constantBoolDecl = std::make_unique<ast::expr::ExpressionConstantBoolDecl>(line, column);
				constantBoolDecl->valueDecl = m_lexer->getToken().type == TokenType_e::True ? true : false;
				m_lexer->nextToken();
				return constantBoolDecl;
			}
			break;
		case TokenType_e::ConstantInteger:
			{
				auto constantIntegerDecl = std::make_unique<ast::expr::ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::I32;
				constantIntegerDecl->valueDecl = m_lexer->expectConstantInteger();
				return constantIntegerDecl;
			}
			break;
		case TokenType_e::ConstantFp32:
			{
				auto constantRealDecl = std::make_unique<ast::expr::ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = TypeDeclID_e::Fp32;
				constantRealDecl->valueDecl = m_lexer->expectConstantFp32();
				return constantRealDecl;
			}
			break;
		case TokenType_e::ConstantFp64:
			{
				auto constantRealDecl = std::make_unique<ast::expr::ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = TypeDeclID_e::Fp64;
				constantRealDecl->valueDecl = m_lexer->expectConstantFp64();
				return constantRealDecl;
			}
			break;
		case TokenType_e::ConstantString:
			{
				auto constantStringDecl = std::make_unique<ast::expr::ExpressionConstantStringDecl>(line, column);
				constantStringDecl->valueDecl = m_lexer->expectConstantString();
				return constantStringDecl;
			}
			break;
		case TokenType_e::ConstantChar:
			{
				auto constantCharDecl = std::make_unique<ast::expr::ExpressionConstantCharDecl>(line, column);
				constantCharDecl->valueDecl = m_lexer->expectConstantChar();
				return constantCharDecl;
			}
			break;
		case TokenType_e::Void:
		case TokenType_e::Bool:
		case TokenType_e::I8:
		case TokenType_e::U8:
		case TokenType_e::I16:
		case TokenType_e::U16:
		case TokenType_e::I32:
		case TokenType_e::U32:
		case TokenType_e::I64:
		case TokenType_e::U64:
		case TokenType_e::Fp32:
		case TokenType_e::Fp64:
		case TokenType_e::String:
		case TokenType_e::Object:
			{
				auto typeExprDecl = std::make_unique<ast::expr::ExpressionPrimitiveTypeDecl>(line, column);

				// Processa superficialmente.
				if (skipOnly) {
					parseType(ctx);
					return nullptr;
				}

				// Consome 'super'
				typeExprDecl->typeDecl = parseType(ctx);

				return typeExprDecl;
			}
			break;
		default:
			break;
		}

		// Resolucao de escopo unaria, indica acesso ao escopo global.
		if (m_lexer->isScopeResolution())
		{
			// Consome '::'
			m_lexer->expectToken(TokenType_e::ScopeResolution);

			auto namedExpressionDecl = std::make_unique<ast::expr::ExpressionConstantIdentifierDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
			namedExpressionDecl->identifierDecl = m_lexer->expectIdentifier();
			namedExpressionDecl->startFromRoot = true;
			return namedExpressionDecl;
		}

		// Processa expressao named
		if (m_lexer->isIdentifier())
		{
			auto namedExpressionDecl = std::make_unique<ast::expr::ExpressionConstantIdentifierDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
			namedExpressionDecl->identifierDecl = m_lexer->expectIdentifier();
			return namedExpressionDecl;
		}

		throw exceptions::unexpected_token_exception(
			m_lexer->getToken().value,
			line,
			column
		);
		return nullptr;
	}

	std::unique_ptr<ast::pattern::PatternDecl>
	Parser::parseLiteralPattern(ParserContext_s& ctx)
	{
		auto literalPatternDecl = std::make_unique<ast::pattern::LiteralPatternDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome uma expressao.
		literalPatternDecl->patternExpr = parseExpression(ctx, OperatorPrecLevel_e::Max);

		return literalPatternDecl;
	}

	std::unique_ptr<ast::pattern::PatternDecl>
	Parser::parseEnumPattern(ParserContext_s& ctx)
	{
		auto enumPatternDecl = std::make_unique<ast::pattern::DestructuringPatternDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Atribui tipo de destructuring.
		enumPatternDecl->destructuringType = DestructuringType_e::Enum;

		// Consome expressao.
		enumPatternDecl->enumExpr = parseExpression(ctx, OperatorPrecLevel_e::Max);

		// Consome '('
		m_lexer->expectToken(TokenType_e::LParBracket);

		while (true)
		{
			auto destructuringItem = std::make_unique<ast::pattern::DestructuringItemDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Atribui o tipo de destructuring item.
			destructuringItem->destructuringItemType = DestructuringItemType_e::DirectReference;

			// Consome o identificador.
			destructuringItem->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::Max);

			// Adiciona item a lista.
			enumPatternDecl->destructuringItemDeclList.push_back(std::move(destructuringItem));

			if (m_lexer->isComma())
			{
				// Consome ','
				m_lexer->expectToken(TokenType_e::Comma);
				continue;
			}

			if (m_lexer->isRightParBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{
					TokenType_e::RParBracket,
					TokenType_e::Identifier
				},
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome ')'
		m_lexer->expectToken(TokenType_e::RParBracket);

		return enumPatternDecl;
	}

	std::unique_ptr<ast::pattern::PatternDecl>
	Parser::parseTuplePattern(ParserContext_s& ctx)
	{
		auto tuplePatternDecl = std::make_unique<ast::pattern::DestructuringPatternDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Atribui tipo de destructuring.
		tuplePatternDecl->destructuringType = DestructuringType_e::Tuple;

		// Consome '('
		m_lexer->expectToken(TokenType_e::LParBracket);

		while (true)
		{
			auto destructuringItem = std::make_unique<ast::pattern::DestructuringItemDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Atribui o tipo de destructuring item.
			destructuringItem->destructuringItemType = DestructuringItemType_e::DirectReference;

			// Consome o identificador.
			destructuringItem->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::Max);

			// Adiciona item a lista.
			tuplePatternDecl->destructuringItemDeclList.push_back(std::move(destructuringItem));

			if (m_lexer->isComma())
			{
				// Consome ','
				m_lexer->expectToken(TokenType_e::Comma);
				continue;
			}

			if (m_lexer->isRightParBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{
					TokenType_e::RParBracket,
					TokenType_e::Identifier
				},
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome ')'
		m_lexer->expectToken(TokenType_e::RParBracket);

		return tuplePatternDecl;
	}

	std::unique_ptr<ast::pattern::PatternDecl>
	Parser::parseClassOrStructPattern(ParserContext_s& ctx)
	{
		auto destructuringDecl = std::make_unique<ast::pattern::DestructuringPatternDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Atribui o tipo de destructuring pattern.
		destructuringDecl->destructuringType = DestructuringType_e::ClassOrStruct;

		// Consome '{'
		m_lexer->expectToken(TokenType_e::LBracket);

		while (true) {
			if (m_lexer->isRightBracket())
			{
				break;
			}

			auto destructuringItem = std::make_unique<ast::pattern::DestructuringItemDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);

			// Consome o identificador.
			destructuringItem->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::Max);

			// Verifica se e uma referenciacao.
			if (m_lexer->isColon())
			{
				// Consome ':'
				m_lexer->expectToken(TokenType_e::Colon);

				// Verifica se faz uma referencia indireta ou a
				// um sub destructuring.
				if (m_lexer->isLeftBracket() || m_lexer->isLeftParBracket())
				{
					destructuringItem->destructuringItemType = DestructuringItemType_e::SubDestructuring;
					destructuringItem->subDestructuringPattern = parsePattern(ctx);
				}
				else
				{
					destructuringItem->destructuringItemType = DestructuringItemType_e::IndirectReferenceOrMatch;
					destructuringItem->indirectRefOrMatchingDecl = parseExpression(ctx, OperatorPrecLevel_e::Max);
				}
			}
			else
			{
				destructuringItem->destructuringItemType = DestructuringItemType_e::DirectReference;
			}

			// Adiciona item a lista.
			destructuringDecl->destructuringItemDeclList.push_back(std::move(destructuringItem));

			// Consome ','.
			if (m_lexer->isComma()) {
				m_lexer->expectToken(TokenType_e::Comma);
				continue;
			}

			if (m_lexer->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				m_lexer->getToken().value,
				{
					TokenType_e::Comma,
					TokenType_e::RBracket
				},
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome '}'
		m_lexer->expectToken(TokenType_e::RBracket);

		return destructuringDecl;
	}

	std::unique_ptr<ast::TypeDecl>
	Parser::parseFunctionType(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::Fn);
			m_lexer->expectToken(TokenType_e::LParBracket);

			while (true)
			{
				if (m_lexer->isRightParBracket())
				{
					break;
				}
			parseParameterTypeLabelSkip:

				// Parametros nao podem ser nulos.
				if (m_lexer->isVoid())
				{
					throw exceptions::unexpected_type_exception(
						"void",
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}

				parseType(ctx);

				if (m_lexer->isComma()) {
					m_lexer->expectToken(TokenType_e::Comma);
					goto parseParameterTypeLabelSkip;
				}

				if (m_lexer->isArrow())
				{
					m_lexer->expectToken(TokenType_e::Arrow);
					parseType(ctx);
					break;
				}

				throw exceptions::custom_exception("Expected ')', '->' or ',' declaration",
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}
			m_lexer->expectToken(TokenType_e::RParBracket);
			return nullptr;
		}

		auto functionTypeDecl = std::make_unique<ast::TypeDeclFunction>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome 'fn'
		m_lexer->expectToken(TokenType_e::Fn);

		// Consome '('
		m_lexer->expectToken(TokenType_e::LParBracket);

		Bool hasReturnExplicit = false;
		while (true)
		{
			if (m_lexer->isRightParBracket())
			{
				break;
			}

		parseParameterTypeLabel:

			// Parametros nao podem ser nulos.
			if (m_lexer->isVoid())
			{
				throw exceptions::unexpected_type_exception(
					"void",
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}

			// Consome o tipo do parametro.
			if (auto paramType = parseType(ctx))
			{
				functionTypeDecl->parameterTypeList.push_back(std::move(paramType));

				// Verifica se ha mais parametros e consome ','
				if (m_lexer->isComma()) {
					m_lexer->expectToken(TokenType_e::Comma);
					goto parseParameterTypeLabel;
				}
			}

			// Processa o retorno
			if (m_lexer->isArrow())
			{
				// Consome '->'
				m_lexer->expectToken(TokenType_e::Arrow);

				functionTypeDecl->returnType = parseType(ctx);

				hasReturnExplicit = true;
				break;
			}

			throw exceptions::custom_exception("Expected ')', '->' or ',' declaration",
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Se nao ha retorno explicito void e o tipo padrao.
		if (!hasReturnExplicit)
		{
			functionTypeDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome ')'
		m_lexer->expectToken(TokenType_e::RParBracket);

		return functionTypeDecl;
	}

	std::unique_ptr<ast::TypeDecl>
	Parser::parseTupleType(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::LParBracket);
			
			while (true)
			{
				if (m_lexer->isVoid())
				{
					throw exceptions::unexpected_type_exception("void",
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}

				parseType(ctx);

				if (m_lexer->isComma())
				{
					m_lexer->expectToken(TokenType_e::Comma);
					if (m_lexer->isVoid())
					{
						throw exceptions::unexpected_type_exception("void",
							m_lexer->getToken().line,
							m_lexer->getToken().column
						);
					}
					parseType(ctx);
					continue;
				}

				if (m_lexer->isRightBracket())
				{
					break;
				}

				throw exceptions::unexpected_token_exception(
					m_lexer->getToken().value,
					m_lexer->getToken().line,
					m_lexer->getToken().column
				);
			}

			// Consome ')'.
			m_lexer->expectToken(TokenType_e::RParBracket);
			return nullptr;
		}

		auto tupleTypeDecl = std::make_unique<ast::TypeDeclTuple>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome '('.
		m_lexer->expectToken(TokenType_e::LParBracket);

		while (true)
		{
			// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
			if (auto tupleItemDecl = parseType(ctx))
			{
				if (tupleItemDecl->typeID == TypeDeclID_e::Void)
				{
					throw exceptions::unexpected_type_exception("void",
						m_lexer->getToken().line,
						m_lexer->getToken().column
					);
				}
				tupleTypeDecl->tupleItemList.push_back(std::move(tupleItemDecl));
			}

			if (m_lexer->isComma())
			{
				// Consome ','.
				m_lexer->expectToken(TokenType_e::Comma);

				// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
				if (auto tupleItemDecl = parseType(ctx))
				{
					if (tupleItemDecl->typeID == TypeDeclID_e::Void)
					{
						throw exceptions::unexpected_type_exception("void",
							m_lexer->getToken().line,
							m_lexer->getToken().column
						);
					}
					tupleTypeDecl->tupleItemList.push_back(std::move(tupleItemDecl));
				}
				continue;
			}

			if (m_lexer->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_token_exception(
				m_lexer->getToken().value,
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		// Consome ')'.
		m_lexer->expectToken(TokenType_e::RParBracket);

		return tupleTypeDecl;
	}

	std::unique_ptr<ast::TypeDecl>
	Parser::parseNamedType(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			if (m_lexer->isScopeResolution())
			{
				m_lexer->expectToken(TokenType_e::ScopeResolution);
			}
			m_lexer->expectIdentifier();
			if (m_lexer->isLessThan())
			{
				m_lexer->expectToken(TokenType_e::LessThan);
				while (true)
				{
					if (m_lexer->isBitWiseRightShift() || m_lexer->isBitWiseRightShiftAssign())
					{
						switch (m_lexer->getToken().type)
						{
						case TokenType_e::BitWiseRShift:		// >>
						case TokenType_e::GreaterThanOrEqual:	// >=
						case TokenType_e::BitWiseRShiftAssign:	// >>=
							m_lexer->reinterpretToken(TokenType_e::GreaterThan, 1);
							break;
						default:
							break;
						}
					}

					if (m_lexer->isGreaterThan())
					{
						break;
					}

					parseType(ctx);

					if (m_lexer->isComma())
					{
						m_lexer->expectToken(TokenType_e::Comma);
						continue;
					}
				}
				m_lexer->expectToken(TokenType_e::GreaterThan);
			}

			// Verifica se ha identificadores internos.
			if (m_lexer->isScopeResolution())
			{
				parseInternalNamedType(ctx);
			}
			return nullptr;
		}

		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Verifica se inicia pelo escopo global.
		if (m_lexer->isScopeResolution())
		{
			// Consome '::'.
			m_lexer->expectToken(TokenType_e::ScopeResolution);
			namedTypeDecl->startFromRoot = true;
		}

		// Consome o identificador.
		namedTypeDecl->identifier = m_lexer->expectIdentifier();

		// Verefica se a definicao de generic.
		if (m_lexer->isLessThan())
		{
			// Consome '<'.
			m_lexer->expectToken(TokenType_e::LessThan);

			while (true)
			{
				// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
				// serao quebrados em tokens menores.
				if (m_lexer->isBitWiseRightShift() || m_lexer->isBitWiseRightShiftAssign())
				{
					switch (m_lexer->getToken().type)
					{
					case TokenType_e::BitWiseRShift:		// >>
					case TokenType_e::GreaterThanOrEqual:	// >=
					case TokenType_e::BitWiseRShiftAssign:	// >>=
						m_lexer->reinterpretToken(TokenType_e::GreaterThan, 1);
						break;
					default:
						break;
					}
				}

				// Verifica se terminou a definicao dos generics
				if (m_lexer->isGreaterThan())
				{
					break;
				}

				// Processa a definicao de generic
				namedTypeDecl->genericDefinitionList.push_back(parseType(ctx));

				// Verifica se ha mais definicoes de generic
				if (m_lexer->isComma())
				{
					// Consome ','.
					m_lexer->expectToken(TokenType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			m_lexer->expectToken(TokenType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (m_lexer->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = parseInternalNamedType(ctx);
		}
		return namedTypeDecl;
	}

	std::unique_ptr<ast::TypeDeclNamed>
	Parser::parseInternalNamedType(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::ScopeResolution);
			m_lexer->expectIdentifier();

			if (m_lexer->isLessThan())
			{
				m_lexer->expectToken(TokenType_e::LessThan);
				while (true)
				{
					// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
					// serao quebrados em tokens menores.
					if (m_lexer->isBitWiseRightShift() || m_lexer->isBitWiseRightShiftAssign())
					{
						switch (m_lexer->getToken().type)
						{
						case TokenType_e::BitWiseRShift:		// >>
						case TokenType_e::GreaterThanOrEqual:	// >=
						case TokenType_e::BitWiseRShiftAssign:	// >>=
							m_lexer->reinterpretToken(TokenType_e::GreaterThan, 1);
							break;
						default:
							break;
						}
					}

					// Verifica se terminou a definicao dos generics
					if (m_lexer->isGreaterThan())
					{
						break;
					}

					parseType(ctx);

					// Verifica se ha mais definicoes de generic
					if (m_lexer->isComma())
					{
						// Consome ','.
						m_lexer->expectToken(TokenType_e::Comma);
						continue;
					}
				}

				// Consome '>'.
				m_lexer->expectToken(TokenType_e::GreaterThan);
			}

			// Verifica se ha identificadores internos.
			if (m_lexer->isScopeResolution())
			{
				parseInternalNamedType(ctx);
			}
			return nullptr;
		}

		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome '::'.
		m_lexer->expectToken(TokenType_e::ScopeResolution);

		// Consome o identificador.
		namedTypeDecl->identifier = m_lexer->expectIdentifier();

		// Verefica se a definicao de generic.
		if (m_lexer->isLessThan())
		{
			// Consome '<'.
			m_lexer->expectToken(TokenType_e::LessThan);

			while (true)
			{
				// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
				// serao quebrados em tokens menores.
				if (m_lexer->isBitWiseRightShift() || m_lexer->isBitWiseRightShiftAssign())
				{
					switch (m_lexer->getToken().type)
					{
					case TokenType_e::BitWiseRShift:		// >>
					case TokenType_e::GreaterThanOrEqual:	// >=
					case TokenType_e::BitWiseRShiftAssign:	// >>=
						m_lexer->reinterpretToken(TokenType_e::GreaterThan, 1);
						break;
					default:
						break;
					}
				}

				// Verifica se terminou a definicao dos generics
				if (m_lexer->isGreaterThan())
				{
					break;
				}

				// Processa a definicao de generic
				namedTypeDecl->genericDefinitionList.push_back(parseType(ctx));

				// Verifica se ha mais definicoes de generic
				if (m_lexer->isComma())
				{
					// Consome ','.
					m_lexer->expectToken(TokenType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			m_lexer->expectToken(TokenType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (m_lexer->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = parseInternalNamedType(ctx);
		}
		return namedTypeDecl;
	}

	std::unique_ptr<ast::ArrayDecl>
	Parser::parseArrayDecl(ParserContext_s& ctx)
	{
		const Bool skipOnly = ctx.isFirstPass && ctx.insideExpr;

		if (skipOnly)
		{
			m_lexer->expectToken(TokenType_e::LSquBracket);
			if (m_lexer->isRightSquBracket()) {
				m_lexer->expectToken(TokenType_e::RSquBracket);
				return nullptr;
			}
			m_lexer->expectConstantInteger();
			m_lexer->expectToken(TokenType_e::RSquBracket);
			return nullptr;
		}

		// Consome '['
		m_lexer->expectToken(TokenType_e::LSquBracket);

		// Unsized array
		if (m_lexer->isRightSquBracket()) {
			// Consome ']'
			m_lexer->expectToken(TokenType_e::RSquBracket);
			return std::make_unique<ast::UnsizedArrayDecl>(
				m_lexer->getToken().line,
				m_lexer->getToken().column
			);
		}

		auto sizedArrayDecl = std::make_unique<ast::SizedArrayDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);
		
		// TODO: Futuramente vai fazer o parse de uma expressao em tempode compilacao.
		// mas por hora vai fazer o parse de um I32.
		sizedArrayDecl->size = m_lexer->expectConstantInteger();

		// Consome ']'
		m_lexer->expectToken(TokenType_e::RSquBracket);

		return sizedArrayDecl;
	}

	std::unique_ptr<ast::ScopedIdentifierDecl>
	Parser::parseChildScopedIdentifiers(ParserContext_s& ctx)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>(
			m_lexer->getToken().line,
			m_lexer->getToken().column
		);

		// Consome '::'.
		m_lexer->expectToken(TokenType_e::ScopeResolution);

		// Consome o identificador.
		scopedIdentifierDecl->identifier = m_lexer->expectIdentifier();

		// Verifica se a mais declaracoes de escopo.
		if (m_lexer->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = parseChildScopedIdentifiers(ctx);
		}
		return scopedIdentifierDecl;
	}
} }