#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectFunctionDecl
	 */

	FunctionDeclPtr ParserObjectFunctionDecl::parse(Parser* parser, Bool hasExport)
	{
		auto functionPtr = std::make_unique<ast::FunctionDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		functionPtr->isExported = hasExport;

		// Consome 'fn'
		parser->expectToken(TokenSubType_e::Fn);

		// Consome o identificador.
		functionPtr->identifier = parser->expectIdentifier();

		// Consome o Generic.
		if (parser->isLessThan())
		{
			functionPtr->genericDeclList = ParserObjectGenericDecl::parse(parser);
		}

		// Consome os parametros.
		functionPtr->parameterList = ParserObjectFunctionParameter::parse(parser);

		// Consome o retorno se houver.
		if (parser->isArrow())
		{
			// Consome '->'
			parser->expectToken(TokenSubType_e::Arrow);

			// Consome o tipo retorno.
			functionPtr->returnType = ParserObjectTypeDecl::parse(parser);
		}
		else
		{
			// Consome o tipo retorno.
			functionPtr->returnType = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		// Consome bloco se houver.
		if (!parser->isRightBracket())
		{
			functionPtr->blockDecl = ParserObjectBlockDecl::parse(parser);
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);

		return functionPtr;
	}
} }