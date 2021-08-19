#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectStructDecl
	 */

	StructDeclPtr ParserObjectStructDecl::parse(CompilationContext_t* ctx, Bool hasExport)
	{
		auto structDecl = std::make_unique<ast::StructDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		structDecl->isExported = hasExport;

		// Consome 'struct'.
		ctx->parser->expectToken(TokenSubType_e::Struct);

		// Consome o identificador.
		structDecl->identifier = ctx->parser->expectIdentifier();

		// Consome generic se houver.
		if (ctx->parser->isLessThan())
		{
			structDecl->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome '{'.
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (ctx->parser->isRightBracket())
			{
				break;
			}

		parseVariableLabel:
			if (ctx->parser->isLet() || ctx->parser->isConst())
			{
				// Consome a funcao.
				structDecl->variableList.push_back(parserVariable(ctx));				
				goto parseVariableLabel;
			}

			if (ctx->parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				ctx->parser->getTokenValue(),
				{ TokenSubType_e::Let, TokenSubType_e::Const, TokenSubType_e::RBracket },
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome '}'.
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return structDecl;
	}

	StructVariableDeclPtr ParserObjectStructDecl::parserVariable(CompilationContext_t* ctx)
	{
		auto structVariableDecl = std::make_unique<ast::StructVariableDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Let:
			ctx->parser->expectToken(TokenSubType_e::Let);
			structVariableDecl->isConst = false;
			break;
		case TokenSubType_e::Const:
			ctx->parser->expectToken(TokenSubType_e::Const);
			structVariableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				ctx->parser->getTokenValue(),
				{ TokenSubType_e::Let, TokenSubType_e::Const },
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Verifica se possui o identificador 'ref'.
		if (ctx->parser->isRef())
		{
			// Consome 'ref'.
			ctx->parser->expectToken(TokenSubType_e::Ref);
			structVariableDecl->isReference = true;
		}

		// Consome identificador.
		structVariableDecl->identifier = ctx->parser->expectIdentifier();

		Bool mustHaveInitExpression = structVariableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (ctx->parser->isColon())
		{
			// Consome ':'.
			ctx->parser->expectToken(TokenSubType_e::Colon);

			const U32 line = ctx->parser->getTokenLine();
			const U32 column = ctx->parser->getTokenColumn();

			// Consome o tipo.
			structVariableDecl->typeDecl = ParserObjectTypeDecl::parse(ctx);

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
			ctx->parser->expectToken(TokenSubType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			structVariableDecl->initExpression = ParserObjectExpressionDecl::skip(ctx);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		ctx->parser->expectToken(TokenSubType_e::SemiColon);

		return structVariableDecl;
	}
} }