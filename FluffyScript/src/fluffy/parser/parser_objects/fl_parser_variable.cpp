#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectVariableDecl
	 */

	VariableDeclPtr ParserObjectVariableDecl::parse(Parser* parser, Bool hasExport)
	{
		auto variableDecl = std::make_unique<ast::VariableDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		variableDecl->isExported = hasExport;

		// Consome 'let' ou 'const'
		switch (parser->getTokenType())
		{
		case TokenType_e::Let:
			parser->expectToken(TokenType_e::Let);
			variableDecl->isConst = false;
			break;
		case TokenType_e::Const:
			parser->expectToken(TokenType_e::Const);
			variableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{ TokenType_e::Let, TokenType_e::Const },
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Verifica se o modificador 'ref' foi declarado.
		if (parser->isRef())
		{
			// Consome 'ref'.
			parser->expectToken(TokenType_e::Ref);
			variableDecl->isReference = true;
		}

		// Consome identificador.
		variableDecl->identifier = parser->expectIdentifier();

		Bool mustHaveInitExpression = variableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (parser->isColon())
		{
			// Consome ':'.
			parser->expectToken(TokenType_e::Colon);

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
			parser->expectToken(TokenType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			variableDecl->initExpression = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::Interrogation, true);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return variableDecl;
	}
} }