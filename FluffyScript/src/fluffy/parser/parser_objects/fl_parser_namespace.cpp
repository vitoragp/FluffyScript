#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectNamespace
	 */

	NamespaceDeclPtr ParserObjectNamespace::parse(Parser* parser)
	{
		auto namespaceDecl = std::make_unique<ast::NamespaceDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'namespace'.
		parser->expectToken(TokenSubType_e::Namespace);

		// Consome o identficador com o nome do namespace.
		namespaceDecl->identifier = parser->expectIdentifier();

		// Consome '{'.
		parser->expectToken(TokenSubType_e::LBracket);

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
				namespaceDecl->namespaceDeclList.push_back(ParserObjectNamespace::parse(parser));
				continue;
			}

			// Processa declaracao geral.
			namespaceDecl->generalDeclList.push_back(ParserObjectGeneralDecl::parse(parser));
		}

		// Consome '}'.
		parser->expectToken(TokenSubType_e::RBracket);

		return namespaceDecl;
	}
} }