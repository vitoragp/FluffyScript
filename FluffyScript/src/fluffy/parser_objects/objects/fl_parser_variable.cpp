#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectVariableDecl
	 */

	VariableDeclPtr ParserObjectVariableDecl::parse(CompilationContext_t* ctx, Bool hasExport)
	{
		auto variableDecl = std::make_unique<ast::VariableDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		variableDecl->isExported = hasExport;

		// Consome 'let' ou 'const'
		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Let:
			ctx->parser->expectToken(TokenSubType_e::Let);
			variableDecl->isConst = false;
			break;
		case TokenSubType_e::Const:
			ctx->parser->expectToken(TokenSubType_e::Const);
			variableDecl->isConst = true;
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
			variableDecl->isReference = true;
		}

		// Consome identificador.
		variableDecl->identifier = ctx->parser->expectIdentifier();

		Bool mustHaveInitExpression = variableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (ctx->parser->isColon())
		{
			// Consome ':'.
			ctx->parser->expectToken(TokenSubType_e::Colon);

			const U32 line = ctx->parser->getTokenLine();
			const U32 column = ctx->parser->getTokenColumn();

			// Consome o tipo.
			variableDecl->typeDecl = ParserObjectTypeDecl::parse(ctx);

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
			ctx->parser->expectToken(TokenSubType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			variableDecl->initExpression = ParserObjectExpressionDecl::skip(ctx);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		ctx->parser->expectToken(TokenSubType_e::SemiColon);

		return variableDecl;
	}
} }