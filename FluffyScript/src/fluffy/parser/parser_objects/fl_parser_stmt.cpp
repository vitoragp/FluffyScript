#include "..\..\fl_exceptions.h"
#include "..\fl_ast_stmt.h"
#include "fl_parser_objects.h"
namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectStmtDecl
	 */

	StmtDeclPtr ParserObjectStmtDecl::parse(Parser* parser, Bool skipOnly)
	{
		return parseStmt(parser, skipOnly);
	}

	StmtDeclPtr ParserObjectStmtDecl::parseStmt(Parser* parser, Bool skipOnly) 
	{
		switch (parser->getTokenType())
		{
		case TokenType_e::If:
			{
				// Verifica se e um 'if' ou um 'if let'.
				if (parser->parseNextToken().type == TokenType_e::Let)
				{
					return parseIfLet(parser, skipOnly);
				}
				return parseIf(parser, skipOnly);
			}
			break;
		case TokenType_e::For:
			return parseFor(parser, skipOnly);
		case TokenType_e::While:
			return parseWhile(parser, skipOnly);
		case TokenType_e::Do:
			return parseDoWhile(parser, skipOnly);
		case TokenType_e::Match:
			return parseMatch(parser, skipOnly);
		case TokenType_e::Return:
			return parseReturn(parser, skipOnly);
		case TokenType_e::Continue:
			return parseContinue(parser, skipOnly);
		case TokenType_e::Break:
			return parseBreak(parser, skipOnly);
		case TokenType_e::Goto:
			return parseGoto(parser, skipOnly);
		case TokenType_e::Try:
			return parseTry(parser, skipOnly);
		case TokenType_e::Panic:
			return parsePanic(parser, skipOnly);
		case TokenType_e::Let:
		case TokenType_e::Const:
			return parseVariable(parser, skipOnly);
		default:	// Label, Expr
			{
				// Parse label.
				if (parser->isIdentifier())
				{
					if (parser->parseNextToken().type == TokenType_e::Colon)
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
			parser->expectToken(TokenType_e::If);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			if (parser->isElse()) {
				parser->expectToken(TokenType_e::Else);
				ParserObjectBlockDecl::parse(parser, skipOnly);
			}
			return nullptr;
		}

		auto ifDecl = std::make_unique<ast::StmtIfDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'if'.
		parser->expectToken(TokenType_e::If);

		// Consome expressao.
		ifDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome o block if.
		ifDecl->ifBlockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

		// Verifica se existe o bloco else.
		if (parser->isElse())
		{
			// Consome 'else'.
			parser->expectToken(TokenType_e::Else);

			// Consome o block else.
			ifDecl->elseBlockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);
		}

		return ifDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseIfLet(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::If);
			parser->expectToken(TokenType_e::Let);
			ParserObjectPatternDecl::parse(parser, skipOnly);
			parser->expectToken(TokenType_e::Assign);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			ParserObjectBlockDecl::parse(parser, skipOnly);
			if (parser->isElse()) {
				parser->expectToken(TokenType_e::Else);
				ParserObjectBlockDecl::parse(parser, skipOnly);
			}
			return nullptr;
		}

		auto ifLefDecl = std::make_unique<ast::StmtIfLetDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'if'.
		parser->expectToken(TokenType_e::If);

		// Consome 'let'.
		parser->expectToken(TokenType_e::Let);

		// Consome pattern.
		ifLefDecl->patternDecl = ParserObjectPatternDecl::parse(parser, skipOnly);

		// Consome '='.
		parser->expectToken(TokenType_e::Assign);

		// Consome expressao.
		ifLefDecl->expressionDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome o block if.
		ifLefDecl->ifBlockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

		// Verifica se existe o bloco else.
		if (parser->isElse())
		{
			// Consome 'else'.
			parser->expectToken(TokenType_e::Else);

			// Consome o block else.
			ifLefDecl->elseBlockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);
		}
		return ifLefDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseFor(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::For);
			if (parser->isLet())
			{
				parser->expectIdentifier();
				parser->expectToken(TokenType_e::Assign);
				ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			} else {
				ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			}
			parser->expectToken(TokenType_e::SemiColon);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			parser->expectToken(TokenType_e::SemiColon);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			ParserObjectBlockDecl::parse(parser, skipOnly);
			return nullptr;
		}

		auto forDecl = std::make_unique<ast::StmtForDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);
		
		// Consome 'for'.
		parser->expectToken(TokenType_e::For);

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
			parser->expectToken(TokenType_e::Assign);

			// Consome expressao.
			forDecl->initStmtDecl->initExpression = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
		} else {
			// Consome expressao.
			forDecl->initExprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
		}

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		// Consome expressao.
		forDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		// Consome expressao.
		forDecl->updateExprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome bloco.
		forDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

		return forDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseWhile(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::While);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			ParserObjectBlockDecl::parse(parser, skipOnly);
			return nullptr;
		}

		auto whileDecl = std::make_unique<ast::StmtWhileDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'while'.
		parser->expectToken(TokenType_e::While);

		// Consome expressao
		whileDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome bloco.
		whileDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

		return whileDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseDoWhile(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Do);
			ParserObjectBlockDecl::parse(parser, skipOnly);
			parser->expectToken(TokenType_e::While);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			return nullptr;
		}

		auto doWhileDecl = std::make_unique<ast::StmtDoWhileDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'do'.
		parser->expectToken(TokenType_e::Do);

		// Consome bloco.
		doWhileDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

		// Consome 'while'.
		parser->expectToken(TokenType_e::While);

		// Consome expressao
		doWhileDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return doWhileDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseMatch(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Match);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			parser->expectToken(TokenType_e::LBracket);
			while (true)
			{
				if (parser->isRightBracket())
				{
					break;
				}

			parsePatternlabelSkip:
				parser->expectToken(TokenType_e::When);
				ParserObjectPatternDecl::parse(parser, skipOnly);
				parser->expectToken(TokenType_e::Arrow);
				ParserObjectBlockDecl::parse(parser, skipOnly);
				if (parser->isComma())
				{
					parser->expectToken(TokenType_e::Comma);
					if (parser->isRightBracket()) {
						break;
					}
					goto parsePatternlabelSkip;
				}
				if (parser->isRightBracket())
				{
					break;
				}
				throw exceptions::unexpected_with_possibilities_token_exception(
					parser->getTokenValue(),
					{
						TokenType_e::When,
						TokenType_e::Comma,
						TokenType_e::RBracket
					},
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			parser->expectToken(TokenType_e::RBracket);
			return nullptr;
		}

		auto matchDecl = std::make_unique<ast::StmtMatchDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'match'.
		parser->expectToken(TokenType_e::Match);

		// Consome expressao.
		matchDecl->conditionExprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome '{'.
		parser->expectToken(TokenType_e::LBracket);

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

		parsePatternlabel:
			auto whenDecl = std::make_unique<ast::MatchWhenDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Consome 'when'.
			parser->expectToken(TokenType_e::When);

			// Consome pattern.
			whenDecl->patternDecl = ParserObjectPatternDecl::parse(parser, skipOnly);

			// Consome '->'.
			parser->expectToken(TokenType_e::Arrow);

			// Consome bloco.
			whenDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

			// Adiciona a declaracao when a lista.
			matchDecl->whenDeclList.push_back(std::move(whenDecl));

			if (parser->isComma())
			{
				// Consome ','
				parser->expectToken(TokenType_e::Comma);

				// Se for '}' sai do loop.
				if (parser->isRightBracket()) {
					break;
				}
				goto parsePatternlabel;
			}

			if (parser->isRightBracket())
			{
				break;
			}

			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenType_e::Comma,
					TokenType_e::RBracket
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome '}'.
		parser->expectToken(TokenType_e::RBracket);

		return matchDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseReturn(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Return);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			parser->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto returnDecl = std::make_unique<ast::StmtReturnDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'return'.
		parser->expectToken(TokenType_e::Return);

		// Consome bloco.
		returnDecl->exprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return returnDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseContinue(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Continue);
			parser->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto continueDecl = std::make_unique<ast::StmtContinueDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'continue'.
		parser->expectToken(TokenType_e::Continue);

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return continueDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseBreak(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Break);
			parser->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto continueDecl = std::make_unique<ast::StmtBreakDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'break'.
		parser->expectToken(TokenType_e::Break);

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return continueDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseGoto(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Goto);
			parser->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto gotoDecl = std::make_unique<ast::StmtGotoDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'goto'.
		parser->expectToken(TokenType_e::Goto);

		// Consome identificador.
		gotoDecl->labelIdentifier = parser->expectIdentifier();

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return gotoDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseLabel(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectIdentifier();
			parser->expectToken(TokenType_e::Colon);
			return nullptr;
		}

		auto labelDecl = std::make_unique<ast::StmtLabelDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome identificador.
		labelDecl->identifier = parser->expectIdentifier();

		// Consome ':'.
		parser->expectToken(TokenType_e::Colon);

		return labelDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseTry(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Do);
			ParserObjectBlockDecl::parse(parser, skipOnly);
			parser->expectToken(TokenType_e::Catch);
			ParserObjectTypeDecl::parse(parser);
			parser->expectIdentifier();
			ParserObjectBlockDecl::parse(parser, skipOnly);
			while (true)
			{
				if (parser->isCatch())
				{
					parser->expectToken(TokenType_e::Catch);
					ParserObjectTypeDecl::parse(parser);
					parser->expectIdentifier();
					ParserObjectBlockDecl::parse(parser, skipOnly);
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
		parser->expectToken(TokenType_e::Try);

		// Consome bloco.
		tryDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);
		{
			auto catchDecl = std::make_unique<ast::stmt::StmtTryCatchBlockDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Consome 'catch'
			parser->expectToken(TokenType_e::Catch);

			// Consome declaracao catch.
			catchDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

			// Consome identificador.
			catchDecl->identifier = parser->expectIdentifier();

			// Consome bloco.
			catchDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

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
				parser->expectToken(TokenType_e::Catch);

				// Consome declaracao catch.
				catchDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

				// Consome identificador.
				catchDecl->identifier = parser->expectIdentifier();

				// Consome bloco.
				catchDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);

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
			parser->expectToken(TokenType_e::Panic);
			parser->expectToken(TokenType_e::LParBracket);
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			parser->expectToken(TokenType_e::RParBracket);
			parser->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto panicDecl = std::make_unique<ast::StmtPanicDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'panic'.
		parser->expectToken(TokenType_e::Panic);

		// Consome '('.
		parser->expectToken(TokenType_e::LParBracket);

		// Consome identificador.
		panicDecl->exprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome ')'.
		parser->expectToken(TokenType_e::RParBracket);

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return panicDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseVariable(Parser* parser, Bool skipOnly) 
	{
		if (skipOnly)
		{
			Bool isConst = false;
			switch (parser->getTokenType())
			{
			case TokenType_e::Let:
				isConst = false;
				parser->expectToken(TokenType_e::Let);
				break;
			case TokenType_e::Const:
				isConst = true;
				parser->expectToken(TokenType_e::Const);
				break;
			default:
				throw exceptions::unexpected_with_possibilities_token_exception(
					parser->getTokenValue(),
					{
						TokenType_e::Let,
						TokenType_e::Const
					},
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			if (parser->isRef())
			{
				parser->expectToken(TokenType_e::Ref);
			}

			if (parser->isIdentifier())
			{
				parser->expectIdentifier();
			}
			else if (parser->isLeftBracket() || parser->isLeftParBracket())
			{
				ParserObjectPatternDecl::parse(parser, skipOnly);
			}

			Bool mustHaveInitExpression = isConst ? true : false;

			// Verifica se o tipo foi declarado.
			if (parser->isColon())
			{
				parser->expectToken(TokenType_e::Colon);
				if (parser->isVoid())
				{
					throw exceptions::custom_exception(
						"Variables or constant can't have void type",
						parser->getTokenLine(),
						parser->getTokenColumn()
					);
				}
				ParserObjectTypeDecl::parse(parser);
			} else {
				mustHaveInitExpression = true;
			}
			if (mustHaveInitExpression)
			{
				parser->expectToken(TokenType_e::Assign);
				ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::Interrogation, true);
			}

			// Consome ';'.
			parser->expectToken(TokenType_e::SemiColon);

			return nullptr;
		}

		auto variableDecl = std::make_unique<ast::StmtVariableDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome o tipo de variavel.
		switch (parser->getTokenType())
		{
		case TokenType_e::Let:
			parser->expectToken(TokenType_e::Let);
			variableDecl->isConst = false;
			break;
		case TokenType_e::Const:
			parser->expectToken(TokenType_e::Const);
			variableDecl->isConst = true;
			break;
		default:
			throw exceptions::unexpected_with_possibilities_token_exception(
				parser->getTokenValue(),
				{
					TokenType_e::Let,
					TokenType_e::Const
				},
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome 'ref'.
		if (parser->isRef())
		{
			parser->expectToken(TokenType_e::Ref);
			variableDecl->isReference = true;
		}

		// Consome identificador.
		if (parser->isIdentifier())
		{
			variableDecl->identifier = parser->expectIdentifier();
		}
		else if (parser->isLeftBracket() || parser->isLeftParBracket())
		{
			variableDecl->destructuringPatternDecl = ParserObjectPatternDecl::parse(parser, skipOnly);
		}

		Bool mustHaveInitExpression = variableDecl->isConst ? true : false;

		// Verifica se o tipo foi declarado.
		if (parser->isColon())
		{
			// Consome ':'.
			parser->expectToken(TokenType_e::Colon);

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
			parser->expectToken(TokenType_e::Assign);

			// Processa a expressao superficialmente em busca de erros de sintaxe.
			variableDecl->initExpression = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::Interrogation, false);;
		}

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return variableDecl;
	}

	StmtDeclPtr ParserObjectStmtDecl::parseExpr(Parser* parser, Bool skipOnly) 
	{ 
		if (skipOnly)
		{
			ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
			parser->expectToken(TokenType_e::SemiColon);
			return nullptr;
		}

		auto exprDecl = std::make_unique<ast::StmtExprDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome identificador.
		exprDecl->exprDecl = ParserObjectExpressionDecl::parse(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

		// Consome ';'.
		parser->expectToken(TokenType_e::SemiColon);

		return exprDecl;
	}
} }