#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectEnumDecl
	 */

	EnumDeclPtr ParserObjectEnumDecl::parse(CompilationContext_t* ctx, Bool hasExport)
	{
		auto enumDecl = std::make_unique<ast::EnumDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		enumDecl->isExported = hasExport;

		// Consome 'enum'.
		ctx->parser->expectToken(TokenSubType_e::Enum);

		// Consome o identificador.
		enumDecl->identifier = ctx->parser->expectIdentifier();

		// Consome generic se houver.
		if (ctx->parser->isLessThan())
		{
			enumDecl->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome '{'.
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (ctx->parser->isRightBracket())
			{
				break;
			}

		parseEnumLabel:
			// Consome enum.
			enumDecl->enumItemDeclList.push_back(parserEnum(ctx));

			if (ctx->parser->isComma())
			{				
				// Consome ','.
				ctx->parser->expectToken(TokenSubType_e::Comma);
				goto parseEnumLabel;
			}

			if (ctx->parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_token_exception(
				ctx->parser->getTokenValue(),
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome '}'.
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return enumDecl;
	}

	EnumItemDeclPtr ParserObjectEnumDecl::parserEnum(CompilationContext_t* ctx)
	{
		auto enumItemDecl = std::make_unique<ast::EnumItemDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome o identificador.
		enumItemDecl->identifier = ctx->parser->expectIdentifier();

		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Assign:
			{
				// Consome '='.
				ctx->parser->expectToken(TokenSubType_e::Assign);
				enumItemDecl->hasValue = true;
				enumItemDecl->valueExpression = ParserObjectExpressionDecl::skipEnumExpr(ctx);
			}
			break;
		case TokenSubType_e::LParBracket:
			{
				// Consome '('.
				ctx->parser->expectToken(TokenSubType_e::LParBracket);

				// Indica presenca de dados.
				enumItemDecl->hasData = true;

				// Processa tipos.
				while (true)
				{
					// Consome Tipo
					enumItemDecl->dataTypeDeclList.push_back(ParserObjectTypeDecl::parse(ctx));

					if (ctx->parser->isComma())
					{
						// Consome ','.
						ctx->parser->expectToken(TokenSubType_e::Comma);
						continue;
					}

					if (ctx->parser->isRightParBracket())
					{
						break;
					}

					throw exceptions::unexpected_with_possibilities_token_exception(
						ctx->parser->getTokenValue(),
						{ TokenSubType_e::Comma, TokenSubType_e::RParBracket },
						ctx->parser->getTokenLine(),
						ctx->parser->getTokenColumn()
					);
				}				

				// Consome ')'.
				ctx->parser->expectToken(TokenSubType_e::RParBracket);
			}
			break;
		case TokenSubType_e::Comma:
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				ctx->parser->getTokenValue(),
				{ TokenSubType_e::Assign, TokenSubType_e::LParBracket, TokenSubType_e::Comma },
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		return enumItemDecl;
	}
} }