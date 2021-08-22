#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectGenericTemplate
	 */

	GenericDeclPtrList ParserObjectGenericDecl::parse(Parser* parser)
	{
		GenericDeclPtrList templateDeclList;

		// Consome '<'
		parser->expectToken(TokenType_e::LessThan);

		while (true)
		{
			auto genericDecl = std::make_unique<ast::GenericDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Consome o identificador.
			genericDecl->identifier = parser->expectIdentifier();

			// Processa a clausula where.
			if (parser->isColon())
			{
				// Consome ':'
				parser->expectToken(TokenType_e::Colon);

				// Consome 'where'
				parser->expectToken(TokenType_e::Where);

				const U32 line = parser->getTokenLine();
				const U32 column = parser->getTokenColumn();

				// Consome o identificador
				auto identifier = parser->expectIdentifier();

				// Verifica se a declaracao esta correta, o identificador
				// apos o where deve ser igual o identificador declarado no generic.
				if (genericDecl->identifier != identifier)
				{
					throw exceptions::custom_exception(
						"Where identifier must be like generic identifier: %s",
						line,
						column,
						genericDecl->identifier.str()
					);
				}

				// Consome 'is'
				parser->expectToken(TokenType_e::Is);

				while (true)
				{
					auto whereTypeDecl = ParserObjectTypeDecl::parse(parser);

					if (whereTypeDecl->nullable)
					{
						throw exceptions::custom_exception(
							"Where types can't be nullable",
							line,
							column
						);
					}

					genericDecl->whereTypeList.push_back(std::move(whereTypeDecl));

					// Verifica se a outras declaracoes.
					if (parser->isBitWiseOr())
					{
						// Consome '|'
						parser->expectToken(TokenType_e::BitWiseOr);
						continue;
					}
					break;
				}
			}

			// Adiciona o generic a lista.
			templateDeclList.push_back(
				std::move(genericDecl)
			);

			// Consome ',' e processa proxima declaracao.
			if (parser->isComma())
			{
				parser->expectToken(TokenType_e::Comma);
				continue;
			}

			// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
			// serao quebrados em tokens menores.
			if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
			{
				switch (parser->getTokenType())
				{
				case TokenType_e::BitWiseRShift:		// >>
				case TokenType_e::GreaterThanOrEqual:	// >=
				case TokenType_e::BitWiseRShiftAssign:	// >>=
					parser->reinterpretToken(
						TokenType_e::GreaterThan,
						1
					);
					break;
				default:
					break;
				}
			}

			// Consome '>'
			parser->expectToken(TokenType_e::GreaterThan);
			break;
		}
		return templateDeclList;
	}
} }