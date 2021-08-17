#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectIncludeDecl
	 */

	IncludeDeclPtr ParserObjectIncludeDecl::parse(Parser* parser)
	{
		auto includeDecl = std::make_unique<ast::IncludeDecl>();

		// Consome 'include'.
		parser->expectToken([parser]() { return parser->isInclude(); });

		// Consome '{'
		parser->expectToken([parser]() { return parser->isLeftBracket(); });

		// Consome Identificadores.
		while (true)
		{
			if (parser->isMultiplication()) {
				// Se existe identificaores declarados, o caractere coringa
				// nao pode ser usado.
				if (includeDecl->includedItemList.size()) {
					throw exceptions::unexpected_token_exception(parser->getTokenValue(), parser->getTokenLine(), parser->getTokenColumn());
				}

				// Consome '*'
				parser->expectToken([parser]() { return parser->isMultiplication(); });
				break;
			}

			// Consome identificador.
			includeDecl->includedItemList.push_back(parser->expectIdentifier());

			// Verifica se ha mais declaracoes.
			if (!parser->isComma()) {
				break;
			}

			// Consome ','
			parser->expectToken([parser]() { return parser->isComma(); });
		}

		// Consome '}'
		parser->expectToken([parser]() { return parser->isRightBracket(); });

		// Consome 'from'
		parser->expectToken([parser]() { return parser->isFrom(); });

		// Consome o identificador do namespace.
		includeDecl->fromNamespace = ParserObjectScopedIdentifier::parse(parser);

		// Consome ';'
		parser->expectToken([parser]() { return parser->isSemiColon(); });

		return includeDecl;
	}
} }