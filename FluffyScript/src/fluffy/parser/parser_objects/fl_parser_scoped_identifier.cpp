#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectScopedIdentifier
	 */

	ScopedIdentifierDeclPtr	ParserObjectScopedIdentifier::parse(Parser* parser)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Se o escopo vem antes identificador
		if (parser->isScopeResolution())
		{
			// Consome '::'.
			parser->expectToken(TokenSubType_e::ScopeResolution);
			scopedIdentifierDecl->startFromRoot = true;
		}

		// Consome o identificador.
		scopedIdentifierDecl->identifier = parser->expectIdentifier();

		// Verifica se a mais declaracoes de escopo.
		if (parser->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = ParserObjectScopedIdentifier::parseChildScopedIdentifiers(parser);
		}
		return scopedIdentifierDecl;
	}

	ScopedIdentifierDeclPtr ParserObjectScopedIdentifier::parseChildScopedIdentifiers(Parser* parser)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome '::'.
		parser->expectToken(TokenSubType_e::ScopeResolution);

		// Consome o identificador.
		scopedIdentifierDecl->identifier = parser->expectIdentifier();

		// Verifica se a mais declaracoes de escopo.
		if (parser->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = ParserObjectScopedIdentifier::parseChildScopedIdentifiers(parser);
		}
		return scopedIdentifierDecl;
	}
} }