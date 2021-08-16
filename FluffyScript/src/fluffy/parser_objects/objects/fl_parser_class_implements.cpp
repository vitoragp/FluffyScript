#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectClassImplementsDecl
	 */

	TypeDeclPtrList ParserObjectClassImplementsDecl::parse(Parser* parser)
	{
		TypeDeclPtrList interfaceList;

		// Consome 'implements'
		parser->expectToken([parser]() { return parser->isImplements(); });

		while (true)
		{
			// Consome o identificador com escopo.
			interfaceList.push_back(ParserObjectTypeDecl::parse(parser));

			if (parser->isComma())
			{
				// Consome ','
				parser->expectToken([parser]() { return parser->isComma(); });
				continue;
			}
			break;
		}
		return interfaceList;
	}
} }