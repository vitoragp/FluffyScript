#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectStructDecl
	 */

	StructDeclPtr ParserObjectStructDecl::parse(Parser* parser, Bool hasExport)
	{
		auto structDecl = std::make_unique<ast::StructDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		structDecl->isExported = hasExport;

		// Consome 'struct'.
		parser->expectToken(TokenSubType_e::Struct);

		// Consome o identificador.
		structDecl->identifier = parser->expectIdentifier();

		// Consome generic se houver.
		if (parser->isLessThan())
		{
			structDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome '{'.
		parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

		parseVariableLabel:
			if (parser->isLet() || parser->isConst())
			{
				// Consome a funcao.
				structDecl->variableList.push_back(parserVariable(parser));				
				goto parseVariableLabel;
			}

			if (parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{ TokenSubType_e::Let, TokenSubType_e::Const, TokenSubType_e::RBracket },
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '}'.
		parser->expectToken(TokenSubType_e::RBracket);

		return structDecl;
	}

	StructVariableDeclPtr ParserObjectStructDecl::parserVariable(Parser* parser)
	{
		auto structVariableDecl = std::make_unique<ast::StructVariableDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Let:
			parser->expectToken(TokenSubType_e::Let);
			structVariableDecl->isConst = false;
			break;
		case TokenSubType_e::Const:
			parser->expectToken(TokenSubType_e::Const);
			structVariableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{ TokenSubType_e::Let, TokenSubType_e::Const },
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Verifica se possui o identificador 'ref'.
		if (parser->isRef())
		{
			// Consome 'ref'.
			parser->expectToken(TokenSubType_e::Ref);
			structVariableDecl->isReference = true;
		}

		// Consome identificador.
		structVariableDecl->identifier = parser->expectIdentifier();

		Bool mustHaveInitExpression = structVariableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (parser->isColon())
		{
			// Consome ':'.
			parser->expectToken(TokenSubType_e::Colon);

			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();

			// Consome o tipo.
			structVariableDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

			// Verifica se o tipo e valido.
			if (structVariableDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Variables or constant can't have void type",
					line,
					column
				);
			}
		} else {
			// Se o tipo nao e declarado explicitamente a variavel deve ser iniciado.
			mustHaveInitExpression = true;
		}

		// Consome a expressao de inicializacao.
		if (mustHaveInitExpression)
		{
			// Consome '='.
			parser->expectToken(TokenSubType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			structVariableDecl->initExpression = ParserObjectExpressionDecl::skip(parser);
		}

		// Toda declaracao de variavel ou constante deve terminar com ';'
		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return structVariableDecl;
	}
} }