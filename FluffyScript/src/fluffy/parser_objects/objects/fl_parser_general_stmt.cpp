#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_object.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectGeneralStmt
	 */

	GeneralStmtPtr ParserObjectGeneralStmt::parse(Parser* parser)
	{
		Bool hasExport = false;

		// Verifica se houve a declararao para exportar o elemento.
		if (parser->isExport())
		{
			parser->expectToken([parser]() { return parser->isExport();  });
			hasExport = true;
		}

		// Verifica qual declaracao processar.
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::eTST_Class:
			throw exceptions::not_implemented_feature_exception("parseClassDecl");
		case TokenSubType_e::eTST_Interface:
			throw exceptions::not_implemented_feature_exception("parseInterfaceDecl");
		case TokenSubType_e::eTST_Struct:
			throw exceptions::not_implemented_feature_exception("parseStructDecl");
		case TokenSubType_e::eTST_Enum:
			throw exceptions::not_implemented_feature_exception("parseEnumDecl");
		case TokenSubType_e::eTST_Trait:
			throw exceptions::not_implemented_feature_exception("parseTraitDecl");
		case TokenSubType_e::eTST_Let:
			throw exceptions::not_implemented_feature_exception("parseVariableDecl");
		case TokenSubType_e::eTST_Fn:
			throw exceptions::not_implemented_feature_exception("parseFunctionDecl");
		default:
			throw exceptions::unexpected_token_exception(parser->getTokenValue(), parser->getTokenLine(), parser->getTokenColumn());
		}

		return nullptr;
	}
} }