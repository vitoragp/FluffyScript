#include "..\fl_ast_expr.h"
#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy {
	/**
	 * isExprOperator
	 */

	bool isExprOperator(TokenType_e type)
	{
		switch (type) {
		case TokenType_e::Comma:
		case TokenType_e::Assign:
		case TokenType_e::LogicalOr:
		case TokenType_e::LogicalAnd:
		case TokenType_e::BitWiseOr:
		case TokenType_e::BitWiseXor:
		case TokenType_e::BitWiseAnd:
		case TokenType_e::BitWiseOrAssign:
		case TokenType_e::BitWiseXorAssign:
		case TokenType_e::BitWiseAndAssign:
		case TokenType_e::BitWiseRShiftAssign:
		case TokenType_e::BitWiseLShiftAssign:
		case TokenType_e::Equal:
		case TokenType_e::NotEqual:
		case TokenType_e::GreaterThan:
		case TokenType_e::LessThan:
		case TokenType_e::GreaterThanOrEqual:
		case TokenType_e::LessThanOrEqual:
		case TokenType_e::BitWiseLShift:
		case TokenType_e::BitWiseRShift:
		case TokenType_e::Plus:
		case TokenType_e::Minus:
		case TokenType_e::Multiplication:
		case TokenType_e::Division:
		case TokenType_e::Modulo:
		case TokenType_e::PlusAssign:
		case TokenType_e::MinusAssign:
		case TokenType_e::MultAssign:
		case TokenType_e::DivAssign:
		case TokenType_e::ModAssign:
		case TokenType_e::Increment:
		case TokenType_e::Decrement:
		case TokenType_e::As:
		case TokenType_e::Is:
		case TokenType_e::LSquBracket:
		case TokenType_e::LParBracket:
		case TokenType_e::Dot:
		case TokenType_e::BitWiseNot:
		case TokenType_e::LogicalNot:
		case TokenType_e::ScopeResolution:
		case TokenType_e::Interrogation:
		case TokenType_e::Match:
		case TokenType_e::Ref:
			return true;
		default:
			break;
		}
		return false;
	}

	/**
	 * TokenInfo_t
	 */

	struct TokenInfo_t
	{
		TokenType_e	op;
		U32				prec;
		Bool			left;
	};

	static const TokenInfo_t op_info[] = {
			{ TokenType_e::Comma,				1, true },
			{ TokenType_e::Interrogation,		2, false },
			{ TokenType_e::Assign,				2, false },
			{ TokenType_e::PlusAssign,			2, false },
			{ TokenType_e::MinusAssign,			2, false },
			{ TokenType_e::MultAssign,			2, false },
			{ TokenType_e::DivAssign,			2, false },
			{ TokenType_e::ModAssign,			2, false },
			{ TokenType_e::BitWiseOrAssign,		2, false },
			{ TokenType_e::BitWiseXorAssign,	2, false },
			{ TokenType_e::BitWiseAndAssign,	2, false },
			{ TokenType_e::BitWiseLShiftAssign,	2, false },
			{ TokenType_e::BitWiseRShiftAssign,	2, false },
			{ TokenType_e::LogicalOr,			3, true },
			{ TokenType_e::LogicalAnd,			4, true },
			{ TokenType_e::BitWiseOr,			5, true },
			{ TokenType_e::BitWiseXor,			6, true },
			{ TokenType_e::BitWiseAnd,			7, true },
			{ TokenType_e::Equal,			    8, true },
			{ TokenType_e::NotEqual,			8, true },
			{ TokenType_e::GreaterThan,		    9, true },
			{ TokenType_e::LessThan,			9, true },
			{ TokenType_e::GreaterThanOrEqual,  9, true },
			{ TokenType_e::LessThanOrEqual,		9, true },
			{ TokenType_e::BitWiseLShift,		10, true },
			{ TokenType_e::BitWiseRShift,		10, true },
			{ TokenType_e::Plus,				11, true },   // Add
			{ TokenType_e::Minus,				11, true },   // Sub
			{ TokenType_e::Multiplication,		12, true },
			{ TokenType_e::Division,			12, true },
			{ TokenType_e::Modulo,				12, true },
			{ TokenType_e::Increment,			13, false },  // Prefix
			{ TokenType_e::Decrement,			13, false },  // Prefix
			{ TokenType_e::Ref,					13, false },  // Prefix
			{ TokenType_e::Plus,				13, false },  // Unary positive
			{ TokenType_e::Minus,				13, false },  // Unary negative
			{ TokenType_e::Match,				13, false },
			{ TokenType_e::As,					13, true },
			{ TokenType_e::Is,					13, true },
			{ TokenType_e::LSquBracket,			14, true },   // Array
			{ TokenType_e::LParBracket,			14, true },   // Function Call
			{ TokenType_e::Increment,			14, true },   // Postfix
			{ TokenType_e::Decrement,			14, true },   // Postfix
			{ TokenType_e::Dot,					14, true },
			{ TokenType_e::BitWiseNot,			14, true },
			{ TokenType_e::LogicalNot,			14, true },
			{ TokenType_e::ScopeResolution,		15, true },
			{ TokenType_e::Unknown,				0, true }
	};

	// Retorna informacoes sobre os operadores.
	const TokenInfo_t& getOperatorInfo(TokenType_e op)
	{
		U32 current_op = 0;
		while (true)
		{
			if (op_info[current_op].op == op)
			{
				return op_info[current_op];
			}
			if (op_info[current_op].op == TokenType_e::Unknown)
			{
				return op_info[current_op];
			}
			current_op++;
		}
	}
}

namespace fluffy { namespace parser_objects {
	using namespace fluffy::ast::expr;

	/**
	 * ParserObjectExpressionDecl
	 */

	ExpressionDeclPtr ParserObjectExpressionDecl::parse(Parser* parser, U32 prec, Bool skipOnly)
	{
		if (skipOnly) {
			const U32 line = parser->getTokenLine();
			const U32 column = parser->getTokenColumn();
			const U32 beginPosition = parser->getTokenPosition();
			parseExpression(parser, prec, skipOnly);
			const U32 endPosition = parser->getTokenPosition();
			return std::make_unique<ExpressionDeclMark>(line, column, beginPosition, endPosition);
		}
		return parseExpression(parser, prec, skipOnly);
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::parseExpression(Parser* parser, U32 prec, Bool skipOnly)
	{
		ExpressionDeclPtr lhs;
		ExpressionDeclPtr rhs;

		const unsigned int line		= parser->getTokenLine();
		const unsigned int column	= parser->getTokenColumn();

		// Processa operadores unarios Prefixo.
		if (prec > OperatorPrecLevel_e::Unary) {
			switch (parser->getTokenType())
			{
			case TokenType_e::BitWiseNot:
			case TokenType_e::LogicalNot:
			case TokenType_e::Minus:
			case TokenType_e::Plus:
			case TokenType_e::Increment:
			case TokenType_e::Decrement:
			case TokenType_e::Ref:
				{
					// Processa superficialmente
					auto unaryExprDecl = !skipOnly ? std::make_unique<ExpressionUnaryDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					) : nullptr;

					if (!skipOnly) {
						unaryExprDecl->op = parser->getTokenType();
						unaryExprDecl->unaryType = ExpressionUnaryType_e::Prefix;
					}

					// Consome operator.
					parser->nextToken();

					// Processa expressao a direita.
					if (!skipOnly) {
						unaryExprDecl->exprDecl = parseExpression(parser, OperatorPrecLevel_e::Unary, skipOnly);
					} else {
						parseExpression(parser, OperatorPrecLevel_e::Unary, skipOnly);
					}
					return unaryExprDecl;
				}
				break;
			case TokenType_e::Match:
				{
					auto matchExprDecl = !skipOnly ? std::make_unique<ExpressionMatchDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					) : nullptr;

					// Consome operator.
					parser->nextToken();

					// Processa expressao a direita.
					if (!skipOnly) {
						matchExprDecl->exprDecl = parseExpression(parser, OperatorPrecLevel_e::Unary, skipOnly);
					} else {
						parseExpression(parser, OperatorPrecLevel_e::Unary, skipOnly);
					}

					// Consome '{'
					parser->expectToken(TokenType_e::LBracket);

					// Processa declaracao 'when'
					while (true)
					{
						auto whenDecl = !skipOnly ? std::make_unique<ExpressionMatchWhenDecl>(
							parser->getTokenLine(),
							parser->getTokenColumn()
						) : nullptr;

						// Consome 'when'
						parser->expectToken(TokenType_e::When);

						// Consome pattern.
						if (!skipOnly) {
							whenDecl->patternDecl = ParserObjectPatternDecl::parse(parser, skipOnly);
						} else {
							ParserObjectPatternDecl::parse(parser, skipOnly);
						}

						// Consome '->'
						parser->expectToken(TokenType_e::Arrow);

						// Console o bloco de codigo de expressao.
						if (!skipOnly) {
							whenDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);
						} else {
							ParserObjectBlockDecl::parse(parser, skipOnly);
						}
					}

					// Consome '}'
					parser->expectToken(TokenType_e::RBracket);

					return matchExprDecl;
				}
				break;
			default:
				break;
			}
		}

		// Processa atomos: operadores de precedencia maxima
		// como constantes e identificadores.
		if (prec > OperatorPrecLevel_e::Max) {
			return parseAtom(parser, skipOnly);
		}

		// Processa expressao a esquerda.
		if (!skipOnly) {
			lhs = parseExpression(parser, prec + 1, skipOnly);
		} else {
			parseExpression(parser, prec + 1, skipOnly);
		}

		while (true)
		{
			const TokenType_e op = parser->getTokenType();

			// Se nao e um operador de expressao sai do loop.
			if (!isExprOperator(parser->getTokenType()))
			{
				break;
			}

			// Processa operator unario posfixo.
			switch (parser->getTokenType())
			{
			case TokenType_e::Increment:
			case TokenType_e::Decrement:
				{
					// Consome o token: '++' ou '--'
					parser->nextToken();

					auto unaryExprDecl = !skipOnly ? std::make_unique<ExpressionUnaryDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					) : nullptr;

					if (!skipOnly) {
						unaryExprDecl->op = parser->getTokenType();
						unaryExprDecl->unaryType = ExpressionUnaryType_e::Posfix;
						unaryExprDecl->exprDecl = std::move(lhs);

						lhs = std::move(unaryExprDecl);
					}
				}
				continue;
			default:
				break;
			}

			// Pega informacoes do operador.
			const auto& opInfo = getOperatorInfo(parser->getTokenType());

			// Retorna se precedencia dele e menor que a atual da funcao.
			if (opInfo.prec < prec) {
				break;
			}

			// Calcula proxima precedencia minima.
			U32 nextMinPrec = opInfo.left ? opInfo.prec + 1 : opInfo.prec;
			
			// Consome o operador.
			parser->nextToken();

			// Processa chamada de funcao ou indexacao de dados.
			switch (op)
			{
			case TokenType_e::LParBracket:
				{
					// Se nao ha parametros e uma expressao unaria posfixa f().
					if (parser->isRightParBracket())
					{
						// Consome ')'
						parser->expectToken(TokenType_e::RParBracket);

						auto functionCallExpr = !skipOnly ? std::make_unique<ExpressionFunctionCall>(
							line,
							column
						) : nullptr;

						if (!skipOnly) {
							functionCallExpr->leftDecl = std::move(lhs);

							lhs = std::move(functionCallExpr);
						}
						continue;
					}

					auto functionCallExpr = !skipOnly ? std::make_unique<ExpressionFunctionCall>(
						line,
						column
					) : nullptr;

					if (!skipOnly) {
						functionCallExpr->leftDecl = std::move(lhs);
						functionCallExpr->rightDecl = parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
					} else {
						parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
					}

					// Consome ')'
					parser->expectToken(TokenType_e::RParBracket);

					if (!skipOnly) {
						lhs = std::move(functionCallExpr);
					}
					continue;
				}
				break;
			case TokenType_e::LSquBracket:
				{
					auto indexAddressExpr = !skipOnly ? std::make_unique<ExpressionIndexDecl>(
						line,
						column
					) : nullptr;

					if (!skipOnly) {
						indexAddressExpr->leftDecl = std::move(lhs);
						indexAddressExpr->rightDecl = parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
					} else {
						parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
					}

					// Consome ']'
					parser->expectToken(TokenType_e::RSquBracket);

					if (!skipOnly) {
						lhs = std::move(indexAddressExpr);
					}
					continue;
				}
				break;
			default:
				break;
			}

			// Processa expressao a direita.
			if (!skipOnly) {
				rhs = parseExpression(parser, nextMinPrec, skipOnly);
			} else {
				parseExpression(parser, nextMinPrec, skipOnly);
			}

			// Verifica se a operacao e ternaria
			if (op == TokenType_e::Interrogation)
			{
				auto ternaryExprDecl = !skipOnly ? std::make_unique<ExpressionTernaryDecl>(
					line,
					column
				) : nullptr;

				// Consome ':'
				parser->expectToken(TokenType_e::Colon);

				if (!skipOnly) {
					ternaryExprDecl->conditionDecl = std::move(lhs);
					ternaryExprDecl->leftDecl = std::move(rhs);
					ternaryExprDecl->rightDecl = parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

					lhs = std::move(ternaryExprDecl);
				}
				break;
			}

			// Processa operador binario.
			if (!skipOnly) {
				auto binaryExprDecl = std::make_unique<ExpressionBinaryDecl>(
					line,
					column
				);

				binaryExprDecl->op = op;
				binaryExprDecl->leftDecl = std::move(lhs);
				binaryExprDecl->rightDecl = std::move(rhs);

				lhs = std::move(binaryExprDecl);				
			}
		}
		return lhs;
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::parseAtom(Parser* parser, Bool skipOnly)
	{
		const U32 line = parser->getTokenLine();
		const U32 column = parser->getTokenColumn();

		// Processa expressao entre parenteses.
		if (parser->isLeftParBracket())
		{
			ExpressionDeclPtr expr;

			// Consome '('
			parser->expectToken(TokenType_e::LParBracket);

			// Processa expressao.
			if (!skipOnly) {
				expr = parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

				// Ajusta a posicao inicial
				expr->line = line;
				expr->column = column;

				// Consome ')'
				parser->expectToken(TokenType_e::RParBracket);
			} else {
				parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);

				// Consome ')'
				parser->expectToken(TokenType_e::RParBracket);
			}
			return expr;
		}

		// Processa declaracao de array.
		if (parser->isLeftSquBracket())
		{
			auto arrayIniDecl = !skipOnly ? std::make_unique<ast::ExpressionArrayInitDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			) : nullptr;

			// Consome '['
			parser->expectToken(TokenType_e::LSquBracket);

			while (true)
			{
				if (parser->isRightSquBracket())
				{
					break;
				}

				// Consome elemento
				if (!skipOnly) {
					arrayIniDecl->arrayElementDeclList.push_back(parseExpression(parser, OperatorPrecLevel_e::Interrogation, skipOnly));
				} else {
					parseExpression(parser, OperatorPrecLevel_e::Interrogation, skipOnly);
				}

				if (parser->isComma())
				{
					parser->expectToken(TokenType_e::Comma);
				}
			}

			// Consome ']'
			parser->expectToken(TokenType_e::RSquBracket);

			return arrayIniDecl;
		}

		// Processa declaracao de funcao anonima.
		if (parser->isBitWiseOr() || parser->isLogicalOr())
		{
			// Quebra token.
			if (parser->isLogicalOr())
			{
				parser->reinterpretToken(TokenType_e::BitWiseOr, 1);
			}

			auto functionDecl = !skipOnly ? std::make_unique<ExpressionFunctionDecl>(
				line,
				column
			) : nullptr;

			// Consome '|'
			parser->expectToken(TokenType_e::BitWiseOr);

			// Uma funcao anonima pode ter parametros tipados ou nao, depende do contexto.
			// Por exemplo se a funcao esta sendo passada como parametro, seus tipos podem ser
			// deduzidos com base no tipo do parametro, o mesmo pode ocorrer na declaracao de variaveis
			// ou ainda atribui-la a um membro de classe, struct.
			// Porem ha casos onde a tipagem de faz necessaria por exemplo ao usar uma declaracao de variavel
			// ou constate onde o tipo e omitido, logo, nao se poderia fazer a deducao.

			if (parser->isIdentifier()) {
				if (parser->parseNextToken().type == TokenType_e::Colon)
				{
					// Processa o restante do parametros se houver.
					while (true)
					{
						if (parser->isBitWiseOr())
						{
							break;
						}

					parseParamterDecl:
						auto paramDecl = !skipOnly ? std::make_unique<ExpressionFunctionParameterDecl>(
							parser->getTokenLine(),
							parser->getTokenColumn()
						) : nullptr;

						// Consome identificador.
						if (!skipOnly) {
							paramDecl->identifier = parser->expectIdentifier();
						} else {
							parser->expectIdentifier();
						}

						// Consome ellipsis
						if (parser->isEllipsis()) {
							if (!skipOnly) {
								paramDecl->isEllipsis = true;
							}
							parser->expectToken(TokenType_e::Ellipsis);
							goto parseFunctionRetOrBody;
						}

						// Consome ':'
						parser->expectToken(TokenType_e::Colon);

						// Consome o tipo do parametro.
						if (!skipOnly) {
							paramDecl->typeDecl = ParserObjectTypeDecl::parse(parser);
						} else {
							ParserObjectTypeDecl::skip(parser);
						}

						// Parametro nao podem ter tipo nulo.
						if (!skipOnly) {
							if (paramDecl->typeDecl->typeID == TypeDeclID_e::Void)
							{
								throw exceptions::custom_exception(
									"Parameter '%s' can't have void type",
									parser->getTokenLine(),
									parser->getTokenColumn(),
									paramDecl->identifier.str()
								);
							}

							// Adiciona o parametro a lista
							functionDecl->parametersDecl.push_back(std::move(paramDecl));
						}

						// Consome ','
						if (parser->isComma())
						{
							parser->expectToken(TokenType_e::Comma);
							goto parseParamterDecl;
						}

						if (parser->isBitWiseOr())
						{
							break;
						}

						throw exceptions::unexpected_with_possibilities_token_exception(
							parser->getTokenValue(),
							{
								TokenType_e::Comma,
								TokenType_e::BitWiseOr
							},
							parser->getTokenLine(),
							parser->getTokenColumn()
						);
					}
				} else {
					// Atribui nivel de inferencia.
					if (!skipOnly) {
						functionDecl->inferenceType = InferenceType_e::OnlyParams;
					}

					auto paramDecl = !skipOnly ? std::make_unique<ExpressionFunctionParameterDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					) : nullptr;

					if (!skipOnly) {
						paramDecl->identifier = parser->expectIdentifier();
					} else {
						parser->expectIdentifier();
					}

					// Consome ellipsis
					if (parser->isEllipsis()) {
						if (!skipOnly) {
							paramDecl->isEllipsis = true;
						}
						parser->expectToken(TokenType_e::Ellipsis);
						goto parseFunctionRetOrBody;
					}

					// Adiciona o parametro a lista
					if (!skipOnly) {
						functionDecl->parametersDecl.push_back(std::move(paramDecl));
					}

					// Processa o restante do parametros se houver.
					while (true)
					{
						if (parser->isBitWiseOr())
						{
							break;
						}

						// Consome ','
						parser->expectToken(TokenType_e::Comma);

						auto paramDecl = !skipOnly ? std::make_unique<ExpressionFunctionParameterDecl>(
							parser->getTokenLine(),
							parser->getTokenColumn()
						) : nullptr;

						if (!skipOnly) {
							paramDecl->identifier = parser->expectIdentifier();
						} else {
							parser->expectIdentifier();
						}

						// Consome ellipsis
						if (parser->isEllipsis()) {
							if (!skipOnly) {
								paramDecl->isEllipsis = true;
								functionDecl->parametersDecl.push_back(std::move(paramDecl));
							}
							parser->expectToken(TokenType_e::Ellipsis);
							goto parseFunctionRetOrBody;
						}
					}
				}
			}

		parseFunctionRetOrBody:

			// Consome '|'
			parser->expectToken(TokenType_e::BitWiseOr);

			// Verifica se o tipo de retorno e explicito.
			if (parser->isArrow()) {
				// Consome '->'
				parser->expectToken(TokenType_e::Arrow);

				if (!skipOnly) {
					functionDecl->returnTypeDecl = ParserObjectTypeDecl::parse(parser);
				} else {
					ParserObjectTypeDecl::skip(parser);
				}
			} else {
				// Atribui nivel de inferencia.
				if (!skipOnly) {
					if (functionDecl->inferenceType == InferenceType_e::OnlyParams)	{
						functionDecl->inferenceType = InferenceType_e::Full;
					} else {
						functionDecl->inferenceType = InferenceType_e::OnlyReturn;
					}

					functionDecl->returnTypeDecl = std::make_unique<ast::TypeDeclVoid>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					);
				}
			}

			// Consome bloco.
			if (!skipOnly) {
				functionDecl->blockDecl = ParserObjectBlockDecl::parse(parser, skipOnly);
			} else {
				ParserObjectBlockDecl::parse(parser, skipOnly);
			}

			return functionDecl;
		}

		// Processa new.
		if (parser->isNew())
		{
			auto newDecl = !skipOnly ? std::make_unique<ast::ExpressionNewDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			) : nullptr;

			// Consome 'new'
			parser->expectToken(TokenType_e::New);

			// Consome o tipo.
			if (!skipOnly) {
				newDecl->objectTypeDecl = ParserObjectTypeDecl::parse(parser);
			} else {
				ParserObjectTypeDecl::skip(parser);
			}

			// Consome '('
			parser->expectToken(TokenType_e::LParBracket);

			// Consome expressao no caso os parametros passados para o construtor.
			if (!parser->isRightParBracket())
			{
				if (!skipOnly) {
					newDecl->expressionDecl = ParserObjectExpressionDecl::parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
				} else {
					ParserObjectExpressionDecl::parseExpression(parser, OperatorPrecLevel_e::MinPrec, skipOnly);
				}
			}

			// Consome ')'
			parser->expectToken(TokenType_e::RParBracket);

			// Processa o bloco de inicio.
			if (parser->isLeftBracket())
			{
				// Consome '{'
				parser->expectToken(TokenType_e::LBracket);

				newDecl->objectInitBlockDecl = !skipOnly ? std::make_unique<ast::ExpressionNewBlockDecl>(
					parser->getTokenLine(),
					parser->getTokenColumn()
				) : nullptr;

				while (true)
				{
					if (parser->isRightBracket())
					{
						break;
					}

					auto itemDecl = !skipOnly ? std::make_unique<ExpressionNewItemDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					) : nullptr;

					// Processa identificador.
					if (!skipOnly) {
						itemDecl->identifier = parser->expectIdentifier();
					} else {
						parser->expectIdentifier();
					}

					// Consome ':'
					if (parser->isColon())
					{
						parser->expectToken(TokenType_e::Colon);

						// Consome expressao.
						if (!skipOnly) {
							itemDecl->expressionDecl = ParserObjectExpressionDecl::parseExpression(parser, OperatorPrecLevel_e::EnumExpr, skipOnly);
						} else {
							ParserObjectExpressionDecl::parseExpression(parser, OperatorPrecLevel_e::EnumExpr, skipOnly);
						}
					}

					// Adiciona item ao bloco.
					if (!skipOnly) {
						newDecl->objectInitBlockDecl->itemDeclList.push_back(std::move(itemDecl));
					}

					// Verifica se existem mais itens.
					if (parser->isComma())
					{
						// Consome ','
						parser->expectToken(TokenType_e::Comma);
					}
				}

				// Consome '}'
				parser->expectToken(TokenType_e::RBracket);
			}
			return newDecl;
		}

		// Processa this.
		if (parser->isThis())
		{
			// Processa superficialmente.
			if (skipOnly) {
				parser->expectToken(TokenType_e::This);
				return nullptr;
			}

			// Consome 'this'
			parser->expectToken(TokenType_e::This);

			return std::make_unique<ExpressionThisDecl>(line,column);
		}

		// Processa super.
		if (parser->isSuper())
		{
			// Processa superficialmente.
			if (skipOnly) {
				parser->expectToken(TokenType_e::Super);
				return nullptr;
			}

			// Consome 'super'
			parser->expectToken(TokenType_e::Super);

			return std::make_unique<ExpressionSuperDecl>(line, column);
		}

		// Processa null.
		if (parser->isNull())
		{
			// Processa superficialmente.
			if (skipOnly) {
				parser->expectToken(TokenType_e::Null);
				return nullptr;
			}

			// Consome 'null'
			parser->expectToken(TokenType_e::Null);

			return std::make_unique<ExpressionConstantNullDecl>(line, column);
		}

		// Processa superficialmente.
		if (skipOnly) {
			// Processa algumas constantes
			switch (parser->getTokenType())
			{
			case TokenType_e::False:
			case TokenType_e::True:
				{
					parser->nextToken();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantInteger:
				{
					parser->expectConstantInteger();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantFp32:
				{
					parser->expectConstantFp32();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantFp64:
				{
					parser->expectConstantFp64();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantString:
				{
					parser->expectConstantString();
					return nullptr;
				}
				break;
			case TokenType_e::ConstantChar:
				{
					parser->expectConstantChar();
					return nullptr;
				}
				break;
			default:
				break;
			}

			// Resolucao de escopo unaria, indica acesso ao escopo global.
			if (parser->isScopeResolution())
			{
				parser->expectToken(TokenType_e::ScopeResolution);
				parser->expectIdentifier();
				return nullptr;
			}

			// Processa expressao named
			if (parser->isIdentifier())
			{
				parser->expectIdentifier();
				return nullptr;
			}
		
			throw exceptions::unexpected_token_exception(
				parser->getTokenValue(),
				line,
				column
			);
		}

		// Processa algumas constantes
		switch (parser->getTokenType())
		{
		case TokenType_e::False:
		case TokenType_e::True:
			{
				auto constantBoolDecl = std::make_unique<ExpressionConstantBoolDecl>(line, column);
				constantBoolDecl->valueDecl = parser->getTokenType() == TokenType_e::True ? true : false;
				parser->nextToken();
				return constantBoolDecl;
			}
			break;
		case TokenType_e::ConstantInteger:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::I32;
				constantIntegerDecl->valueDecl = parser->expectConstantInteger();
				return constantIntegerDecl;
			}
			break;
		case TokenType_e::ConstantFp32:
			{
				auto constantRealDecl = std::make_unique<ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = TypeDeclID_e::Fp32;
				constantRealDecl->valueDecl = parser->expectConstantFp32();
				return constantRealDecl;
			}
			break;
		case TokenType_e::ConstantFp64:
			{
				auto constantRealDecl = std::make_unique<ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = TypeDeclID_e::Fp64;
				constantRealDecl->valueDecl = parser->expectConstantFp64();
				return constantRealDecl;
			}
			break;
		case TokenType_e::ConstantString:
			{
				auto constantStringDecl = std::make_unique<ExpressionConstantStringDecl>(line, column);
				constantStringDecl->valueDecl = parser->expectConstantString();
				return constantStringDecl;
			}
			break;
		case TokenType_e::ConstantChar:
			{
				auto constantCharDecl = std::make_unique<ExpressionConstantCharDecl>(line, column);
				constantCharDecl->valueDecl = parser->expectConstantChar();
				return constantCharDecl;
			}
			break;
		default:
			break;
		}

		// Resolucao de escopo unaria, indica acesso ao escopo global.
		if (parser->isScopeResolution())
		{
			// Consome '::'
			parser->expectToken(TokenType_e::ScopeResolution);

			auto namedExpressionDecl = std::make_unique<ExpressionConstantIdentifierDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
			namedExpressionDecl->identifierDecl = parser->expectIdentifier();
			namedExpressionDecl->startFromRoot = true;
			return namedExpressionDecl;
		}

		// Processa expressao named
		if (parser->isIdentifier())
		{
			auto namedExpressionDecl = std::make_unique<ExpressionConstantIdentifierDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
			namedExpressionDecl->identifierDecl = parser->expectIdentifier();
			return namedExpressionDecl;
		}
		
		throw exceptions::unexpected_token_exception(
			parser->getTokenValue(),
			line,
			column
		);
	}
} }