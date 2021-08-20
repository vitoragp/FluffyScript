#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectClassDecl
	 */

	ClassDeclPtr ParserObjectClassDecl::parse(Parser* parser, Bool hasExport, Bool hasAbstract)
	{
		auto classDecl = std::make_unique<ast::ClassDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		classDecl->isExported = hasExport;
		classDecl->isAbstract = hasAbstract;

		// Consome 'class'.
		parser->expectToken(TokenSubType_e::Class);

		// Consome o nome da classe.
		classDecl->identifier = parser->expectIdentifier();

		// Verifica se a declaracao de generic.
		if (parser->isLessThan())
		{
			classDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Verifica se a declaracao de extends.
		if (parser->isExtends())
		{
			classDecl->baseClass = ParserObjectClassDecl::parseExtends(parser);
		}

		// Verifica se a declaracao de interfaces.
		if (parser->isImplements())
		{
			classDecl->interfaceList = ParserObjectClassDecl::parseImplements(parser);
		}

		// Consome '{'.
		parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			Bool hasAccessModifier = false;
			TokenSubType_e accessModifier = TokenSubType_e::Unknown;
			Bool staticModifier = false;

			// Finaliza a declaracao do bloco.
			if (parser->isRightBracket())
			{
				break;
			}

			// Processa declaracao de modificador de acesso.
			if (parser->isPublic() || parser->isProtected() || parser->isPrivate())
			{
				hasAccessModifier = true;
				accessModifier = parser->getTokenSubType();
				parser->nextToken();
			}

			// Processa modificador static
			if (parser->isStatic())
			{
				staticModifier = true;
				parser->nextToken();
			}

			switch (parser->getTokenSubType())
			{
			case TokenSubType_e::Abstract:
				if (staticModifier)
				{
					throw exceptions::custom_exception(
						"Static function can't be abstract",
						parser->getTokenLine(),
						parser->getTokenColumn()
					);
				}
				goto processFunction;

			case TokenSubType_e::Virtual:
				if (staticModifier)
				{
					throw exceptions::custom_exception(
						"Static function can't be virtual",
						parser->getTokenLine(),
						parser->getTokenColumn()
					);
				}
				goto processFunction;

			case TokenSubType_e::Fn:
			processFunction:
				if (staticModifier) {
					classDecl->staticFunctionList.push_back(parseStaticFunction(parser, accessModifier));
				} else {
					classDecl->functionList.push_back(parseFunction(parser, accessModifier));
				}
				break;

			case TokenSubType_e::Const:
			case TokenSubType_e::Let:
				if (staticModifier) {
					classDecl->staticVariableList.push_back(parseVariable(parser, accessModifier, staticModifier));
				} else {
					classDecl->variableList.push_back(parseVariable(parser, accessModifier, staticModifier));
				}
				break;

			case TokenSubType_e::Constructor:
				{
					if (staticModifier)
					{
						throw exceptions::custom_exception(
							"Constructors can't be static",
							parser->getTokenLine(),
							parser->getTokenColumn()
						);
					}
					classDecl->constructorList.push_back(parseConstructor(parser, accessModifier));
				}
				break;

			case TokenSubType_e::Destructor:
				{
					if (staticModifier)
					{
						throw exceptions::custom_exception(
							"Destructor can't be static",
							parser->getTokenLine(),
							parser->getTokenColumn()
						);
					}

					if (hasAccessModifier)
					{
						throw exceptions::custom_exception(
							"Destructors are public, can't have any access modifier",
							parser->getTokenLine(),
							parser->getTokenColumn()
						);
					}
					classDecl->destructorDecl = parseDestructor(parser);
				}
				break;

			default:
				throw exceptions::unexpected_with_possibilities_token_exception(
					parser->getTokenValue(),
					{
						TokenSubType_e::Public, TokenSubType_e::Protected, TokenSubType_e::Private,
						TokenSubType_e::Let, TokenSubType_e::Const,
						TokenSubType_e::Virtual, TokenSubType_e::Abstract, TokenSubType_e::Fn,
						TokenSubType_e::LBracket
					},
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
		}

		// Consome '}'.
		parser->expectToken(TokenSubType_e::RBracket);

		return classDecl;
	}

	TypeDeclPtr ParserObjectClassDecl::parseExtends(Parser* parser)
	{
		// Consome 'extends'
		parser->expectToken(TokenSubType_e::Extends);
		return ParserObjectTypeDecl::parse(parser);
	}

	TypeDeclPtrList ParserObjectClassDecl::parseImplements(Parser* parser)
	{
		TypeDeclPtrList interfaceList;

		// Consome 'implements'
		parser->expectToken(TokenSubType_e::Implements);

		while (true)
		{
			// Consome o identificador com escopo.
			interfaceList.push_back(ParserObjectTypeDecl::parse(parser));

			if (parser->isComma())
			{
				// Consome ','
				parser->expectToken(TokenSubType_e::Comma);
				continue;
			}
			break;
		}
		return interfaceList;
	}

	ClassFunctionDeclPtr ParserObjectClassDecl::parseStaticFunction(Parser* parser, TokenSubType_e accessModifier)
	{
		auto classFunctionPtr = std::make_unique<ast::ClassFunctionDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Atribui modificador static.
		classFunctionPtr->isStatic = true;

		// Atribui modificadores de acesso.
		switch (accessModifier) {
		case TokenSubType_e::Protected:
			classFunctionPtr->accessModifier = ClassMemberAccessModifier_e::Protected;
			break;
		case TokenSubType_e::Unknown:
		case TokenSubType_e::Private:
			classFunctionPtr->accessModifier = ClassMemberAccessModifier_e::Private;
			break;
		case TokenSubType_e::Public:
			classFunctionPtr->accessModifier = ClassMemberAccessModifier_e::Public;
			break;
		default:
			break;
		}

		// Consome 'fn'
		parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		classFunctionPtr->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			classFunctionPtr->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome os parametros.
		classFunctionPtr->parameterList = ParserObjectFunctionParameter::parse(parser);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			classFunctionPtr->returnType = ParserObjectTypeDecl::parse(parser);
		}
		else
		{
			// Consome o tipo retorno.
			classFunctionPtr->returnType = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			classFunctionPtr->blockDecl = ParserObjectBlockDecl::parse(parser);
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);

		return classFunctionPtr;
	}

	ClassFunctionDeclPtr ParserObjectClassDecl::parseFunction(Parser* parser, TokenSubType_e accessModifier)
	{
		auto classFunctionPtr = std::make_unique<ast::ClassFunctionDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Processa modificador 'abtract' ou 'virtual'.
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Abstract:
			parser->expectToken(TokenSubType_e::Abstract);
			classFunctionPtr->isAbstract = true;
			break;
		case TokenSubType_e::Virtual:
			parser->expectToken(TokenSubType_e::Virtual);
			classFunctionPtr->isVirtual = true;
			break;
		default:
			break;
		}

		// Atribui modificadores de acesso.
		switch (accessModifier) {
		case TokenSubType_e::Protected:
			classFunctionPtr->accessModifier = ClassMemberAccessModifier_e::Protected;
			break;
		case TokenSubType_e::Private:
		case TokenSubType_e::Unknown:
			classFunctionPtr->accessModifier = ClassMemberAccessModifier_e::Private;
			break;
		case TokenSubType_e::Public:
			classFunctionPtr->accessModifier = ClassMemberAccessModifier_e::Public;
			break;
		default:
			break;
		}

		// Consome 'fn'
		parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		classFunctionPtr->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			classFunctionPtr->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome os parametros.
		classFunctionPtr->parameterList = ParserObjectFunctionParameter::parse(parser);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			classFunctionPtr->returnType = ParserObjectTypeDecl::parse(parser);
		}
		else
		{
			// Consome o tipo retorno.
			classFunctionPtr->returnType = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Override:
			// Funcoes abstratas nao podem ser override ou final
			if (classFunctionPtr->isAbstract)
			{
				throw exceptions::custom_exception(
					"Abstract function '%s' can't have override or final modifiers",
					parser->getTokenLine(),
					parser->getTokenColumn(),
					classFunctionPtr->identifier.str()
				);
			}
			parser->expectToken(TokenSubType_e::Override);
			classFunctionPtr->isOverride = true;
			break;
		case TokenSubType_e::Final:
			// Funcoes abstratas nao podem ser override ou final
			if (classFunctionPtr->isAbstract)
			{
				throw exceptions::custom_exception(
					"Abstract function '%s' can't have override or final modifiers",
					parser->getTokenLine(),
					parser->getTokenColumn(),
					classFunctionPtr->identifier.str()
				);
			}
			parser->expectToken(TokenSubType_e::Final);
			classFunctionPtr->isFinal = true;
			break;
		default:
			break;
		}

		if (classFunctionPtr->isAbstract)
		{
			// Consome ';'
			parser->expectToken(TokenSubType_e::SemiColon);
			return classFunctionPtr;
		}

		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			classFunctionPtr->blockDecl = ParserObjectBlockDecl::parse(parser);
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);

		return classFunctionPtr;
	}

	ClassVariableDeclPtr ParserObjectClassDecl::parseVariable(Parser* parser, TokenSubType_e accessModifier, Bool isStatic)
	{
		auto classVariableDecl = std::make_unique<ast::ClassVariableDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Define a variavel como estatica.
		classVariableDecl->isStatic = isStatic;

		// Atribui modificadores de acesso.
		switch (accessModifier) {
		case TokenSubType_e::Protected:
			classVariableDecl->accessModifier = ClassMemberAccessModifier_e::Protected;
			break;
		case TokenSubType_e::Private:
		case TokenSubType_e::Unknown:
			classVariableDecl->accessModifier = ClassMemberAccessModifier_e::Private;
			break;
		case TokenSubType_e::Public:
			classVariableDecl->accessModifier = ClassMemberAccessModifier_e::Public;
			break;
		default:
			break;
		}

		// Consome 'let' ou 'const'
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Let:
			parser->expectToken(TokenSubType_e::Let);
			classVariableDecl->isConst = false;
			break;
		case TokenSubType_e::Const:
			parser->expectToken(TokenSubType_e::Const);
			classVariableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{ TokenSubType_e::Let, TokenSubType_e::Const },
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Verifica se o modificador 'ref' foi declarado.
		if (parser->isRef())
		{
			// Consome 'ref'.
			parser->expectToken(TokenSubType_e::Ref);
			classVariableDecl->isReference = true;
		}

		// Consome identificador.
		classVariableDecl->identifier = parser->expectIdentifier();

		Bool mustHaveInitExpression = classVariableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (parser->isColon())
		{
			// Consome ':'.
			parser->expectToken(TokenSubType_e::Colon);

			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();

			// Consome o tipo.
			classVariableDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

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
			parser->expectToken(TokenSubType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			classVariableDecl->initExpression = ParserObjectExpressionDecl::skip(parser);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return classVariableDecl;
	}

	ClassConstructorDeclPtr ParserObjectClassDecl::parseConstructor(Parser* parser, TokenSubType_e accessModifier)
	{
		auto classConstructorDecl = std::make_unique<ast::ClassConstructorDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);
		
		// Consome 'constructor'.
		parser->expectToken(TokenSubType_e::Constructor);

		// Consome os parametros.
		classConstructorDecl->parameterList = ParserObjectFunctionParameter::parse(parser);

		// Verifica se a declaracao do construtor base.
		if (parser->isColon())
		{
			// Consome ':'.
			parser->expectToken(TokenSubType_e::Colon);

			if (parser->isSuper())
			{
				// Consome 'super'.
				parser->expectToken(TokenSubType_e::Super);

				// Consome '('.
				parser->expectToken(TokenSubType_e::LParBracket);

				// Consome os parametros.
				classConstructorDecl->superParameters = ParserObjectExpressionDecl::skip(parser);

				// Consome ')'.
				parser->expectToken(TokenSubType_e::RParBracket);
			} else {
				auto variableInitDecl = std::make_unique<ast::ClassVariableInitDecl>(
					parser->getTokenLine(),
					parser->getTokenColumn()
					);

				// Consome o identificador
				variableInitDecl->identifier = parser->expectIdentifier();

				// Consome '('.
				parser->expectToken(TokenSubType_e::LParBracket);

				variableInitDecl->initExpression = ParserObjectExpressionDecl::skip(parser);

				// Consome ')'.
				parser->expectToken(TokenSubType_e::RParBracket);

				// Adiciona inicializacao de variavel a lista.
				classConstructorDecl->variableInitDeclList.push_back(std::move(variableInitDecl));
			}

			// Consome inicializacao e variaveis.
			while (true)
			{
				if (!parser->isComma())
				{
					break;
				}

				// Consome ','.
				parser->expectToken(TokenSubType_e::Comma);

				auto variableInitDecl = std::make_unique<ast::ClassVariableInitDecl>(
					parser->getTokenLine(),
					parser->getTokenColumn()
				);

				// Consome o identificador
				variableInitDecl->identifier = parser->expectIdentifier();

				// Consome '('.
				parser->expectToken(TokenSubType_e::LParBracket);

				variableInitDecl->initExpression = ParserObjectExpressionDecl::skip(parser);

				// Consome ')'.
				parser->expectToken(TokenSubType_e::RParBracket);

				// Adiciona inicializacao de variavel a lista.
				classConstructorDecl->variableInitDeclList.push_back(std::move(variableInitDecl));
			}
		}

		// Console bloco.
		classConstructorDecl->blockDecl = ParserObjectBlockDecl::parse(parser);

		return classConstructorDecl;
	}

	ClassDestructorDeclPtr ParserObjectClassDecl::parseDestructor(Parser* parser)
	{
		auto classDestructorDecl = std::make_unique<ast::ClassDestructorDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'destructor'.
		parser->expectToken(TokenSubType_e::Destructor);

		// Consome '('.
		parser->expectToken(TokenSubType_e::LParBracket);

		// Consome ')'.
		parser->expectToken(TokenSubType_e::RParBracket);

		// Console bloco..
		classDestructorDecl->blockDecl = ParserObjectBlockDecl::parse(parser);

		return classDestructorDecl;
	}
} }