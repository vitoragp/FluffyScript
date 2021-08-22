#include "..\fl_ast_expr.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {

	/**
	 * ParserObjectBlockDecl
	 */
	
	BlockDeclPtr ParserObjectBlockDecl::parse(Parser* parser, Bool skipOnly)
	{
		if (skipOnly) {
			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();

			const U32 beginPosition = parser->getTokenPosition();

			// Consome '{'
			parser->expectToken(TokenType_e::LBracket);

			while (true)
			{
				if (parser->isRightBracket())
				{
					break;
				}

				ParserObjectStmtDecl::parse(parser, skipOnly);
			}

			// Consome '}'
			parser->expectToken(TokenType_e::RBracket);

			const U32 endPosition = parser->getTokenPosition();

			return std::make_unique<ast::BlockDecl>(
				beginPosition,
				endPosition,
				line,
				column
			);
		}

		auto blockDecl = std::make_unique<ast::BlockDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome '{'
		parser->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

			blockDecl->stmtList.push_back(ParserObjectStmtDecl::parse(parser, skipOnly));
		}

		// Consome '}'
		parser->expectToken(TokenType_e::RBracket);
		return blockDecl;
	}
} }