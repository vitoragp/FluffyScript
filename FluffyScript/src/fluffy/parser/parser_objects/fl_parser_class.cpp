#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectClassDecl
	 */

	ClassDeclPtr ParserObjectClassDecl::parse(CompilationContext_t* ctx, Bool hasExport, Bool hasAbstract)
	{
		auto classDecl = std::make_unique<ast::ClassDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		classDecl->isExported = hasExport;
		classDecl->isAbstract = hasAbstract;

		// Consome 'class'.
		ctx->parser->expectToken(TokenSubType_e::Class);

		// Consome o nome da classe.
		classDecl->identifier = ctx->parser->expectIdentifier();

		// Verifica se a declaracao de generic.
		if (ctx->parser->isLessThan())
		{
			classDecl->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Verifica se a declaracao de extends.
		if (ctx->parser->isExtends())
		{
			classDecl->baseClass = ParserObjectClassDecl::parseExtends(ctx);
		}

		// Verifica se a declaracao de interfaces.
		if (ctx->parser->isImplements())
		{
			classDecl->interfaceList = ParserObjectClassDecl::parseImplements(ctx);
		}

		// Consome '{'.
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			Bool hasAccessModifier = false;
			TokenSubType_e accessModifier = TokenSubType_e::Unknown;
			Bool staticModifier = false;

			// Finaliza a declaracao do bloco.
			if (ctx->parser->isRightBracket())
			{
				break;
			}

			// Processa declaracao de modificador de acesso.
			if (ctx->parser->isPublic() || ctx->parser->isProtected() || ctx->parser->isPrivate())
			{
				hasAccessModifier = true;
				accessModifier = ctx->parser->getTokenSubType();
				ctx->parser->nextToken();
			}

			// Processa modificador static
			if (ctx->parser->isStatic())
			{
				staticModifier = true;
				ctx->parser->nextToken();
			}

			switch (ctx->parser->getTokenSubType())
			{
			case TokenSubType_e::Abstract:
				if (staticModifier)
				{
					throw exceptions::custom_exception(
						"Static function can't be abstract",
						ctx->parser->getTokenLine(),
						ctx->parser->getTokenColumn()
					);
				}
				goto processFunction;

			case TokenSubType_e::Virtual:
				if (staticModifier)
				{
					throw exceptions::custom_exception(
						"Static function can't be virtual",
						ctx->parser->getTokenLine(),
						ctx->parser->getTokenColumn()
					);
				}
				goto processFunction;

			case TokenSubType_e::Fn:
			processFunction:
				if (staticModifier) {
					classDecl->staticFunctionList.push_back(parseStaticFunction(ctx, accessModifier));
				} else {
					classDecl->functionList.push_back(parseFunction(ctx, accessModifier));
				}
				break;

			case TokenSubType_e::Const:
			case TokenSubType_e::Let:
				if (staticModifier) {
					classDecl->staticVariableList.push_back(parseVariable(ctx, accessModifier, staticModifier));
				} else {
					classDecl->variableList.push_back(parseVariable(ctx, accessModifier, staticModifier));
				}
				break;

			case TokenSubType_e::Constructor:
				{
					if (staticModifier)
					{
						throw exceptions::custom_exception(
							"Constructors can't be static",
							ctx->parser->getTokenLine(),
							ctx->parser->getTokenColumn()
						);
					}
					classDecl->constructorList.push_back(parseConstructor(ctx, accessModifier));
				}
				break;

			case TokenSubType_e::Destructor:
				{
					if (staticModifier)
					{
						throw exceptions::custom_exception(
							"Destructor can't be static",
							ctx->parser->getTokenLine(),
							ctx->parser->getTokenColumn()
						);
					}

					if (hasAccessModifier)
					{
						throw exceptions::custom_exception(
							"Destructors are public, can't have any access modifier",
							ctx->parser->getTokenLine(),
							ctx->parser->getTokenColumn()
						);
					}
					classDecl->destructorDecl = parseDestructor(ctx);
				}
				break;

			default:
				throw exceptions::unexpected_with_possibilities_token_exception(
					ctx->parser->getTokenValue(),
					{
						TokenSubType_e::Public, TokenSubType_e::Protected, TokenSubType_e::Private,
						TokenSubType_e::Let, TokenSubType_e::Const,
						TokenSubType_e::Virtual, TokenSubType_e::Abstract, TokenSubType_e::Fn,
						TokenSubType_e::LBracket
					},
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn()
				);
			}
		}

		// Consome '}'.
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return classDecl;
	}

	TypeDeclPtr ParserObjectClassDecl::parseExtends(CompilationContext_t* ctx)
	{
		// Consome 'extends'
		ctx->parser->expectToken(TokenSubType_e::Extends);
		return ParserObjectTypeDecl::parse(ctx);
	}

	TypeDeclPtrList ParserObjectClassDecl::parseImplements(CompilationContext_t* ctx)
	{
		TypeDeclPtrList interfaceList;

		// Consome 'implements'
		ctx->parser->expectToken(TokenSubType_e::Implements);

		while (true)
		{
			// Consome o identificador com escopo.
			interfaceList.push_back(ParserObjectTypeDecl::parse(ctx));

			if (ctx->parser->isComma())
			{
				// Consome ','
				ctx->parser->expectToken(TokenSubType_e::Comma);
				continue;
			}
			break;
		}
		return interfaceList;
	}

	ClassFunctionDeclPtr ParserObjectClassDecl::parseStaticFunction(CompilationContext_t* ctx, TokenSubType_e accessModifier)
	{
		auto classFunctionPtr = std::make_unique<ast::ClassFunctionDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
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
		ctx->parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		classFunctionPtr->identifier = ctx->parser->expectIdentifier();

		// Consome o Generic.
		if (ctx->parser->isLessThan())
		{
			classFunctionPtr->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome os parametros.
		classFunctionPtr->parameterList = ParserObjectFunctionParameter::parse(ctx);

		// Consome o retorno se houver.
		if (ctx->parser->isArrow())
		{
			// Consome '->'
			ctx->parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			classFunctionPtr->returnType = ParserObjectTypeDecl::parse(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			classFunctionPtr->returnType = std::make_unique<ast::TypeDeclVoid>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			classFunctionPtr->blockDecl = ParserObjectBlockDecl::parse(ctx);
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return classFunctionPtr;
	}

	ClassFunctionDeclPtr ParserObjectClassDecl::parseFunction(CompilationContext_t* ctx, TokenSubType_e accessModifier)
	{
		auto classFunctionPtr = std::make_unique<ast::ClassFunctionDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Processa modificador 'abtract' ou 'virtual'.
		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Abstract:
			ctx->parser->expectToken(TokenSubType_e::Abstract);
			classFunctionPtr->isAbstract = true;
			break;
		case TokenSubType_e::Virtual:
			ctx->parser->expectToken(TokenSubType_e::Virtual);
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
		ctx->parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		classFunctionPtr->identifier = ctx->parser->expectIdentifier();

		// Consome o Generic.
		if (ctx->parser->isLessThan())
		{
			classFunctionPtr->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome os parametros.
		classFunctionPtr->parameterList = ParserObjectFunctionParameter::parse(ctx);

		// Consome o retorno se houver.
		if (ctx->parser->isArrow())
		{
			// Consome '->'
			ctx->parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			classFunctionPtr->returnType = ParserObjectTypeDecl::parse(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			classFunctionPtr->returnType = std::make_unique<ast::TypeDeclVoid>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Override:
			// Funcoes abstratas nao podem ser override ou final
			if (classFunctionPtr->isAbstract)
			{
				throw exceptions::custom_exception(
					"Abstract function '%s' can't have override or final modifiers",
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn(),
					classFunctionPtr->identifier.c_str()
				);
			}
			ctx->parser->expectToken(TokenSubType_e::Override);
			classFunctionPtr->isOverride = true;
			break;
		case TokenSubType_e::Final:
			// Funcoes abstratas nao podem ser override ou final
			if (classFunctionPtr->isAbstract)
			{
				throw exceptions::custom_exception(
					"Abstract function '%s' can't have override or final modifiers",
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn(),
					classFunctionPtr->identifier.c_str()
				);
			}
			ctx->parser->expectToken(TokenSubType_e::Final);
			classFunctionPtr->isFinal = true;
			break;
		default:
			break;
		}

		if (classFunctionPtr->isAbstract)
		{
			// Consome ';'
			ctx->parser->expectToken(TokenSubType_e::SemiColon);
			return classFunctionPtr;
		}

		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			classFunctionPtr->blockDecl = ParserObjectBlockDecl::parse(ctx);
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return classFunctionPtr;
	}

	ClassVariableDeclPtr ParserObjectClassDecl::parseVariable(CompilationContext_t* ctx, TokenSubType_e accessModifier, Bool isStatic)
	{
		auto classVariableDecl = std::make_unique<ast::ClassVariableDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
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
		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Let:
			ctx->parser->expectToken(TokenSubType_e::Let);
			classVariableDecl->isConst = false;
			break;
		case TokenSubType_e::Const:
			ctx->parser->expectToken(TokenSubType_e::Const);
			classVariableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				ctx->parser->getTokenValue(),
				{ TokenSubType_e::Let, TokenSubType_e::Const },
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Verifica se o modificador 'ref' foi declarado.
		if (ctx->parser->isRef())
		{
			// Consome 'ref'.
			ctx->parser->expectToken(TokenSubType_e::Ref);
			classVariableDecl->isReference = true;
		}

		// Consome identificador.
		classVariableDecl->identifier = ctx->parser->expectIdentifier();

		Bool mustHaveInitExpression = classVariableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (ctx->parser->isColon())
		{
			// Consome ':'.
			ctx->parser->expectToken(TokenSubType_e::Colon);

			const U32 line = ctx->parser->getTokenLine();
			const U32 column = ctx->parser->getTokenColumn();

			// Consome o tipo.
			classVariableDecl->typeDecl = ParserObjectTypeDecl::parse(ctx);

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
			ctx->parser->expectToken(TokenSubType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			classVariableDecl->initExpression = ParserObjectExpressionDecl::skip(ctx);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		ctx->parser->expectToken(TokenSubType_e::SemiColon);

		return classVariableDecl;
	}

	ClassConstructorDeclPtr ParserObjectClassDecl::parseConstructor(CompilationContext_t* ctx, TokenSubType_e accessModifier)
	{
		auto classConstructorDecl = std::make_unique<ast::ClassConstructorDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);
		
		// Consome 'constructor'.
		ctx->parser->expectToken(TokenSubType_e::Constructor);

		// Consome os parametros.
		classConstructorDecl->parameterList = ParserObjectFunctionParameter::parse(ctx);

		// Verifica se a declaracao do construtor base.
		if (ctx->parser->isColon())
		{
			// Consome ':'.
			ctx->parser->expectToken(TokenSubType_e::Colon);

			if (ctx->parser->isSuper())
			{
				// Consome 'super'.
				ctx->parser->expectToken(TokenSubType_e::Super);

				// Consome '('.
				ctx->parser->expectToken(TokenSubType_e::LParBracket);

				// Consome os parametros.
				classConstructorDecl->superParameters = ParserObjectExpressionDecl::skip(ctx);

				// Consome ')'.
				ctx->parser->expectToken(TokenSubType_e::RParBracket);
			} else {
				auto variableInitDecl = std::make_unique<ast::ClassVariableInitDecl>(
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn()
					);

				// Consome o identificador
				variableInitDecl->identifier = ctx->parser->expectIdentifier();

				// Consome '('.
				ctx->parser->expectToken(TokenSubType_e::LParBracket);

				variableInitDecl->initExpression = ParserObjectExpressionDecl::skip(ctx);

				// Consome ')'.
				ctx->parser->expectToken(TokenSubType_e::RParBracket);

				// Adiciona inicializacao de variavel a lista.
				classConstructorDecl->variableInitDeclList.push_back(std::move(variableInitDecl));
			}

			// Consome inicializacao e variaveis.
			while (true)
			{
				if (!ctx->parser->isComma())
				{
					break;
				}

				// Consome ','.
				ctx->parser->expectToken(TokenSubType_e::Comma);

				auto variableInitDecl = std::make_unique<ast::ClassVariableInitDecl>(
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn()
				);

				// Consome o identificador
				variableInitDecl->identifier = ctx->parser->expectIdentifier();

				// Consome '('.
				ctx->parser->expectToken(TokenSubType_e::LParBracket);

				variableInitDecl->initExpression = ParserObjectExpressionDecl::skip(ctx);

				// Consome ')'.
				ctx->parser->expectToken(TokenSubType_e::RParBracket);

				// Adiciona inicializacao de variavel a lista.
				classConstructorDecl->variableInitDeclList.push_back(std::move(variableInitDecl));
			}
		}

		// Console bloco..
		classConstructorDecl->blockDecl = ParserObjectBlockDecl::skip(ctx);

		return classConstructorDecl;
	}

	ClassDestructorDeclPtr ParserObjectClassDecl::parseDestructor(CompilationContext_t* ctx)
	{
		auto classDestructorDecl = std::make_unique<ast::ClassDestructorDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'destructor'.
		ctx->parser->expectToken(TokenSubType_e::Destructor);

		// Consome '('.
		ctx->parser->expectToken(TokenSubType_e::LParBracket);

		// Consome ')'.
		ctx->parser->expectToken(TokenSubType_e::RParBracket);

		// Console bloco..
		classDestructorDecl->blockDecl = ParserObjectBlockDecl::skip(ctx);

		return classDestructorDecl;
	}
} }