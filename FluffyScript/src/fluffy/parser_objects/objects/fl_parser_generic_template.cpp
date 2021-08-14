#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectGenericTemplate
	 */

	StringList ParserObjectGenericTemplateDecl::parse(Parser* parser)
	{
		StringList templateDeclList;

		// Consome '<'
		parser->expectToken([parser]() { return parser->isLessThan(); });

		while (true)
		{
			// Consome o identificador.
			templateDeclList.push_back(
				parser->expectIdentifier()
			);

			// Consome ',' e processa proxima declaracao.
			if (parser->isComma())
			{
				parser->expectToken([parser]() { return parser->isComma(); });
				continue;
			}

			// Consome '>'
			parser->expectToken([parser]() { return parser->isGreaterThan(); });
			break;
		}
		return templateDeclList;
	}
} }