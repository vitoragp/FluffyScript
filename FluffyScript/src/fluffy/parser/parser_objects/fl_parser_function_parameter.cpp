#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectFunctionParameter
	 */

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parse(Parser* parser)
	{
		return parseParameters(parser, false);
	}

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parseWithSelf(Parser* parser)
	{
		return parseParameters(parser, true);
	}

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parseParameters(Parser* parser, Bool traitMode)
	{
		FunctionParameterDeclPtrList parameterDeclList;

		// Consome '('
		parser->expectToken(TokenSubType_e::LParBracket);

		// Finaliza declaracao da lista de parametros.
		if (parser->isRightParBracket())
		{
			// Consome ')'
			parser->expectToken(TokenSubType_e::RParBracket);
			return parameterDeclList;
		}

		while (true)
		{
			auto parameterDecl = std::make_unique<ast::FunctionParameterDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();

			parameterDecl->identifier = parser->expectIdentifier();

			// Consome ':'
			parser->expectToken(TokenSubType_e::Colon);

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
					line,
					column,
					parameterDecl->identifier.str()
				);
			}

			// Adiciona o parametro a lista.
			parameterDeclList.push_back(std::move(parameterDecl));

			// Consome ','
			if (parser->isComma())
			{
				parser->expectToken(TokenSubType_e::Comma);
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
		parser->expectToken(TokenSubType_e::RParBracket);

		return parameterDeclList;
	}
} }