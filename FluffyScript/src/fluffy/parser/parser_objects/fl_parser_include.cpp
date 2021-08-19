#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectIncludeDecl
	 */

	IncludeDeclPtr ParserObjectIncludeDecl::parse(CompilationContext_t* ctx)
	{
		auto includeDecl = std::make_unique<ast::IncludeDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'include'.
		ctx->parser->expectToken(TokenSubType_e::Include);

		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Consome Identificadores.
		while (true)
		{
			if (ctx->parser->isMultiplication()) {
				// Se existe identificaores declarados, o caractere coringa
				// nao pode ser usado.
				if (includeDecl->includedItemList.size()) {
					throw exceptions::unexpected_token_exception(ctx->parser->getTokenValue(), ctx->parser->getTokenLine(), ctx->parser->getTokenColumn());
				}

				// Consome '*'
				ctx->parser->expectToken(TokenSubType_e::Multiplication);
				break;
			}

			// Consome identificador.
			includeDecl->includedItemList.push_back(ctx->parser->expectIdentifier());

			// Verifica se ha mais declaracoes.
			if (!ctx->parser->isComma()) {
				break;
			}

			// Consome ','
			ctx->parser->expectToken(TokenSubType_e::Comma);
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		// Consome 'from'
		ctx->parser->expectToken(TokenSubType_e::From);

		// Consome o identificador do namespace.
		includeDecl->fromNamespace = ParserObjectScopedIdentifier::parse(ctx);

		// Consome ';'
		ctx->parser->expectToken(TokenSubType_e::SemiColon);

		return includeDecl;
	}
} }