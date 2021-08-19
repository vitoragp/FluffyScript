#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectGenericTemplate
	 */

	GenericDeclPtrList ParserObjectGenericDecl::parse(CompilationContext_t* ctx)
	{
		GenericDeclPtrList templateDeclList;

		// Consome '<'
		ctx->parser->expectToken(TokenSubType_e::LessThan);

		while (true)
		{
			auto genericDecl = std::make_unique<ast::GenericDecl>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);

			// Consome o identificador.
			genericDecl->identifier = ctx->parser->expectIdentifier();

			// Processa a clausula where.
			if (ctx->parser->isColon())
			{
				// Consome ':'
				ctx->parser->expectToken(TokenSubType_e::Colon);

				// Consome 'where'
				ctx->parser->expectToken(TokenSubType_e::Where);

				const U32 line = ctx->parser->getTokenLine();
				const U32 column = ctx->parser->getTokenColumn();

				// Consome o identificador
				auto identifier = ctx->parser->expectIdentifier();

				// Verifica se a declaracao esta correta, o identificador
				// apos o where deve ser igual o identificador declarado no generic.
				if (genericDecl->identifier != identifier)
				{
					throw exceptions::custom_exception(
						"Where identifier must be like generic identifier: %s",
						line,
						column,
						genericDecl->identifier.c_str()
					);
				}

				// Consome 'is'
				ctx->parser->expectToken(TokenSubType_e::Is);

				while (true)
				{
					auto whereTypeDecl = ParserObjectTypeDecl::parse(ctx);

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
					if (ctx->parser->isBitWiseOr())
					{
						// Consome '|'
						ctx->parser->expectToken(TokenSubType_e::BitWiseOr);
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
			if (ctx->parser->isComma())
			{
				ctx->parser->expectToken(TokenSubType_e::Comma);
				continue;
			}

			// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
			// serao quebrados em tokens menores.
			if (ctx->parser->isBitWiseRightShift() || ctx->parser->isBitWiseRightShiftAssign())
			{
				switch (ctx->parser->getTokenSubType())
				{
				case TokenSubType_e::BitWiseRShift:		// >>
				case TokenSubType_e::GreaterThanOrEqual:	// >=
				case TokenSubType_e::BitWiseRShiftAssign:	// >>=
					ctx->parser->reinterpretToken(
						TokenType_e::Keyword,
						TokenSubType_e::GreaterThan,
						1
					);
					break;
				default:
					break;
				}
			}

			// Consome '>'
			ctx->parser->expectToken(TokenSubType_e::GreaterThan);
			break;
		}
		return templateDeclList;
	}
} }