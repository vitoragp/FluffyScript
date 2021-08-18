#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectInterfaceDecl
	 */

	InterfaceDeclPtr ParserObjectInterfaceDecl::parse(Parser* parser, Bool hasExport)
	{
		auto interfaceDecl = std::make_unique<ast::InterfaceDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		interfaceDecl->isExported = hasExport;

		// Consome 'interface'.
		parser->expectToken(TokenSubType_e::Interface);

		// Consome o identificador.
		interfaceDecl->identifier = parser->expectIdentifier();

		// Consome generic se houver.
		if (parser->isLessThan())
		{
			interfaceDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome '{'.
		parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

		parseFunctionLabel:
			if (parser->isFn())
			{				
				// Consome a funcao.
				interfaceDecl->functionDeclList.push_back(parserFunction(parser));
				goto parseFunctionLabel;
			}

			if (parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{ TokenSubType_e::Fn, TokenSubType_e::RBracket },
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '}'.
		parser->expectToken(TokenSubType_e::RBracket);

		return interfaceDecl;
	}

	InterfaceFunctionDeclPtr ParserObjectInterfaceDecl::parserFunction(Parser* parser)
	{
		auto interfaceFunctionDecl = std::make_unique<ast::InterfaceFunctionDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'fn'
		parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		interfaceFunctionDecl->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			interfaceFunctionDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome os parametros.
		interfaceFunctionDecl->parameterList = ParserObjectFunctionParameter::parse(parser);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			interfaceFunctionDecl->returnType = ParserObjectTypeDecl::parse(parser);
		}
		else
		{
			// Consome o tipo retorno.
			interfaceFunctionDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome ';'
		parser->expectToken(TokenSubType_e::SemiColon);

		return interfaceFunctionDecl;
	}
} }