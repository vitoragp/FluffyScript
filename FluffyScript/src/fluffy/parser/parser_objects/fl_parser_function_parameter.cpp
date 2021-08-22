#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectFunctionParameter
	 */

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parse(Parser* parser, Bool skipOnly)
	{
		return parseParameters(parser, false, skipOnly);
	}

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parseWithSelf(Parser* parser, Bool skipOnly)
	{
		return parseParameters(parser, true, skipOnly);
	}

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parseParameters(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		FunctionParameterDeclPtrList parameterDeclList;

		// Consome '('
		parser->expectToken(TokenType_e::LParBracket);

		// Finaliza declaracao da lista de parametros.
		if (parser->isRightParBracket())
		{
			// Consome ')'
			parser->expectToken(TokenType_e::RParBracket);
			return parameterDeclList;
		}

		while (true)
		{
			Bool isReference = false;

			if (parser->isRef())
			{
				parser->expectToken(TokenType_e::Ref);
				isReference = true;
			}

			if (parser->isIdentifier())
			{
				auto parameterDecl = std::make_unique<ast::FunctionParameterDecl>(
					parser->getTokenLine(),
					parser->getTokenColumn()
				);

				parameterDecl->isReference = isReference;
				parameterDecl->identifier = parser->expectIdentifier();

				// Verifica se e uma ellipsis
				if (parser->isEllipsis())
				{
					parameterDecl->isEllipsis = true;
					parser->expectToken(TokenType_e::Ellipsis);
					break;
				}

				// Consome ':'
				parser->expectToken(TokenType_e::Colon);

				// Consome tipo
				if (traitMode)
				{
					parameterDecl->typeDecl = ParserObjectTypeDecl::parseWithSelf(parser);
				}
				else
				{
					parameterDecl->typeDecl = ParserObjectTypeDecl::parse(parser);
				}

				// Parametros nao podem ser do tipo void.
				if (parameterDecl->typeDecl->typeID == TypeDeclID_e::Void)
				{
					throw exceptions::custom_exception(
						"Parameter '%s' can't have void type",
						parameterDecl->typeDecl->line,
						parameterDecl->typeDecl->column,
						parameterDecl->identifier.str()
					);
				}

				// Adiciona o parametro a lista.
				parameterDeclList.push_back(std::move(parameterDecl));
			}
			else if (parser->isLeftBracket())
			{
				auto parameterDecl = std::make_unique<ast::FunctionParameterDecl>(
					parser->getTokenLine(),
					parser->getTokenColumn()
				);

				parameterDecl->isReference = isReference;

				// Consome destructuring.
				parameterDecl->destructuringPatternDecl = ParserObjectPatternDecl::parse(parser, skipOnly);

				// Consome ':'
				parser->expectToken(TokenType_e::Colon);

				// Consome tipo
				if (traitMode)
				{
					parameterDecl->typeDecl = ParserObjectTypeDecl::parseWithSelf(parser);
				}
				else
				{
					parameterDecl->typeDecl = ParserObjectTypeDecl::parse(parser);
				}

				// Parametros nao podem ser do tipo void.
				if (parameterDecl->typeDecl->typeID == TypeDeclID_e::Void)
				{
					throw exceptions::custom_exception(
						"Parameter can't have void type",
						parameterDecl->typeDecl->line,
						parameterDecl->typeDecl->column
					);
				}

				// Adiciona o parametro a lista.
				parameterDeclList.push_back(std::move(parameterDecl));
			}

			// Consome ','
			if (parser->isComma())
			{
				parser->expectToken(TokenType_e::Comma);
				continue;
			}

			// Finaliza declaracao da lista de parametros.
			if (parser->isRightParBracket())
			{
				break;
			}

			throw exceptions::custom_exception("Expected ',' or ')' token",
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome ')'
		parser->expectToken(TokenType_e::RParBracket);

		return parameterDeclList;
	}
} }