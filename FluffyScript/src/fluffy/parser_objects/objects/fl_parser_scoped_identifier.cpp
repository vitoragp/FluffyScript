#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectScopedIdentifier
	 */

	ScopedIdentifierDeclPtr	ParserObjectScopedIdentifier::parse(Parser* parser)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>();

		// Se o escopo vem antes identificador
		if (parser->isScopeResolution())
		{
			// Consome '::'.
			parser->expectToken([parser]() { return parser->isScopeResolution(); });
			scopedIdentifierDecl->startFromRoot = true;
		}

		// Consome o identificador.
		scopedIdentifierDecl->identifiers.push_back(parser->expectIdentifier());

		while (true)
		{
			// Verifica se a resolucao de escopo.
			if (parser->isScopeResolution())
			{
				// Consome '::'.
				parser->expectToken([parser]() { return parser->isScopeResolution(); });
				scopedIdentifierDecl->identifiers.push_back(parser->expectIdentifier());
				continue;
			}
			break;
		}
		return scopedIdentifierDecl;
	}
} }