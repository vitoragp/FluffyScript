#include "..\..\fl_exceptions.h"
#include "..\fl_ast_stmt.h"
#include "fl_parser_objects.h"
namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectStmtDecl
	 */

	StmtDeclPtr ParserObjectStmtDecl::parse(Parser* parser)
	{
		return parseStmt(parser, false);
	}

	StmtDeclPtr ParserObjectStmtDecl::skip(Parser* parser)
	{
		return parseStmt(parser, true);
	}

	StmtDeclPtr ParserObjectStmtDecl::parseStmt(Parser* parser, Bool skipOnly) 
	{
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::If:
			{
				// Verifica se e um 'if' ou um 'if let'.
				if (parser->parseNextToken().subType == TokenSubType_e::Let)
				{
					return parseIfLet(parser, skipOnly);
				}
				return parseIf(parser, skipOnly);
			}
			break;
		case TokenSubType_e::For:
			return parseFor(parser, skipOnly);
		case TokenSubType_e::While:
			return parseWhile(parser, skipOnly);
		case TokenSubType_e::Do:
			return parseDoWhile(parser, skipOnly);
		case TokenSubType_e::Match:
			return parseMatch(parser, skipOnly);
		case TokenSubType_e::Return:
			return parseReturn(parser, skipOnly);
		case TokenSubType_e::Continue:
			return parseContinue(parser, skipOnly);
		case TokenSubType_e::Break:
			return parseBreak(parser, skipOnly);
		case TokenSubType_e::Goto:
			return parseGoto(parser, skipOnly);
		case TokenSubType_e::Try:
			return parseTry(parser, skipOnly);
		case TokenSubType_e::Panic:
			return parsePanic(parser, skipOnly);
		case TokenSubType_e::Let:
		case TokenSubType_e::Const:
			return parseVariable(parser, skipOnly);
		default:	// Label, Expr
			{
				// Parse label.
				if (parser->isIdentifier())
				{
					if (parser->parseNextToken().subType == TokenSubType_e::Colon)
					{
						return parseLabel(parser, skipOnly);
					}
				}
				return parseExpr(parser, skipOnly);
			}
			break;
		}
		return nullptr;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseIf(Parser* parser, Bool skipOnly) 
	{
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::If);
			ParserObjectExpressionDecl::skip(parser);
			if (parser->isElse()) {
				parser->expectToken(TokenSubType_e::Else);
				ParserObjectBlockDecl::skip(parser);
			}
			return nullptr;
		}

		auto ifDecl = std::make_unique<ast::StmtIfDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'if'.
		parser->expectToken(TokenSubType_e::If);

		// Consome expressao.
		ifDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome o block if.
		ifDecl->ifBlockDecl = ParserObjectBlockDecl::parse(parser);

		// Verifica se existe o bloco else.
		if (parser->isElse())
		{
			// Consome 'else'.
			parser->expectToken(TokenSubType_e::Else);

			// Consome o block else.
			ifDecl->elseBlockDecl = ParserObjectBlockDecl::parse(parser);
		}

		return ifDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseIfLet(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::If);
			parser->expectToken(TokenSubType_e::Let);
			ParserObjectPatternDecl::skip(parser);
			ParserObjectBlockDecl::skip(parser);
			if (parser->isElse()) {
				parser->expectToken(TokenSubType_e::Else);
				ParserObjectBlockDecl::skip(parser);
			}
			return nullptr;
		}

		auto ifLefDecl = std::make_unique<ast::StmtIfLetDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'if'.
		parser->expectToken(TokenSubType_e::If);

		// Consome 'let'.
		parser->expectToken(TokenSubType_e::Let);

		// Consome expressao.
		ifLefDecl->patternDecl = ParserObjectPatternDecl::parse(parser);

		// Consome o block if.
		ifLefDecl->ifBlockDecl = ParserObjectBlockDecl::parse(parser);

		// Verifica se existe o bloco else.
		if (parser->isElse())
		{
			// Consome 'else'.
			parser->expectToken(TokenSubType_e::Else);

			// Consome o block else.
			ifLefDecl->elseBlockDecl = ParserObjectBlockDecl::parse(parser);
		}

		return ifLefDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseFor(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::For);
			if (parser->isLet())
			{
				parser->expectIdentifier();
				parser->expectToken(TokenSubType_e::Assign);
				ParserObjectExpressionDecl::skip(parser);
			} else {
				ParserObjectExpressionDecl::skip(parser);
			}
			parser->expectToken(TokenSubType_e::SemiColon);
			ParserObjectExpressionDecl::skip(parser);
			parser->expectToken(TokenSubType_e::SemiColon);
			ParserObjectExpressionDecl::skip(parser);
			ParserObjectBlockDecl::skip(parser);
			return nullptr;
		}

		auto forDecl = std::make_unique<ast::StmtForDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);
		
		// Consome 'for'.
		parser->expectToken(TokenSubType_e::For);

		// Verifica se e uma expressao e inicilizacao.
		if (parser->isLet())
		{
			forDecl->initStmtDecl = std::make_unique<ast::StmtForInitDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Consome o identificador.
			forDecl->initStmtDecl->identifier = parser->expectIdentifier();

			// Consome '='.
			parser->expectToken(TokenSubType_e::Assign);

			// Consome expressao.
			forDecl->initStmtDecl->initExpression = ParserObjectExpressionDecl::parse(parser);
		} else {
			// Consome expressao.
			forDecl->initExprDecl = ParserObjectExpressionDecl::parse(parser);
		}

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		// Consome expressao.
		forDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		// Consome expressao.
		forDecl->updateExprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome bloco.
		forDecl->blockDecl = ParserObjectBlockDecl::parse(parser);

		return forDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseWhile(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::While);
			ParserObjectExpressionDecl::skip(parser);
			ParserObjectBlockDecl::skip(parser);
			return nullptr;
		}

		auto whileDecl = std::make_unique<ast::StmtWhileDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'while'.
		parser->expectToken(TokenSubType_e::While);

		// Consome expressao
		whileDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome bloco.
		whileDecl->blockDecl = ParserObjectBlockDecl::parse(parser);

		return whileDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseDoWhile(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Do);
			ParserObjectBlockDecl::skip(parser);
			parser->expectToken(TokenSubType_e::While);
			ParserObjectExpressionDecl::skip(parser);
			return nullptr;
		}

		auto doWhileDecl = std::make_unique<ast::StmtDoWhileDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'do'.
		parser->expectToken(TokenSubType_e::Do);

		// Consome bloco.
		doWhileDecl->blockDecl = ParserObjectBlockDecl::parse(parser);

		// Consome 'while'.
		parser->expectToken(TokenSubType_e::While);

		// Consome expressao
		doWhileDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return doWhileDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseMatch(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Match);
			ParserObjectExpressionDecl::skip(parser);
			parser->expectToken(TokenSubType_e::LBracket);
			while (true)
			{
				if (parser->isRightBracket())
				{
					break;
				}

			parsePatternlabelSkip:
				parser->expectToken(TokenSubType_e::When);
				ParserObjectPatternDecl::skip(parser);
				if (parser->isComma())
				{
					parser->expectToken(TokenSubType_e::Comma);
					goto parsePatternlabelSkip;
				}
				if (parser->isRightBracket())
				{
					break;
				}
				throw exceptions::unexpected_with_possibilities_token_exception(
					parser->getTokenValue(),
					{
						TokenSubType_e::When,
						TokenSubType_e::Comma,
						TokenSubType_e::RBracket
					},
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			parser->expectToken(TokenSubType_e::RBracket);
			return nullptr;
		}

		auto matchDecl = std::make_unique<ast::StmtMatchDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'match'.
		parser->expectToken(TokenSubType_e::Match);

		// Consome expressao.
		matchDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome '{'.
		parser->expectToken(TokenSubType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

		parsePatternlabel:

			// Consome 'when'.
			parser->expectToken(TokenSubType_e::When);

			// Consome pattern.
			matchDecl->patternList.push_back(ParserObjectPatternDecl::parse(parser));

			if (parser->isComma())
			{
				// Consome ','
				parser->expectToken(TokenSubType_e::Comma);
				goto parsePatternlabel;
			}

			if (parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenSubType_e::When,
					TokenSubType_e::Comma,
					TokenSubType_e::RBracket
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '}'.
		parser->expectToken(TokenSubType_e::RBracket);

		return matchDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseReturn(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Return);
			ParserObjectExpressionDecl::skip(parser);
			parser->expectToken(TokenSubType_e::SemiColon);
			return nullptr;
		}

		auto returnDecl = std::make_unique<ast::StmtReturnDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'return'.
		parser->expectToken(TokenSubType_e::Return);

		// Consome bloco.
		returnDecl->exprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return returnDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseContinue(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Continue);
			parser->expectToken(TokenSubType_e::SemiColon);
			return nullptr;
		}

		auto continueDecl = std::make_unique<ast::StmtContinueDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'continue'.
		parser->expectToken(TokenSubType_e::Continue);

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return continueDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseBreak(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Break);
			parser->expectToken(TokenSubType_e::SemiColon);
			return nullptr;
		}

		auto continueDecl = std::make_unique<ast::StmtBreakDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'break'.
		parser->expectToken(TokenSubType_e::Break);

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return continueDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseGoto(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Goto);
			parser->expectToken(TokenSubType_e::SemiColon);
			return nullptr;
		}

		auto gotoDecl = std::make_unique<ast::StmtGotoDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'goto'.
		parser->expectToken(TokenSubType_e::Goto);

		// Consome identificador.
		gotoDecl->labelIdentifier = parser->expectIdentifier();

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return gotoDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseLabel(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectIdentifier();
			parser->expectToken(TokenSubType_e::Colon);
			return nullptr;
		}

		auto labelDecl = std::make_unique<ast::StmtLabelDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome identificador.
		labelDecl->identifier = parser->expectIdentifier();

		// Consome ':'.
		parser->expectToken(TokenSubType_e::Colon);

		return labelDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseTry(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Do);
			ParserObjectBlockDecl::skip(parser);
			parser->expectToken(TokenSubType_e::Catch);
			ParserObjectTypeDecl::skip(parser);
			parser->expectIdentifier();
			ParserObjectBlockDecl::skip(parser);
			while (true)
			{
				if (parser->isCatch())
				{
					parser->expectToken(TokenSubType_e::Catch);
					ParserObjectTypeDecl::skip(parser);
					parser->expectIdentifier();
					ParserObjectBlockDecl::skip(parser);
					continue;
				}
				break;
			}
			return nullptr;
		}

		auto tryDecl = std::make_unique<ast::StmtTryDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'try'.
		parser->expectToken(TokenSubType_e::Try);

		// Consome bloco.
		tryDecl->blockDecl = ParserObjectBlockDecl::parse(parser);
		{
			auto catchDecl = std::make_unique<ast::stmt::StmtTryCatchBlockDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Consome 'catch'
			parser->expectToken(TokenSubType_e::Catch);

			// Consome declaracao catch.
			catchDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

			// Consome identificador.
			catchDecl->identifier = parser->expectIdentifier();

			// Consome bloco.
			catchDecl->blockDecl = ParserObjectBlockDecl::parse(parser);

			// Adiciona catch a lista.
			tryDecl->catchDeclList.push_back(std::move(catchDecl));
		}

		while (true)
		{
			if (parser->isCatch())
			{
				auto catchDecl = std::make_unique<ast::StmtTryCatchBlockDecl>(
					parser->getTokenLine(),
					parser->getTokenColumn()
					);

				// Consome 'catch'
				parser->expectToken(TokenSubType_e::Catch);

				// Consome declaracao catch.
				catchDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

				// Consome identificador.
				catchDecl->identifier = parser->expectIdentifier();

				// Consome bloco.
				catchDecl->blockDecl = ParserObjectBlockDecl::parse(parser);

				// Adiciona catch a lista.
				tryDecl->catchDeclList.push_back(std::move(catchDecl));
				continue;
			}
			break;
		}
		return tryDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parsePanic(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenSubType_e::Panic);
			parser->expectToken(TokenSubType_e::LParBracket);
			ParserObjectExpressionDecl::skip(parser);
			parser->expectToken(TokenSubType_e::RParBracket);
			parser->expectToken(TokenSubType_e::SemiColon);
			return nullptr;
		}

		auto panicDecl = std::make_unique<ast::StmtPanicDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'panic'.
		parser->expectToken(TokenSubType_e::Panic);

		// Consome '('.
		parser->expectToken(TokenSubType_e::LParBracket);

		// Consome identificador.
		panicDecl->exprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome ')'.
		parser->expectToken(TokenSubType_e::RParBracket);

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return panicDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseVariable(Parser* parser, Bool skipOnly) 
	{
		if (skipOnly)
		{
			Bool isConst = false;
			switch (parser->getTokenSubType())
			{
			case TokenSubType_e::Let:
				isConst = false;
				parser->expectToken(TokenSubType_e::Let);
				break;
			case TokenSubType_e::Const:
				isConst = true;
				parser->expectToken(TokenSubType_e::Const);
				break;
			default:
				throw exceptions::unexpected_with_possibilities_token_exception(
					parser->getTokenValue(),
					{
						TokenSubType_e::Let,
						TokenSubType_e::Const
					},
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			if (parser->isRef())
			{
				parser->expectToken(TokenSubType_e::Ref);
			}
			parser->expectIdentifier();

			Bool mustHaveInitExpression = isConst ? true : false;

			// Verifica se o tipo foi declarado.
			if (parser->isColon())
			{
				parser->expectToken(TokenSubType_e::Colon);
				if (parser->isVoid())
				{
					throw exceptions::custom_exception(
						"Variables or constant can't have void type",
						parser->getTokenLine(),
						parser->getTokenColumn()
					);
				}
				ParserObjectTypeDecl::skip(parser);
			} else {
				mustHaveInitExpression = true;
			}
			if (mustHaveInitExpression)
			{
				parser->expectToken(TokenSubType_e::Assign);
				ParserObjectExpressionDecl::skip(parser);
			}

			// Consome ';'.
			parser->expectToken(TokenSubType_e::SemiColon);

			return nullptr;
		}

		auto variableDecl = std::make_unique<ast::StmtVariableDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome o tipo de variavel.
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Let:
			parser->expectToken(TokenSubType_e::Let);
			variableDecl->isConst = false;
			break;
		case TokenSubType_e::Const:
			parser->expectToken(TokenSubType_e::Const);
			variableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenSubType_e::Let,
					TokenSubType_e::Const
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome 'ref'.
		if (parser->isRef())
		{
			parser->expectToken(TokenSubType_e::Ref);
			variableDecl->isReference = true;
		}

		// Consome identificador.
		variableDecl->identifier = parser->expectIdentifier();

		Bool mustHaveInitExpression = variableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (parser->isColon())
		{
			// Consome ':'.
			parser->expectToken(TokenSubType_e::Colon);

			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();

			// Consome o tipo.
			variableDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

			// Verifica se o tipo e valido.
			if (variableDecl->typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception(
					"Variables or constant can't have void type",
					line,
					column
				);
			}
		}
		else {
			// Se o tipo nao e declarado explicitamente a variavel deve ser iniciado.
			mustHaveInitExpression = true;
		}

		// Consome a expressao de inicializacao.
		if (mustHaveInitExpression)
		{
			// Consome '='.
			parser->expectToken(TokenSubType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			variableDecl->initExpression = ParserObjectExpressionDecl::parse(parser);
		}

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return variableDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseExpr(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			ParserObjectExpressionDecl::skip(parser);
			parser->expectToken(TokenSubType_e::SemiColon);
			return nullptr;
		}

		auto exprDecl = std::make_unique<ast::StmtExprDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome identificador.
		exprDecl->exprDecl = ParserObjectExpressionDecl::parse(parser);

		// Consome ';'.
		parser->expectToken(TokenSubType_e::SemiColon);

		return exprDecl;
	}
} }