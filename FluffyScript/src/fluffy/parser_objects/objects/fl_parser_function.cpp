#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectFunctionDecl
	 */

	FunctionDeclPtr ParserObjectFunctionDecl::parse(CompilationContext_t* ctx, Bool hasExport)
	{
		auto functionPtr = std::make_unique<ast::FunctionDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		functionPtr->isExported = hasExport;

		// Consome 'fn'
		ctx->parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		functionPtr->identifier = ctx->parser->expectIdentifier();

		// Consome o Generic.
		if (ctx->parser->isLessThan())
		{
			functionPtr->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome os parametros.
		functionPtr->parameterList = ParserObjectFunctionParameter::parse(ctx);

		// Consome o retorno se houver.
		if (ctx->parser->isArrow())
		{
			// Consome '->'
			ctx->parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			functionPtr->returnType = ParserObjectTypeDecl::parse(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			functionPtr->returnType = std::make_unique<ast::TypeDeclVoid>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Consome bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			functionPtr->blockDecl = ParserObjectBlockDecl::parse(ctx);
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return functionPtr;
	}
} }