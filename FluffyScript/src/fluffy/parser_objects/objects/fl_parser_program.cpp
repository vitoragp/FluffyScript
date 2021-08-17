#include "../../fl_ast_decl.h"
#include "../fl_parser_objects.h"

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
				program->includeDeclList.push_back(ParserObjectIncludeDecl::parse(parser));
				continue;
			}
			break;
		}

		auto codeUnit = std::make_unique<ast::CodeUnit>(parser->getFilename());

		while (true)
		{
			if (parser->isEof()) {
				program->codeUnits.push_back(std::move(codeUnit));
				return program;
			}
						
			// Processa namespaces.
			if (parser->isNamespace()) {
				codeUnit->namespaceDeclList.push_back(ParserObjectNamespace::parse(parser));
			}
		}
		return nullptr;
	}
} }