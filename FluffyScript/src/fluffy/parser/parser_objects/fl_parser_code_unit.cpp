#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	using lexer::Lexer;

	/**
	 * ParserObjectCodeUnit
	 */

	CodeUnitPtr ParserObjectCodeUnit::parse(Parser* parser)
	{
		auto codeUnit = std::make_unique<ast::CodeUnit>(
			parser->getFilename()
		);

		// Faz a analise do primeiro token.
		parser->nextToken();

		if (parser->isEof()) {
			return codeUnit;
		}

		// Processa declaracoes de include.
		while (true)
		{
			if (parser->isEof()) {
				return codeUnit;
			}
			if (parser->isInclude()) {
				codeUnit->includeDeclList.push_back(ParserObjectIncludeDecl::parse(parser));
				continue;
			}
			break;
		}

		while (true)
		{
			if (parser->isEof()) {
				break;
			}

			// Processa namespaces.
			if (parser->isNamespace()) {
				codeUnit->namespaceDeclList.push_back(ParserObjectNamespace::parse(parser));
				continue;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenType_e::Namespace
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}
		return codeUnit;
	}
} }