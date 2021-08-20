#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectTraitDecl
	 */

	TraitDeclPtr ParserObjectTraitDecl::parse(Parser* parser, Bool hasExport)
	{
		auto traitDecl = std::make_unique<ast::TraitDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		traitDecl->isExported = hasExport;

		// Consome 'interface'.
		parser->expectToken(TokenSubType_e::Interface);

		// Consome o identificador.
		traitDecl->identifier = parser->expectIdentifier();

		// Consome generic se houver.
		if (parser->isLessThan())
		{
			traitDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Verifica se e um definicao de trait.
		if (parser->isFor())
		{
			traitDecl->isDefinition = true;

			// Consome 'for'.
			parser->expectToken(TokenSubType_e::For);

			// Consome o tipo para qual o trait esta sendo implementado.
			traitDecl->typeDefinitionDecl = ParserObjectTypeDecl::parseWithSelf(parser);
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
				traitDecl->functionDeclList.push_back(parserFunction(parser));
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

		return traitDecl;
	} 

	TraitFunctionDeclPtr ParserObjectTraitDecl::parserFunction(Parser* parser)
	{
		auto traitFunctionDecl = std::make_unique<ast::TraitFunctionDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'fn'
		parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		traitFunctionDecl->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			traitFunctionDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome os parametros.
		traitFunctionDecl->parameterList = ParserObjectFunctionParameter::parseWithSelf(parser);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			traitFunctionDecl->returnType = ParserObjectTypeDecl::parseWithSelf(parser);
		}
		else
		{
			// Consome o tipo retorno.
			traitFunctionDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome ';'
		parser->expectToken(TokenSubType_e::SemiColon);

		return traitFunctionDecl;
	}
} }