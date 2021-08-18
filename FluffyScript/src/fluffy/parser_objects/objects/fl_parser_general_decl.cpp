#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectGeneralDecl
	 */

	GeneralStmtPtr ParserObjectGeneralDecl::parse(Parser* parser)
	{
		Bool hasExport = false;
		Bool hasAbtract = false;

		// Verifica se houve a declararao para exportar o elemento.
		if (parser->isExport())
		{
			// Consome 'export'.
			parser->expectToken(TokenSubType_e::Export);
			hasExport = true;
		}

		// Verifica se a declaracao para a classe ser abstrata.
		if (parser->isAbstract())
		{
			// Consome 'abtract'.
			parser->expectToken(TokenSubType_e::Abstract);
			hasAbtract = true;

			// Obrigatoriamente 'abstract' deve se referir a uma classe.
			if (!parser->isClass()) {
				throw exceptions::unexpected_token_exception(parser->getTokenValue(), parser->getTokenLine(), parser->getTokenColumn());
			}
		}

		// export, abstract, class, interface, struct, enum, trait, let, fn

		// Verifica qual declaracao processar.
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Class:
			return ParserObjectClassDecl::parse(parser, hasExport, hasAbtract);
		case TokenSubType_e::Interface:
			return ParserObjectInterfaceDecl::parse(parser, hasExport);
		case TokenSubType_e::Struct:
			return ParserObjectStructDecl::parse(parser, hasExport);
		case TokenSubType_e::Enum:
			throw exceptions::not_implemented_feature_exception("parseEnumDecl");
		case TokenSubType_e::Trait:
			throw exceptions::not_implemented_feature_exception("parseTraitDecl");
		case TokenSubType_e::Let:
			throw exceptions::not_implemented_feature_exception("parseVariableDecl");
		case TokenSubType_e::Fn:
			throw exceptions::not_implemented_feature_exception("parseFunctionDecl");
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenSubType_e::Class,
					TokenSubType_e::Interface,
					TokenSubType_e::Struct,
					TokenSubType_e::Enum,
					TokenSubType_e::Trait,
					TokenSubType_e::Let,
					TokenSubType_e::Fn
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		return nullptr;
	}
} }