#include "..\fl_ast_expr.h"
#include "..\fl_ast_pattern.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectPatternDecl
	 */
	
	PatternDeclPtr ParserObjectPatternDecl::parse(Parser* parser, Bool skipOnly)
	{
		return parsePattern(parser, skipOnly);
	}

	PatternDeclPtr ParserObjectPatternDecl::parsePattern(Parser* parser, Bool skipOnly)
	{
		switch (parser->getTokenType())
		{
		case TokenType_e::ConstantInteger:
			return parseLiteralPattern(parser, skipOnly);
		case TokenType_e::Identifier:
			{
				U32 position = parser->getTokenPosition();

				// Verifica se pode ser um enum destructuring.
				while (true)
				{
					if (parser->isScopeResolution())
					{
						parser->nextToken();
						continue;
					}

					if (parser->isIdentifier())
					{
						parser->nextToken();
						continue;
					}

					if (parser->isLeftParBracket())
					{
						parser->resetToPosition(position);
						return parseEnumPattern(parser, skipOnly);
					}
					parser->resetToPosition(position);
					break;
				}
				return parseLiteralPattern(parser, skipOnly);
			}
			break;
		case TokenType_e::LParBracket:
			return parseTuplePattern(parser, skipOnly);
		case TokenType_e::LBracket:
			return parseClassOrStructPattern(parser, skipOnly);
		default:
			throw exceptions::not_implemented_feature_exception("pattern matching");
		}
		return nullptr;
	}

	PatternDeclPtr ParserObjectPatternDecl::parseLiteralPattern(Parser* parser, Bool skipOnly)
	{
		auto literalPatternDecl = std::make_unique<ast::pattern::LiteralPatternDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome uma expressao.
		literalPatternDecl->patternExpr = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::Max, skipOnly);

		return literalPatternDecl;
	}

	PatternDeclPtr ParserObjectPatternDecl::parseEnumPattern(Parser* parser, Bool skipOnly)
	{
		auto enumPatternDecl = std::make_unique<ast::pattern::DestructuringPatternDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome expressao.
		enumPatternDecl->enumExpr = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::Max, skipOnly);

		// Consome '('
		parser->expectToken(TokenType_e::LParBracket);

		while (true)
		{
			auto destructuringItem = std::make_unique<ast::pattern::DestructuringItemDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Consome o identificador.
			destructuringItem->identifier = parser->expectIdentifier();

			// Adiciona item a lista.
			enumPatternDecl->destructuringItemDeclList.push_back(std::move(destructuringItem));

			if (parser->isComma())
			{
				// Consome ','
				parser->expectToken(TokenType_e::Comma);
				continue;
			}

			if (parser->isRightParBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenType_e::RParBracket,
					TokenType_e::Identifier
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome ')'
		parser->expectToken(TokenType_e::RParBracket);

		return enumPatternDecl;
	}

	PatternDeclPtr ParserObjectPatternDecl::parseTuplePattern(Parser* parser, Bool skipOnly)
	{
		return nullptr;
	}

	PatternDeclPtr ParserObjectPatternDecl::parseClassOrStructPattern(Parser* parser, Bool skipOnly)
	{
		auto destructuringDecl = std::make_unique<ast::pattern::DestructuringPatternDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Atribui o tipo de destructuring pattern.
		destructuringDecl->destructuringType = DestructuringType_e::ClassOrStruct;

		// Consome '{'
		parser->expectToken(TokenType_e::LBracket);

		while (true) {
			if (parser->isRightBracket())
			{
				break;
			}

			auto destructuringItem = std::make_unique<ast::pattern::DestructuringItemDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Consome o identificador.
			destructuringItem->identifier = parser->expectIdentifier();

			// Verifica se e uma referenciacao.
			if (parser->isColon())
			{
				// Consome ':'
				parser->expectToken(TokenType_e::Colon);

				// Verifica se faz uma referencia indireta ou a
				// um sub destructuring.
				if (parser->isLeftBracket() || parser->isLeftParBracket())
				{
					destructuringItem->destructuringItemType = DestructuringItemType_e::SubDestructuring;
					destructuringItem->subDestructuringPattern = parsePattern(parser, skipOnly);
				}
				else
				{
					destructuringItem->destructuringItemType = DestructuringItemType_e::IndirectReferenceOrMatch;
					destructuringItem->indirectRefOrMatchingDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::Max, skipOnly);
				}
			}
			else
			{
				destructuringItem->destructuringItemType = DestructuringItemType_e::DirectReference;
			}

			// Adiciona item a lista.
			destructuringDecl->destructuringItemDeclList.push_back(std::move(destructuringItem));

			// Consome ','.
			if (parser->isComma()) {
				parser->expectToken(TokenType_e::Comma);
				continue;
			}

			if (parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenType_e::Comma,
					TokenType_e::RBracket
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '}'
		parser->expectToken(TokenType_e::RBracket);

		return destructuringDecl;

		return nullptr;
	}
} }