#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectScopedIdentifier
	 */

	ScopedIdentifierDeclPtr	ParserObjectScopedIdentifier::parse(Parser* parser)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>();

		// Se o escopo vem antes identificador
		if (parser->isScopeResolution())
		{
			// Consome '::'.
			parser->expectToken([parser]() { return parser->isScopeResolution(); });
			scopedIdentifierDecl->startFromRoot = true;
		}

		// Consome o identificador.
		scopedIdentifierDecl->identifier = parser->expectIdentifier();

		// Verifica se o identificador possui declaracao de generic
		if (parser->isLessThan())
		{
			scopedIdentifierDecl->genericTemplateList = ParserObjectGenericTemplateDecl::parse(parser);
		}

		// Verifica se a mais declaracoes de escopo.
		if (parser->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = ParserObjectScopedIdentifier::parseChildScopedIdentifiers(parser);
		}
		return scopedIdentifierDecl;
	}

	ScopedIdentifierDeclPtr ParserObjectScopedIdentifier::parseChildScopedIdentifiers(Parser* parser)
	{
		auto scopedIdentifierDecl = std::make_unique<ast::ScopedIdentifierDecl>();

		// Consome '::'.
		parser->expectToken([parser]() { return parser->isScopeResolution(); });

		// Consome o identificador.
		scopedIdentifierDecl->identifier = parser->expectIdentifier();

		// Verifica se o identificador possui declaracao de generic
		if (parser->isLessThan())
		{
			scopedIdentifierDecl->genericTemplateList = ParserObjectGenericTemplateDecl::parse(parser);
		}

		// Verifica se a mais declaracoes de escopo.
		if (parser->isScopeResolution())
		{
			scopedIdentifierDecl->tailIdentifier = ParserObjectScopedIdentifier::parseChildScopedIdentifiers(parser);
		}
		return scopedIdentifierDecl;
	}
} }