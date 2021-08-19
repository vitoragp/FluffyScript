#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectScopedIdentifier
	 */

	ScopedIdentifierDeclPtr	ParserObjectScopedIdentifier::parse(CompilationContext_t* ctx)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Se o escopo vem antes identificador
		if (ctx->parser->isScopeResolution())
		{
			// Consome '::'.
			ctx->parser->expectToken(TokenSubType_e::ScopeResolution);
			scopedIdentifierDecl->startFromRoot = true;
		}

		// Consome o identificador.
		scopedIdentifierDecl->identifier = ctx->parser->expectIdentifier();

		// Verifica se a mais declaracoes de escopo.
		if (ctx->parser->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = ParserObjectScopedIdentifier::parseChildScopedIdentifiers(ctx);
		}
		return scopedIdentifierDecl;
	}

	ScopedIdentifierDeclPtr ParserObjectScopedIdentifier::parseChildScopedIdentifiers(CompilationContext_t* ctx)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome '::'.
		ctx->parser->expectToken(TokenSubType_e::ScopeResolution);

		// Consome o identificador.
		scopedIdentifierDecl->identifier = ctx->parser->expectIdentifier();

		// Verifica se a mais declaracoes de escopo.
		if (ctx->parser->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = ParserObjectScopedIdentifier::parseChildScopedIdentifiers(ctx);
		}
		return scopedIdentifierDecl;
	}
} }