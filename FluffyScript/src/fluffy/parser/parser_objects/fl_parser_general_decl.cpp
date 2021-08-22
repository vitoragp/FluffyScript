#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

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
			parser->expectToken(TokenType_e::Export);
			hasExport = true;
		}

		// Verifica se a declaracao para a classe ser abstrata.
		if (parser->isAbstract())
		{
			// Consome 'abtract'.
			parser->expectToken(TokenType_e::Abstract);
			hasAbtract = true;

			// Obrigatoriamente 'abstract' deve se referir a uma classe.
			if (!parser->isClass()) {
				throw exceptions::unexpected_token_exception(parser->getTokenValue(), parser->getTokenLine(), parser->getTokenColumn());
			}
		}

		// export, abstract, class, interface, struct, enum, trait, let, fn

		// Verifica qual declaracao processar.
		switch (parser->getTokenType())
		{
		case TokenType_e::Class:
			return ParserObjectClassDecl::parse(parser, hasExport, hasAbtract);
		case TokenType_e::Interface:
			return ParserObjectInterfaceDecl::parse(parser, hasExport);
		case TokenType_e::Struct:
			return ParserObjectStructDecl::parse(parser, hasExport);
		case TokenType_e::Enum:
			return ParserObjectEnumDecl::parse(parser, hasExport);
		case TokenType_e::Trait:
			return ParserObjectTraitDecl::parse(parser, hasExport);
		case TokenType_e::Fn:
			return ParserObjectFunctionDecl::parse(parser, hasExport);
		case TokenType_e::Const:
		case TokenType_e::Let:
			return ParserObjectVariableDecl::parse(parser, hasExport);
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenType_e::Class,
					TokenType_e::Interface,
					TokenType_e::Struct,
					TokenType_e::Enum,
					TokenType_e::Trait,
					TokenType_e::Let,
					TokenType_e::Const,
					TokenType_e::Fn
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		return nullptr;
	}
} }