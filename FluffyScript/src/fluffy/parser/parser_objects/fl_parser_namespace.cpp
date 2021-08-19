#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectNamespace
	 */

	NamespaceDeclPtr ParserObjectNamespace::parse(CompilationContext_t* ctx)
	{
		auto namespaceDecl = std::make_unique<ast::NamespaceDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'namespace'.
		ctx->parser->expectToken(TokenSubType_e::Namespace);

		// Consome o identficador com o nome do namespace.
		namespaceDecl->identifier = ctx->parser->expectIdentifier();

		// Consome '{'.
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			// Verifica se chegou ao fim do namespace.
			if (ctx->parser->isRightBracket())
			{
				break;
			}

			// Processa namespace.
			if (ctx->parser->isNamespace())
			{
				namespaceDecl->namespaceDeclList.push_back(ParserObjectNamespace::parse(ctx));
				continue;
			}

			// Processa declaracao geral.
			namespaceDecl->generalDeclList.push_back(ParserObjectGeneralDecl::parse(ctx));
		}

		// Consome '}'.
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return namespaceDecl;
	}
} }