#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectClassExtendsDecl
	 */

	TypeDeclPtr ParserObjectClassExtendsDecl::parse(Parser* parser)
	{
		// Consome 'extends'
		parser->expectToken([parser]() { return parser->isExtends(); });
		return ParserObjectTypeDecl::parse(parser);
	}
} }