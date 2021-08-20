#include "..\fl_ast_expr.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectBlockDecl
	 */
	
	BlockDeclPtr ParserObjectBlockDecl::parse(Parser* parser)
	{
		auto blockDecl = std::make_unique<ast::BlockDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

			blockDecl->stmtList.push_back(ParserObjectStmtDecl::parse(parser));
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);
		return blockDecl;
	}

	BlockDeclPtr ParserObjectBlockDecl::skip(Parser* parser)
	{				
		// Consome '{'
		parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

			ParserObjectStmtDecl::skip(parser);
		}

		// Consome '}'
		parser->expectToken(TokenSubType_e::RBracket);

		return nullptr;
	}
} }