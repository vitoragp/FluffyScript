#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectGeneralDecl
	 */

	GeneralStmtPtr ParserObjectGeneralDecl::parse(CompilationContext_t* ctx)
	{
		Bool hasExport = false;
		Bool hasAbtract = false;

		// Verifica se houve a declararao para exportar o elemento.
		if (ctx->parser->isExport())
		{
			// Consome 'export'.
			ctx->parser->expectToken(TokenSubType_e::Export);
			hasExport = true;
		}

		// Verifica se a declaracao para a classe ser abstrata.
		if (ctx->parser->isAbstract())
		{
			// Consome 'abtract'.
			ctx->parser->expectToken(TokenSubType_e::Abstract);
			hasAbtract = true;

			// Obrigatoriamente 'abstract' deve se referir a uma classe.
			if (!ctx->parser->isClass()) {
				throw exceptions::unexpected_token_exception(ctx->parser->getTokenValue(), ctx->parser->getTokenLine(), ctx->parser->getTokenColumn());
			}
		}

		// export, abstract, class, interface, struct, enum, trait, let, fn

		// Verifica qual declaracao processar.
		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Class:
			return ParserObjectClassDecl::parse(ctx, hasExport, hasAbtract);
		case TokenSubType_e::Interface:
			return ParserObjectInterfaceDecl::parse(ctx, hasExport);
		case TokenSubType_e::Struct:
			return ParserObjectStructDecl::parse(ctx, hasExport);
		case TokenSubType_e::Enum:
			return ParserObjectEnumDecl::parse(ctx, hasExport);
		case TokenSubType_e::Trait:
			return ParserObjectTraitDecl::parse(ctx, hasExport);
		case TokenSubType_e::Fn:
			return ParserObjectFunctionDecl::parse(ctx, hasExport);
		case TokenSubType_e::Const:
		case TokenSubType_e::Let:
			return ParserObjectVariableDecl::parse(ctx, hasExport);
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				ctx->parser->getTokenValue(),
				{
					TokenSubType_e::Class,
					TokenSubType_e::Interface,
					TokenSubType_e::Struct,
					TokenSubType_e::Enum,
					TokenSubType_e::Trait,
					TokenSubType_e::Let,
					TokenSubType_e::Const,
					TokenSubType_e::Fn
				},
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		return nullptr;
	}
} }