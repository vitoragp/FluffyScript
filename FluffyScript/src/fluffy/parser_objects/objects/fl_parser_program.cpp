#include "../../fl_ast.h"
#include "../fl_parser_object.h"

namespace fluffy { namespace parser_objects {
	/**
		* ParserObjectProgram
		*/

	ProgramPtr ParserObjectProgram::parse(Parser* parser)
	{
		auto program = std::make_unique<ast::Program>();

		// Faz a analise do primeiro token.
		parser->nextToken();

		if (parser->isEof()) {
			return program;
		}

		// Processa declaracoes de include.
		while (true)
		{
			if (parser->isEof()) {
				return program;
			}
			if (parser->isInclude()) {
				program->includeList.push_back(ParserObjectInclude::parse(parser));
				continue;
			}
			break;
		}

		while (true)
		{
			if (parser->isEof()) {
				return program;
			}

			// Processa namespaces.
			if (parser->isNamespace()) {
				program->namespaceList.push_back(ParserObjectNamespace::parse(parser));
			}
		}
		return nullptr;
	}
} }