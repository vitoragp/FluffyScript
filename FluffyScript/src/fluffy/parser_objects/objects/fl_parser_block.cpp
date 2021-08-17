#include "../../fl_ast_expr.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectBlockDecl
	 */
	
	BlockDeclPtr ParserObjectBlockDecl::parse(Parser* parser)
	{
		throw exceptions::not_implemented_feature_exception("Block");
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::parseExprBlock(Parser* parser)
	{
		throw exceptions::not_implemented_feature_exception("Block expression");
		return nullptr;
	}

	BlockDeclPtr ParserObjectBlockDecl::parseBlockOrExprBlock(Parser* parser)
	{
		throw exceptions::not_implemented_feature_exception("Block or Block expression");
		return nullptr;
	}
} }