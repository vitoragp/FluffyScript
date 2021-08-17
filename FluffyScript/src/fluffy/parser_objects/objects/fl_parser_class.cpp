#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectClassDecl
	 */

	ClassDeclPtr ParserObjectClassDecl::parse(Parser* parser, Bool hasExport, Bool hasAbstract)
	{
		auto classDecl = std::make_unique<ast::ClassDecl>();

		classDecl->isExported = hasExport;
		classDecl->isAbstract = hasAbstract;

		// Consome 'class'.
		parser->expectToken([parser]() { return TokenSubType_e::Class; });

		// Consome o nome da classe.
		classDecl->name = parser->expectIdentifier();

		// Verifica se a declaracao de generic.
		if (parser->isLessThan())
		{
			classDecl->genericTemplateList = ParserObjectGenericTemplateDecl::parse(parser);
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
		parser->expectToken([parser]() { return TokenSubType_e::LBracket; });

		while (true)
		{
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

			default:
				throw exceptions::unexpected_token_exception(
					parser->getTokenValue(),
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
		}

		// Consome '}'.
		parser->expectToken([parser]() { return TokenSubType_e::RBracket; });

		return classDecl;
	}

	TypeDeclPtr ParserObjectClassDecl::parseExtends(Parser* parser)
	{
		// Consome 'extends'
		parser->expectToken([parser]() { return TokenSubType_e::Extends; });
		return ParserObjectTypeDecl::parse(parser);
	}

	TypeDeclPtrList ParserObjectClassDecl::parseImplements(Parser* parser)
	{
		TypeDeclPtrList interfaceList;

		// Consome 'implements'
		parser->expectToken([parser]() { return TokenSubType_e::Implements; });

		while (true)
		{
			// Consome o identificador com escopo.
			interfaceList.push_back(ParserObjectTypeDecl::parse(parser));

			if (parser->isComma())
			{
				// Consome ','
				parser->expectToken([parser]() { return TokenSubType_e::Comma; });
				continue;
			}
			break;
		}
		return interfaceList;
	}

	ClassFunctionDeclPtr ParserObjectClassDecl::parseStaticFunction(Parser* parser, TokenSubType_e accessModifier)
	{
		auto classFunctionPtr = std::make_unique<ast::ClassFunctionDecl>();

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
		parser->expectToken([parser]() { return TokenSubType_e::Fn; });

		// Consome o identificador.
		classFunctionPtr->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			classFunctionPtr->genericTemplateList = ParserObjectGenericTemplateDecl::parse(parser);
		}

		// Consome os parametros.
		classFunctionPtr->parameterList = ParserObjectClassDecl::parseParameterList(parser);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken([parser]() { return TokenSubType_e::Arrow; });

			// Consome o tipo retorno.
			classFunctionPtr->returnType = ParserObjectTypeDecl::parse(parser);
		}
		else
		{
			// Consome o tipo retorno.
			classFunctionPtr->returnType = std::make_unique<ast::TypeDeclVoid>();
		}

		// Consome '{'
		parser->expectToken([parser]() { return TokenSubType_e::LBracket; });

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			classFunctionPtr->blockDecl = ParserObjectBlockDecl::parse(parser);
		}

		// Consome '}'
		parser->expectToken([parser]() { return TokenSubType_e::RBracket; });

		return classFunctionPtr;
	}

	ClassFunctionDeclPtr ParserObjectClassDecl::parseFunction(Parser* parser, TokenSubType_e accessModifier)
	{
		auto classFunctionPtr = std::make_unique<ast::ClassFunctionDecl>();

		// Processa modificador 'abtract' ou 'virtual'.
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Abstract:
			parser->expectToken([parser]() { return TokenSubType_e::Abstract; });
			classFunctionPtr->isAbstract = true;
			break;
		case TokenSubType_e::Virtual:
			parser->expectToken([parser]() { return TokenSubType_e::Virtual; });
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
		parser->expectToken([parser]() { return TokenSubType_e::Fn; });

		// Consome o identificador.
		classFunctionPtr->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			classFunctionPtr->genericTemplateList = ParserObjectGenericTemplateDecl::parse(parser);
		}

		// Consome os parametros.
		classFunctionPtr->parameterList = ParserObjectClassDecl::parseParameterList(parser);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken([parser]() { return TokenSubType_e::Arrow; });

			// Consome o tipo retorno.
			classFunctionPtr->returnType = ParserObjectTypeDecl::parse(parser);
		}
		else
		{
			// Consome o tipo retorno.
			classFunctionPtr->returnType = std::make_unique<ast::TypeDeclVoid>();
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
					classFunctionPtr->identifier.c_str()
				);
			}
			parser->expectToken([parser]() { return TokenSubType_e::Override; });
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
					classFunctionPtr->identifier.c_str()
				);
			}
			parser->expectToken([parser]() { return TokenSubType_e::Final; });
			classFunctionPtr->isFinal = true;
			break;
		default:
			break;
		}

		if (classFunctionPtr->isAbstract)
		{
			// Consome ';'
			parser->expectToken([parser]() { return TokenSubType_e::SemiColon; });
			return classFunctionPtr;
		}

		// Consome '{'
		parser->expectToken([parser]() { return TokenSubType_e::LBracket; });

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			classFunctionPtr->blockDecl = ParserObjectBlockDecl::parse(parser);
		}

		// Consome '}'
		parser->expectToken([parser]() { return TokenSubType_e::RBracket; });

		return classFunctionPtr;
	}

	ClassVariableDeclPtr ParserObjectClassDecl::parseVariable(Parser* parser, TokenSubType_e accessModifier, Bool isStatic)
	{
		auto variableDecl = std::make_unique<ast::ClassVariableDecl>();

		// Define a variavel como estatica.
		variableDecl->isStatic = isStatic;

		// Atribui modificadores de acesso.
		switch (accessModifier) {
		case TokenSubType_e::Protected:
			variableDecl->accessModifier = ClassMemberAccessModifier_e::Protected;
			break;
		case TokenSubType_e::Private:
		case TokenSubType_e::Unknown:
			variableDecl->accessModifier = ClassMemberAccessModifier_e::Private;
			break;
		case TokenSubType_e::Public:
			variableDecl->accessModifier = ClassMemberAccessModifier_e::Public;
			break;
		default:
			break;
		}

		// Consome 'let' ou 'const'
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Let:
			parser->expectToken([parser]() { return TokenSubType_e::Let; });
			variableDecl->isConst = false;
			break;
		case TokenSubType_e::Const:
			parser->expectToken([parser]() { return TokenSubType_e::Const; });
			variableDecl->isConst = true;
			break;
		default:
			break;
		}

		// Verifica se o modificador 'ref' foi declarado.
		if (parser->isRef())
		{
			// Consome 'ref'.
			parser->expectToken([parser]() { return TokenSubType_e::Ref; });
			variableDecl->isReference = true;
		}

		// Consome identificador.
		variableDecl->identifier = parser->expectIdentifier();

		Bool mustHaveInitExpression = variableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (parser->isColon())
		{
			// Consome ':'.
			parser->expectToken([parser]() { return TokenSubType_e::Colon; });

			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();

			// Consome o tipo.
			variableDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

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
			parser->expectToken([parser]() { return TokenSubType_e::Assign; });

			// Processa expressao.
			variableDecl->initExpression = ParserObjectExpressionDecl::parse(parser);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		parser->expectToken([parser]() { return TokenSubType_e::SemiColon; });

		return variableDecl;
	}

	ClassFunctionParameterDeclPtrList ParserObjectClassDecl::parseParameterList(Parser* parser)
	{
		ClassFunctionParameterDeclPtrList parameterDeclList;

		// Consome '('
		parser->expectToken([parser]() { return TokenSubType_e::LParBracket; });

		// Finaliza declaracao da lista de parametros.
		if (parser->isRightParBracket())
		{
			// Consome ')'
			parser->expectToken([parser]() { return TokenSubType_e::RParBracket; });
			return parameterDeclList;
		}

		while (true)
		{
			auto parameterDecl = std::make_unique<ast::ClassFunctionParameterDecl>();

			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();

			parameterDecl->identifier = parser->expectIdentifier();

			// Consome ':'
			parser->expectToken([parser]() { return TokenSubType_e::Colon; });

			// Consome tipo
			parameterDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

			// Parametros nao podem ser do tipo void.
			if (parameterDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Parameter '%s' can't have void type",
					line,
					column,
					parameterDecl->identifier.c_str()
				);
			}

			// Adiciona o parametro a lista.
			parameterDeclList.push_back(std::move(parameterDecl));

			// Consome ','
			if (parser->isComma())
			{
				parser->expectToken([parser]() { return TokenSubType_e::Comma; });
				continue;
			}

			// Finaliza declaracao da lista de parametros.
			if (parser->isRightParBracket())
			{
				break;
			}

			throw exceptions::custom_exception("Expected ',' or ')' token",
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome ')'
		parser->expectToken([parser]() { return TokenSubType_e::RParBracket; });

		return parameterDeclList;
	}
} }