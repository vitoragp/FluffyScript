#include "../../fl_ast_decl.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
		* ParserObjectProgram
		*/

	ProgramPtr ParserObjectProgram::parse(CompilationContext_t* ctx)
	{
		auto program = std::make_unique<ast::Program>();

		// Faz a analise do primeiro token.
		ctx->parser->nextToken();

		if (ctx->parser->isEof()) {
			return program;
		}

		// Processa declaracoes de include.
		while (true)
		{
			if (ctx->parser->isEof()) {
				return program;
			}
			if (ctx->parser->isInclude()) {
				program->includeDeclList.push_back(ParserObjectIncludeDecl::parse(ctx));
				continue;
			}
			break;
		}

		if (auto codeUnit = std::make_unique<ast::CodeUnit>(ctx->parser->getFilename()))
		{
			while (true)
			{
				if (ctx->parser->isEof()) {
					program->codeUnitList.push_back(std::move(codeUnit));
					break;
				}

				// Processa namespaces.
				if (ctx->parser->isNamespace()) {
					codeUnit->namespaceDeclList.push_back(ParserObjectNamespace::parse(ctx));
				}
			}
		}
		return program;
	}
} }