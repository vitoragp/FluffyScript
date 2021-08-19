#include "..\fl_ast_expr.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectBlockDecl
	 */
	
	BlockDeclPtr ParserObjectBlockDecl::parse(CompilationContext_t* ctx)
	{
		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block");
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::skip(CompilationContext_t* ctx)
	{
		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block");
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::parseExprBlock(CompilationContext_t* ctx)
	{
		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block expression");
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::skipExprBlock(CompilationContext_t* ctx)
	{
		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block expression");
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::parseBlockOrExprBlock(CompilationContext_t* ctx)
	{
		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block or Block expression");
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::skipBlockOrExprBlock(CompilationContext_t* ctx)
	{
		// Consome '{'
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!ctx->parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block or Block expression");
		}

		// Consome '}'
		ctx->parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}
} }