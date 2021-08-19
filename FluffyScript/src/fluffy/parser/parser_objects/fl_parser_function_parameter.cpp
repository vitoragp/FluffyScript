#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectFunctionParameter
	 */

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parse(CompilationContext_t* ctx)
	{
		return parseParameters(ctx, false);
	}

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parseWithSelf(CompilationContext_t* ctx)
	{
		return parseParameters(ctx, true);
	}

	FunctionParameterDeclPtrList ParserObjectFunctionParameter::parseParameters(CompilationContext_t* ctx, Bool traitMode)
	{
		FunctionParameterDeclPtrList parameterDeclList;

		// Consome '('
		ctx->parser->expectToken(TokenSubType_e::LParBracket);

		// Finaliza declaracao da lista de parametros.
		if (ctx->parser->isRightParBracket())
		{
			// Consome ')'
			ctx->parser->expectToken(TokenSubType_e::RParBracket);
			return parameterDeclList;
		}

		while (true)
		{
			auto parameterDecl = std::make_unique<ast::FunctionParameterDecl>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);

			const U32 line = ctx->parser->getTokenLine();
			const U32 column = ctx->parser->getTokenColumn();

			parameterDecl->identifier = ctx->parser->expectIdentifier();

			// Consome ':'
			ctx->parser->expectToken(TokenSubType_e::Colon);

			// Consome tipo
			if (traitMode)
			{
				parameterDecl->typeDecl = ParserObjectTypeDecl::parseWithSelf(ctx);
			}
			else
			{
				parameterDecl->typeDecl = ParserObjectTypeDecl::parse(ctx);
			}

			// Parametros nao podem ser do tipo void.
			if (parameterDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Parameter '%s' can't have void type",
					line,
					column,
					parameterDecl->identifier.c_str()
				);
			}

			// Adiciona o parametro a lista.
			parameterDeclList.push_back(std::move(parameterDecl));

			// Consome ','
			if (ctx->parser->isComma())
			{
				ctx->parser->expectToken(TokenSubType_e::Comma);
				continue;
			}

			// Finaliza declaracao da lista de parametros.
			if (ctx->parser->isRightParBracket())
			{
				break;
			}

			throw exceptions::custom_exception("Expected ',' or ')' token",
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome ')'
		ctx->parser->expectToken(TokenSubType_e::RParBracket);

		return parameterDeclList;
	}
} }