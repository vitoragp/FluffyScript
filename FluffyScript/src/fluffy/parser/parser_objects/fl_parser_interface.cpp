#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectInterfaceDecl
	 */

	InterfaceDeclPtr ParserObjectInterfaceDecl::parse(CompilationContext_t* ctx, Bool hasExport)
	{
		auto interfaceDecl = std::make_unique<ast::InterfaceDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		interfaceDecl->isExported = hasExport;

		// Consome 'interface'.
		ctx->parser->expectToken(TokenSubType_e::Interface);

		// Consome o identificador.
		interfaceDecl->identifier = ctx->parser->expectIdentifier();

		// Consome generic se houver.
		if (ctx->parser->isLessThan())
		{
			interfaceDecl->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome '{'.
		ctx->parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (ctx->parser->isRightBracket())
			{
				break;
			}

		parseFunctionLabel:
			if (ctx->parser->isFn())
			{				
				// Consome a funcao.
				interfaceDecl->functionDeclList.push_back(parserFunction(ctx));
				goto parseFunctionLabel;
			}

			if (ctx->parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				ctx->parser->getTokenValue(),
				{ TokenSubType_e::Fn, TokenSubType_e::RBracket },
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome '}'.
		ctx->parser->expectToken(TokenSubType_e::RBracket);

		return interfaceDecl;
	}

	InterfaceFunctionDeclPtr ParserObjectInterfaceDecl::parserFunction(CompilationContext_t* ctx)
	{
		auto interfaceFunctionDecl = std::make_unique<ast::InterfaceFunctionDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'fn'
		ctx->parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		interfaceFunctionDecl->identifier = ctx->parser->expectIdentifier();

		// Consome o Generic.
		if (ctx->parser->isLessThan())
		{
			interfaceFunctionDecl->genericDeclList = ParserObjectGenericDecl::parse(ctx);
		}

		// Consome os parametros.
		interfaceFunctionDecl->parameterList = ParserObjectFunctionParameter::parse(ctx);

		// Consome o retorno se houver.
		if (ctx->parser->isArrow())
		{
			// Consome '->'
			ctx->parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			interfaceFunctionDecl->returnType = ParserObjectTypeDecl::parse(ctx);
		}
		else
		{
			// Consome o tipo retorno.
			interfaceFunctionDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome ';'
		ctx->parser->expectToken(TokenSubType_e::SemiColon);

		return interfaceFunctionDecl;
	}
} }