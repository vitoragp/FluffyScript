#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectTraitDecl
	 */

	TraitDeclPtr ParserObjectTraitDecl::parse(CompilationContext_t* ctx, Bool hasExport)
	{
		auto traitDecl = std::make_unique<ast::TraitDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		traitDecl->isExported = hasExport;

		// Consome 'interface'.
		ctx->parser->expectToken(TokenSubType_e::Interface);

		// Consome o identificador.
		traitDecl->identifier = ctx->parser->expectIdentifier();

		// Consome generic se houver.
		if (ctx->parser->isLessThan())
		{
			traitDecl->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Verifica se e um definicao de trait.
		if (ctx->parser->isFor())
		{
			traitDecl->isDefinition = true;

			// Consome 'for'.
			ctx->parser->expectToken(TokenSubType_e::For);

			// Consome o tipo para qual o trait esta sendo implementado.
			traitDecl->typeDefinitionDecl = ParserObjectTypeDecl::parseWithSelf(ctx);
		}

		// Consome '{'.
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (ctx->parser->isRightBracket())
			{
				break;
			}

		parseFunctionLabel:
			if (ctx->parser->isFn())
			{
				// Consome a funcao.
				traitDecl->functionDeclList.push_back(parserFunction(ctx));
				goto parseFunctionLabel;
			}

			if (ctx->parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				ctx->parser->getTokenValue(),
				{ TokenSubType_e::Fn, TokenSubType_e::RBracket },
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome '}'.
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return traitDecl;
	} 

	TraitFunctionDeclPtr ParserObjectTraitDecl::parserFunction(CompilationContext_t* ctx)
	{
		auto traitFunctionDecl = std::make_unique<ast::TraitFunctionDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'fn'
		ctx->parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		traitFunctionDecl->identifier = ctx->parser->expectIdentifier();

		// Consome o Generic.
		if (ctx->parser->isLessThan())
		{
			traitFunctionDecl->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome os parametros.
		traitFunctionDecl->parameterList = ParserObjectFunctionParameter::parseWithSelf(ctx);

		// Consome o retorno se houver.
		if (ctx->parser->isArrow())
		{
			// Consome '->'
			ctx->parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			traitFunctionDecl->returnType = ParserObjectTypeDecl::parseWithSelf(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			traitFunctionDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome ';'
		ctx->parser->expectToken(TokenSubType_e::SemiColon);

		return traitFunctionDecl;
	}
} }