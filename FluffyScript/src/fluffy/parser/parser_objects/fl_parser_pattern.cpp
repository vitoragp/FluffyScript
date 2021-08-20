#include "..\fl_ast_expr.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectPatternDecl
	 */
	
	PatternDeclPtr ParserObjectPatternDecl::parse(Parser* parser)
	{
		throw exceptions::not_implemented_feature_exception("Parser pattern");
		return nullptr;
	}

	PatternDeclPtr ParserObjectPatternDecl::skip(Parser* parser)
	{
		throw exceptions::not_implemented_feature_exception("Parser pattern");
		return nullptr;
	}
} }