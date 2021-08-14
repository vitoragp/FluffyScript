#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_object.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectNamespace
	 */

	NamespacePtr ParserObjectNamespace::parse(Parser* parser)
	{
		auto namespaceDecl = std::make_unique<ast::Namespace>();

		// Consome 'namespace'.
		parser->expectToken([parser]() { return parser->isNamespace(); });

		// Consome o identficador com o nome do namespace.
		namespaceDecl->name = parser->expectIdentifier();

		// Consome '{'.
		parser->expectToken([parser]() { return parser->isLeftBracket(); });

		while (true)
		{
			// Verifica se chegou ao fim do namespace.
			if (parser->isRightBracket())
			{
				break;
			}

			// Processa namespace.
			if (parser->isNamespace())
			{
				namespaceDecl->namespaceList.push_back(ParserObjectNamespace::parse(parser));
				continue;
			}

			// Processa declaracao geral.
			namespaceDecl->generalDeclList.push_back(ParserObjectGeneralStmt::parse(parser));
		}

		// Consome '}'.
		parser->expectToken([parser]() { return parser->isRightBracket(); });

		return namespaceDecl;
	}
} }