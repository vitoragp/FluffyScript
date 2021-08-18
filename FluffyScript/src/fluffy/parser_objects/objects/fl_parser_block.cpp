#include "../../fl_ast_expr.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectBlockDecl
	 */
	
	BlockDeclPtr ParserObjectBlockDecl::parse(Parser* parser)
	{
		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block");
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::skip(Parser* parser)
	{
		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block");
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::parseExprBlock(Parser* parser)
	{
		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block expression");
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::skipExprBlock(Parser* parser)
	{
		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block expression");
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::parseBlockOrExprBlock(Parser* parser)
	{
		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block or Block expression");
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::skipBlockOrExprBlock(Parser* parser)
	{
		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Console bloco se houver.
		if (!parser->isRightBracket())
		{
			throw exceptions::not_implemented_feature_exception("Block or Block expression");
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);
		return nullptr;
	}
} }