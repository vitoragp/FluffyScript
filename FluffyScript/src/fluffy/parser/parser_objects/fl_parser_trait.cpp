#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectTraitDecl
	 */

	TraitDeclPtr ParserObjectTraitDecl::parse(Parser* parser, Bool hasExport)
	{
		Bool isDefinition = false;

		auto traitDecl = std::make_unique<ast::TraitDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		traitDecl->isExported = hasExport;

		// Consome 'trait'.
		parser->expectToken(TokenType_e::Trait);

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
			parser->expectToken(TokenType_e::For);

			// Consome o tipo para qual o trait esta sendo implementado.
			traitDecl->typeDefinitionDecl = ParserObjectTypeDecl::parseWithSelf(parser);

			// Indica que e uma definicao, permitindo assim que tenho um bloco de codigo.
			isDefinition = true;
		}

		// Consome '{'.
		parser->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

		parseFunctionLabel:
			Bool isStatic = false;

			// Consome 'static'.
			if (parser->isStatic())
			{
				parser->expectToken(TokenType_e::Static);
				isStatic = true;
			}

			if (parser->isFn())
			{
				// Consome a funcao.
				traitDecl->functionDeclList.push_back(parserFunction(parser, isDefinition, isStatic));
				goto parseFunctionLabel;
			}

			if (parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{ TokenType_e::Fn, TokenType_e::Static, TokenType_e::RBracket },
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '}'.
		parser->expectToken(TokenType_e::RBracket);

		return traitDecl;
	} 

	TraitFunctionDeclPtr ParserObjectTraitDecl::parserFunction(Parser* parser, Bool isDefinition, Bool isStatic)
	{
		auto traitFunctionDecl = std::make_unique<ast::TraitFunctionDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		traitFunctionDecl->isStatic = isStatic;

		// Consome 'fn'
		parser->expectToken(TokenType_e::Fn);

		// Consome o identificador.
		traitFunctionDecl->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			traitFunctionDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome os parametros.
		traitFunctionDecl->parameterList = ParserObjectFunctionParameter::parseWithSelf(parser, false);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken(TokenType_e::Arrow);

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

		// No modo de definicao e necessario implementar o bloco da funcao.
		if (isDefinition) {
			traitFunctionDecl->blockDecl = ParserObjectBlockDecl::parse(parser, false);
		} else {
			// Consome ';'
			parser->expectToken(TokenType_e::SemiColon);
		}
		return traitFunctionDecl;
	}
} }