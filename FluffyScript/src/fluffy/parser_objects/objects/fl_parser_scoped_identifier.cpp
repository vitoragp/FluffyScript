#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_object.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectScopedIdentifier
	 */

	ScopedIdentifierDeclPtr	ParserObjectScopedIdentifier::parse(Parser* parser)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>();

		// Consome o identificador.
		scopedIdentifierDecl->identifier = parser->expectIdentifier();

		while (true)
		{
			// Verifica se a resolucao de escopo.
			if (!parser->isScopeResolution())
			{
				break;
			}

			// Consome '::'.
			parser->expectToken([parser]() { return parser->isScopeResolution(); });

			// Consome o tailDecl.
			scopedIdentifierDecl->tailDecl = ParserObjectScopedIdentifier::parse(parser);
		}
		return scopedIdentifierDecl;
	}
} }