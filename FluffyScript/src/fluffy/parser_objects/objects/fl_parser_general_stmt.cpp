#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectGeneralStmt
	 */

	GeneralStmtPtr ParserObjectGeneralStmt::parse(Parser* parser)
	{
		Bool hasExport = false;
		Bool hasAbtract = false;

		// Verifica se houve a declararao para exportar o elemento.
		if (parser->isExport())
		{
			// Consome 'export'.
			parser->expectToken([parser]() { return parser->isExport();  });
			hasExport = true;
		}

		// Verifica se a declaracao para a classe ser abstrata.
		if (parser->isAbstract())
		{
			// Consome 'abtract'.
			parser->expectToken([parser]() { return parser->isAbstract();  });
			hasAbtract = true;

			// Obrigatoriamente 'abstract' deve se referir a uma classe.
			if (!parser->isClass()) {
				throw exceptions::unexpected_token_exception(parser->getTokenValue(), parser->getTokenLine(), parser->getTokenColumn());
			}
		}

		// Verifica qual declaracao processar.
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Class:
			return ParserObjectClassDecl::parse(parser, hasExport, hasAbtract);
		case TokenSubType_e::Interface:
			throw exceptions::not_implemented_feature_exception("parseInterfaceDecl");
		case TokenSubType_e::Struct:
			throw exceptions::not_implemented_feature_exception("parseStructDecl");
		case TokenSubType_e::Enum:
			throw exceptions::not_implemented_feature_exception("parseEnumDecl");
		case TokenSubType_e::Trait:
			throw exceptions::not_implemented_feature_exception("parseTraitDecl");
		case TokenSubType_e::Let:
			throw exceptions::not_implemented_feature_exception("parseVariableDecl");
		case TokenSubType_e::Fn:
			throw exceptions::not_implemented_feature_exception("parseFunctionDecl");
		default:
			throw exceptions::unexpected_token_exception(parser->getTokenValue(), parser->getTokenLine(), parser->getTokenColumn());
		}

		return nullptr;
	}
} }