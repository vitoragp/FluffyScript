#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectEnumDecl
	 */

	EnumDeclPtr ParserObjectEnumDecl::parse(Parser* parser, Bool hasExport)
	{
		auto enumDecl = std::make_unique<ast::EnumDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		enumDecl->isExported = hasExport;

		// Consome 'enum'.
		parser->expectToken(TokenType_e::Enum);

		// Consome o identificador.
		enumDecl->identifier = parser->expectIdentifier();

		// Consome generic se houver.
		if (parser->isLessThan())
		{
			enumDecl->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome '{'.
		parser->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

		parseEnumLabel:
			// Consome enum.
			enumDecl->enumItemDeclList.push_back(parserEnum(parser));

			if (parser->isComma())
			{				
				// Consome ','.
				parser->expectToken(TokenType_e::Comma);
				goto parseEnumLabel;
			}

			if (parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_token_exception(
				parser->getTokenValue(),
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '}'.
		parser->expectToken(TokenType_e::RBracket);

		return enumDecl;
	}

	EnumItemDeclPtr ParserObjectEnumDecl::parserEnum(Parser* parser)
	{
		auto enumItemDecl = std::make_unique<ast::EnumItemDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome o identificador.
		enumItemDecl->identifier = parser->expectIdentifier();

		switch (parser->getTokenType())
		{
		case TokenType_e::Assign:
			{
				// Consome '='.
				parser->expectToken(TokenType_e::Assign);
				enumItemDecl->hasValue = true;
				enumItemDecl->valueExpression = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::EnumExpr, true);
			}
			break;
		case TokenType_e::LParBracket:
			{
				// Consome '('.
				parser->expectToken(TokenType_e::LParBracket);

				// Indica presenca de dados.
				enumItemDecl->hasData = true;

				// Processa tipos.
				while (true)
				{
					// Consome Tipo
					enumItemDecl->dataTypeDeclList.push_back(ParserObjectTypeDecl::parse(parser));

					if (parser->isComma())
					{
						// Consome ','.
						parser->expectToken(TokenType_e::Comma);
						continue;
					}

					if (parser->isRightParBracket())
					{
						break;
					}

					throw exceptions::unexpected_with_possibilities_token_exception(
						parser->getTokenValue(),
						{ TokenType_e::Comma, TokenType_e::RParBracket },
						parser->getTokenLine(),
						parser->getTokenColumn()
					);
				}				

				// Consome ')'.
				parser->expectToken(TokenType_e::RParBracket);
			}
			break;
		case TokenType_e::Comma:
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{ TokenType_e::Assign, TokenType_e::LParBracket, TokenType_e::Comma },
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		return enumItemDecl;
	}
} }