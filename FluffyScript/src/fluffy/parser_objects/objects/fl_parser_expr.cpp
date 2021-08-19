#include "../../fl_ast_expr.h"
#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy {
	/**
	 * isExprOperator
	 */

	bool isExprOperator(TokenSubType_e type)
	{
		switch (type) {
		case TokenSubType_e::Comma:
		case TokenSubType_e::Assign:
		case TokenSubType_e::LogicalOr:
		case TokenSubType_e::LogicalAnd:
		case TokenSubType_e::BitWiseOr:
		case TokenSubType_e::BitWiseXor:
		case TokenSubType_e::BitWiseAnd:
		case TokenSubType_e::BitWiseOrAssign:
		case TokenSubType_e::BitWiseXorAssign:
		case TokenSubType_e::BitWiseAndAssign:
		case TokenSubType_e::BitWiseRShiftAssign:
		case TokenSubType_e::BitWiseLShiftAssign:
		case TokenSubType_e::Equal:
		case TokenSubType_e::NotEqual:
		case TokenSubType_e::GreaterThan:
		case TokenSubType_e::LessThan:
		case TokenSubType_e::GreaterThanOrEqual:
		case TokenSubType_e::LessThanOrEqual:
		case TokenSubType_e::BitWiseLShift:
		case TokenSubType_e::BitWiseRShift:
		case TokenSubType_e::Plus:
		case TokenSubType_e::Minus:
		case TokenSubType_e::Multiplication:
		case TokenSubType_e::Division:
		case TokenSubType_e::Modulo:
		case TokenSubType_e::PlusAssign:
		case TokenSubType_e::MinusAssign:
		case TokenSubType_e::MultAssign:
		case TokenSubType_e::DivAssign:
		case TokenSubType_e::ModAssign:
		case TokenSubType_e::Increment:
		case TokenSubType_e::Decrement:
		case TokenSubType_e::As:
		case TokenSubType_e::Is:
		case TokenSubType_e::LSquBracket:
		case TokenSubType_e::LParBracket:
		case TokenSubType_e::Dot:
		case TokenSubType_e::BitWiseNot:
		case TokenSubType_e::LogicalNot:
		case TokenSubType_e::ScopeResolution:
		case TokenSubType_e::Interrogation:
		case TokenSubType_e::Fn:
		case TokenSubType_e::Match:
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
		TokenSubType_e	op;
		U32				prec;
		Bool			left;
	};

	static const TokenInfo_t op_info[] = {
			{ TokenSubType_e::Comma,				1, true },
			{ TokenSubType_e::Interrogation,		2, false },
			{ TokenSubType_e::Assign,				2, false },
			{ TokenSubType_e::PlusAssign,			2, false },
			{ TokenSubType_e::MinusAssign,			2, false },
			{ TokenSubType_e::MultAssign,			2, false },
			{ TokenSubType_e::DivAssign,			2, false },
			{ TokenSubType_e::ModAssign,			2, false },
			{ TokenSubType_e::BitWiseOrAssign,		2, false },
			{ TokenSubType_e::BitWiseXorAssign,		2, false },
			{ TokenSubType_e::BitWiseAndAssign,		2, false },
			{ TokenSubType_e::BitWiseLShiftAssign,	2, false },
			{ TokenSubType_e::BitWiseRShiftAssign,	2, false },
			{ TokenSubType_e::LogicalOr,			3, true },
			{ TokenSubType_e::LogicalAnd,			4, true },
			{ TokenSubType_e::BitWiseOr,			5, true },
			{ TokenSubType_e::BitWiseXor,			6, true },
			{ TokenSubType_e::BitWiseAnd,			7, true },
			{ TokenSubType_e::Equal,			    8, true },
			{ TokenSubType_e::NotEqual,			    8, true },
			{ TokenSubType_e::GreaterThan,		    9, true },
			{ TokenSubType_e::LessThan,			    9, true },
			{ TokenSubType_e::GreaterThanOrEqual,   9, true },
			{ TokenSubType_e::LessThanOrEqual,	    9, true },
			{ TokenSubType_e::BitWiseLShift,	   10, true },
			{ TokenSubType_e::BitWiseRShift,	   10, true },
			{ TokenSubType_e::Plus,				   11, true },   // Add
			{ TokenSubType_e::Minus,			   11, true },   // Sub
			{ TokenSubType_e::Multiplication,	   12, true },
			{ TokenSubType_e::Division,			   12, true },
			{ TokenSubType_e::Modulo,			   12, true },
			{ TokenSubType_e::Increment,		   13, false },  // Prefix
			{ TokenSubType_e::Decrement,		   13, false },  // Prefix
			{ TokenSubType_e::Plus,				   13, false },  // Unary positive
			{ TokenSubType_e::Minus,			   13, false },  // Unary negative
			{ TokenSubType_e::Match,			   13, false },
			{ TokenSubType_e::As,				   13, true },
			{ TokenSubType_e::Is,				   13, true },
			{ TokenSubType_e::LSquBracket,		   14, true },   // Array
			{ TokenSubType_e::LParBracket,		   14, true },   // Function Call
			{ TokenSubType_e::Increment,		   14, true },   // Postfix
			{ TokenSubType_e::Decrement,		   14, true },   // Postfix
			{ TokenSubType_e::Dot,				   14, true },
			{ TokenSubType_e::BitWiseNot,		   14, true },
			{ TokenSubType_e::LogicalNot,		   14, true },
			{ TokenSubType_e::ScopeResolution,	   15, true },
			{ TokenSubType_e::Unknown,			    0, true }
	};

	// Retorna informacoes sobre os operadores.
	const TokenInfo_t& getOperatorInfo(TokenSubType_e op)
	{
		U32 current_op = 0;
		while (true)
		{
			if (op_info[current_op].op == op)
			{
				return op_info[current_op];
			}
			if (op_info[current_op].op == TokenSubType_e::Unknown)
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

	ExpressionDeclPtr ParserObjectExpressionDecl::parse(CompilationContext_t* ctx)
	{
		return parseExpression(ctx, OperatorPrecLevel_e::MinPrec, false);
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::skip(CompilationContext_t* ctx)
	{
		const U32 line = ctx->parser->getTokenLine();
		const U32 column = ctx->parser->getTokenColumn();

		const U32 beginPosition = ctx->parser->getTokenPosition();

		parseExpression(ctx, OperatorPrecLevel_e::MinPrec, true);

		const U32 endPosition = ctx->parser->getTokenPosition();

		return std::make_unique<ExpressionDeclMark>(line, column, beginPosition, endPosition);
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::parseEnumExpr(CompilationContext_t* ctx)
	{
		return parseExpression(ctx, OperatorPrecLevel_e::EnumExpr, false);
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::skipEnumExpr(CompilationContext_t* ctx)
	{
		const U32 line = ctx->parser->getTokenLine();
		const U32 column = ctx->parser->getTokenColumn();

		const U32 beginPosition = ctx->parser->getTokenPosition();

		parseExpression(ctx, OperatorPrecLevel_e::EnumExpr, true);

		const U32 endPosition = ctx->parser->getTokenPosition();

		return std::make_unique<ExpressionDeclMark>(line, column, beginPosition, endPosition);
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::parseExpression(CompilationContext_t* ctx, U32 prec, Bool skipOnly)
	{
		ExpressionDeclPtr lhs;
		ExpressionDeclPtr rhs;

		const unsigned int line		= ctx->parser->getTokenLine();
		const unsigned int column	= ctx->parser->getTokenColumn();

		// Processa operadores unarios Prefixo.
		if (prec > OperatorPrecLevel_e::Unary) {
			switch (ctx->parser->getTokenSubType())
			{
			case TokenSubType_e::BitWiseNot:
			case TokenSubType_e::LogicalNot:
			case TokenSubType_e::Minus:
			case TokenSubType_e::Plus:
			case TokenSubType_e::Increment:
			case TokenSubType_e::Decrement:			
				{
					// Processa superficialmente
					if (skipOnly)
					{
						ctx->parser->nextToken();
						parseExpression(ctx, OperatorPrecLevel_e::Unary, skipOnly);
						return nullptr;
					}

					auto unaryExprDecl = std::make_unique<ExpressionUnaryDecl>(
						ctx->parser->getTokenLine(),
						ctx->parser->getTokenColumn()
					);

					unaryExprDecl->op = ctx->parser->getTokenSubType();
					unaryExprDecl->unaryType = ExpressionUnaryType_e::Prefix;

					// Consome operator.
					ctx->parser->nextToken();

					// Processa expressao a direita.
					unaryExprDecl->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::Unary, skipOnly);
					return unaryExprDecl;
				}
				break;
			case TokenSubType_e::Match:
				{
					// Processa superficialmente
					if (skipOnly)
					{
						ctx->parser->nextToken();
						parseExpression(ctx, OperatorPrecLevel_e::Unary, skipOnly);
						ctx->parser->expectToken(TokenSubType_e::LBracket);
						while (true)
						{
							ctx->parser->expectToken(TokenSubType_e::When);
							ParserObjectPatternDecl::parse(ctx);
							ctx->parser->expectToken(TokenSubType_e::Arrow);
							ParserObjectBlockDecl::parseExprBlock(ctx);
						}
						ctx->parser->expectToken(TokenSubType_e::RBracket);
						return nullptr;
					}

					auto matchExprDecl = std::make_unique<ExpressionMatchDecl>(
						ctx->parser->getTokenLine(),
						ctx->parser->getTokenColumn()
					);

					// Consome operator.
					ctx->parser->nextToken();

					// Processa expressao a direita.
					matchExprDecl->exprDecl = parseExpression(ctx, OperatorPrecLevel_e::Unary, skipOnly);

					// Consome '{'
					ctx->parser->expectToken(TokenSubType_e::LBracket);

					// Processa declaracao 'when'
					while (true)
					{
						auto whenDecl = std::make_unique<ExpressionMatchWhenDecl>();

						// Consome 'when'
						ctx->parser->expectToken(TokenSubType_e::When);

						// Consome pattern.
						whenDecl->patternDecl = ParserObjectPatternDecl::parse(ctx);

						// Consome '->'
						ctx->parser->expectToken(TokenSubType_e::Arrow);

						// Console o bloco de codigo de expressao.
						whenDecl->blockDecl = ParserObjectBlockDecl::parseExprBlock(ctx);
					}

					// Consome '}'
					ctx->parser->expectToken(TokenSubType_e::RBracket);

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
			return parseAtom(ctx, skipOnly);
		}

		// Processa expressao a esquerda.
		if (skipOnly) {
			parseExpression(ctx, prec + 1, skipOnly);
		} else {
			lhs = parseExpression(ctx, prec + 1, skipOnly);
		}

		while (true)
		{
			const TokenSubType_e op = ctx->parser->getTokenSubType();

			// Se nao e um operador de expressao sai do loop.
			if (!isExprOperator(ctx->parser->getTokenSubType()))
			{
				break;
			}

			// Processa operator unario posfixo.
			switch (ctx->parser->getTokenSubType())
			{
			case TokenSubType_e::Increment:
			case TokenSubType_e::Decrement:
				{
					// Processa superficialmente.
					if (skipOnly)
					{
						continue;
					}

					auto unaryExprDecl = std::make_unique<ExpressionUnaryDecl>(
						ctx->parser->getTokenLine(),
						ctx->parser->getTokenColumn()
					);

					unaryExprDecl->op = ctx->parser->getTokenSubType();
					unaryExprDecl->unaryType = ExpressionUnaryType_e::Posfix;
					unaryExprDecl->exprDecl = std::move(lhs);

					lhs = std::move(unaryExprDecl);
				}
				continue;
			default:
				break;
			}

			// Pega informacoes do operador.
			const auto& opInfo = getOperatorInfo(ctx->parser->getTokenSubType());

			// Retorna se precedencia dele e menor que a atual da funcao.
			if (opInfo.prec < prec) {
				break;
			}

			// Calcula proxima precedencia minima.
			U32 nextMinPrec = opInfo.left ? opInfo.prec + 1 : opInfo.prec;
			
			// Consome o operador.
			ctx->parser->nextToken();

			// Processa chamada de funcao ou indexacao de dados.
			switch (op)
			{
			case TokenSubType_e::LParBracket:
				{
					// Processa superficialmente.
					if (skipOnly)
					{
						if (ctx->parser->isRightParBracket())
						{
							ctx->parser->expectToken(TokenSubType_e::RParBracket);
							continue;
						}
						parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);
						ctx->parser->expectToken(TokenSubType_e::RParBracket);
						continue;
					}

					// Se nao ha parametros e uma expressao unaria posfixa f().
					if (ctx->parser->isRightParBracket())
					{
						// Consome ')'
						ctx->parser->expectToken(TokenSubType_e::RParBracket);

						auto functionCallExpr = std::make_unique<ExpressionFunctionCall>(
							line,
							column
						);

						functionCallExpr->leftDecl = std::move(lhs);

						lhs = std::move(functionCallExpr);
						continue;
					}

					auto functionCallExpr = std::make_unique<ExpressionFunctionCall>(
						line,
						column
					);

					functionCallExpr->leftDecl = std::move(lhs);
					functionCallExpr->rightDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);

					// Consome ')'
					ctx->parser->expectToken(TokenSubType_e::RParBracket);

					lhs = std::move(functionCallExpr);
					continue;
				}
				break;
			case TokenSubType_e::LSquBracket:
				{
					// Processa superficialmente.
					if (skipOnly)
					{
						parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);
						ctx->parser->expectToken(TokenSubType_e::RSquBracket);
					}

					auto indexAddressExpr = std::make_unique<ExpressionIndexAddress>(
						line,
						column
					);

					indexAddressExpr->leftDecl = std::move(lhs);
					indexAddressExpr->rightDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);

					// Consome ']'
					ctx->parser->expectToken(TokenSubType_e::RSquBracket);

					lhs = std::move(indexAddressExpr);
					continue;
				}
				break;
			default:
				break;
			}

			// Processa superficialmente.
			if (skipOnly) {
				// Processa expressao a direira.
				parseExpression(ctx, nextMinPrec, skipOnly);
			} else {
				// Processa expressao a direira.
				rhs = parseExpression(ctx, nextMinPrec, skipOnly);
			}

			// Verifica se a operacao e ternaria
			if (op == TokenSubType_e::Interrogation)
			{
				// Processa superficialmente.
				if (skipOnly) {
					ctx->parser->expectToken(TokenSubType_e::Colon);
					parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);
					break;
				}

				auto ternaryExprDecl = std::make_unique<ExpressionTernaryDecl>(
					line,
					column
				);

				// Consome ':'
				ctx->parser->expectToken(TokenSubType_e::Colon);

				ternaryExprDecl->conditionDecl = std::move(lhs);
				ternaryExprDecl->leftDecl = std::move(rhs);
				ternaryExprDecl->rightDecl = parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);

				lhs = std::move(ternaryExprDecl);
				break;
			}

			// Processa superficialmente.
			if (skipOnly) {
				continue;
			}

			// Processa operador binario.
			auto binaryExprDecl = std::make_unique<ExpressionBinaryDecl>(
				line,
				column
			);

			binaryExprDecl->op = op;
			binaryExprDecl->leftDecl = std::move(lhs);
			binaryExprDecl->rightDecl = std::move(rhs);

			lhs = std::move(binaryExprDecl);
		}
		return lhs;
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::parseAtom(CompilationContext_t* ctx, Bool skipOnly)
	{
		const U32 line = ctx->parser->getTokenLine();
		const U32 column = ctx->parser->getTokenColumn();

		// Processa expressao entre parenteses.
		if (ctx->parser->isLeftParBracket())
		{
			// Processa superficialmente.
			if (skipOnly) {
				ctx->parser->expectToken(TokenSubType_e::LParBracket);
				parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);
				ctx->parser->expectToken(TokenSubType_e::RParBracket);
				return nullptr;
			}

			// Consome '('
			ctx->parser->expectToken(TokenSubType_e::LParBracket);

			// Processa expressao.
			auto expr = parseExpression(ctx, OperatorPrecLevel_e::MinPrec, skipOnly);

			// Ajusta a posicao inicial
			expr->line = line;
			expr->column = column;

			// Consome ')'
			ctx->parser->expectToken(TokenSubType_e::RParBracket);

			return expr;
		}

		// Processa declaracao de funcao anonima.
		if (ctx->parser->isFn())
		{
			// Processa superficialmente.
			if (skipOnly) {
				ctx->parser->expectToken(TokenSubType_e::Fn);
				ctx->parser->expectToken(TokenSubType_e::LParBracket);
				if (ctx->parser->isIdentifier()) {
					auto identifier = ctx->parser->expectIdentifier();
					if (ctx->parser->isColon())
					{
						ctx->parser->expectToken(TokenSubType_e::Colon);

						U32 paramLine = ctx->parser->getTokenLine();
						U32 paramColumn = ctx->parser->getTokenColumn();

						// Parametro nao podem ter tipo nulo.
						if (ParserObjectTypeDecl::parse(ctx)->typeID == TypeDeclID_e::Void)
						{
							throw exceptions::custom_exception(
								"Parameter '%s' can't have void type",
								paramLine,
								paramColumn,
								identifier.c_str()
							);
						}

						while (true)
						{
							if (ctx->parser->isRightParBracket())
							{
								break;
							}
							ctx->parser->expectToken(TokenSubType_e::Comma);

							U32 paramLine = ctx->parser->getTokenLine();
							U32 paramColumn = ctx->parser->getTokenColumn();

							auto identifierDecl = ctx->parser->expectIdentifier();

							ctx->parser->expectToken(TokenSubType_e::Colon);

							// Parametro nao podem ter tipo nulo.
							if (ParserObjectTypeDecl::parse(ctx)->typeID == TypeDeclID_e::Void)
							{
								throw exceptions::custom_exception(
									"Parameter '%s' can't have void type",
									paramLine,
									paramColumn,
									identifier.c_str()
								);
							}
						}
					} else {
						while (true)
						{
							if (ctx->parser->isRightParBracket())
							{
								break;
							}

							// Consome ','
							ctx->parser->expectToken(TokenSubType_e::Comma);
							ctx->parser->expectIdentifier();
						}
					}
				}

				// Consome ')'
				ctx->parser->expectToken(TokenSubType_e::RParBracket);

				// Verifica se o tipo de retorno e explicito.
				if (ctx->parser->isArrow()) {
					// Consome '->'
					ctx->parser->expectToken(TokenSubType_e::Arrow);
					ParserObjectTypeDecl::parse(ctx);
				}

				// Consome '{'
				ctx->parser->expectToken(TokenSubType_e::LBracket);

				// Consome o block de codigo.
				if (!ctx->parser->isRightBracket())
				{
					ParserObjectBlockDecl::skipBlockOrExprBlock(ctx);
				}

				// Consome '}'
				ctx->parser->expectToken(TokenSubType_e::RBracket);
				return nullptr;
			}

			auto functionDecl = std::make_unique<ExpressionFunctionDecl>(
				line,
				column
			);

			// Consome 'fn'
			ctx->parser->expectToken(TokenSubType_e::Fn);

			// Consome '('
			ctx->parser->expectToken(TokenSubType_e::LParBracket);

			// Uma funcao anonima pode ter parametros tipados ou nao, depende do contexto.
			// Por exemplo se a funcao esta sendo passada como parametro, seus tipos podem ser
			// deduzidos com base no tipo do parametro, o mesmo pode ocorrer na declaracao de variaveis
			// ou ainda atribui-la a um membro de classe, struct.
			// Porem ha casos onde a tipagem de faz necessaria por exemplo ao usar uma declaracao de variavel
			// ou constate onde o tipo e omitido, logo, nao se poderia fazer a deducao.

			if (ctx->parser->isIdentifier()) {

				// Consome o identificador.
				auto identifier = ctx->parser->expectIdentifier();

				// Verifica se o proximo token e ':'
				if (ctx->parser->isColon())
				{
					// Consome ':'
					ctx->parser->expectToken(TokenSubType_e::Colon);

					U32 paramLine = ctx->parser->getTokenLine();
					U32 paramColumn = ctx->parser->getTokenColumn();

					auto paramDecl = std::make_unique<ExpressionFunctionParameterDecl>(
						paramLine,
						paramColumn
					);

					paramDecl->identifierDecl = identifier;
					paramDecl->typeDecl = ParserObjectTypeDecl::parse(ctx);

					// Parametro nao podem ter tipo nulo.
					if (paramDecl->typeDecl->typeID == TypeDeclID_e::Void)
					{
						throw exceptions::custom_exception(
							"Parameter '%s' can't have void type",
							paramLine,
							paramColumn,
							identifier.c_str()
						);
					}

					// Adiciona o parametro a lista
					functionDecl->parametersDecl.push_back(std::move(paramDecl));

					// Processa o restante do parametros se houver.
					while (true)
					{
						if (ctx->parser->isRightParBracket())
						{
							break;
						}

						// Consome ','
						ctx->parser->expectToken(TokenSubType_e::Comma);

						U32 paramLine = ctx->parser->getTokenLine();
						U32 paramColumn = ctx->parser->getTokenColumn();

						auto paramDecl = std::make_unique<ExpressionFunctionParameterDecl>(
							paramLine,
							paramColumn
						);

						paramDecl->identifierDecl = ctx->parser->expectIdentifier();

						// Consome ':'
						ctx->parser->expectToken(TokenSubType_e::Colon);

						paramDecl->typeDecl = ParserObjectTypeDecl::parse(ctx);

						// Parametro nao podem ter tipo nulo.
						if (paramDecl->typeDecl->typeID == TypeDeclID_e::Void)
						{
							throw exceptions::custom_exception(
								"Parameter '%s' can't have void type",
								paramLine,
								paramColumn,
								identifier.c_str()
							);
						}

						// Adiciona o parametro a lista
						functionDecl->parametersDecl.push_back(std::move(paramDecl));
					}
				} else {
					auto paramDecl = std::make_unique<ExpressionFunctionParameterDecl>(
						ctx->parser->getTokenLine(),
						ctx->parser->getTokenColumn()
					);

					paramDecl->identifierDecl = identifier;

					// Adiciona o parametro a lista
					functionDecl->parametersDecl.push_back(std::move(paramDecl));

					// Processa o restante do parametros se houver.
					while (true)
					{
						if (ctx->parser->isRightParBracket())
						{
							break;
						}

						// Consome ','
						ctx->parser->expectToken(TokenSubType_e::Comma);

						auto paramDecl = std::make_unique<ExpressionFunctionParameterDecl>(
							ctx->parser->getTokenLine(),
							ctx->parser->getTokenColumn()
						);

						paramDecl->identifierDecl = ctx->parser->expectIdentifier();

						// Adiciona o parametro a lista
						functionDecl->parametersDecl.push_back(std::move(paramDecl));
					}
				}
			}

			// Consome ')'
			ctx->parser->expectToken(TokenSubType_e::RParBracket);

			// Verifica se o tipo de retorno e explicito.
			if (ctx->parser->isArrow()) {
				// Consome '->'
				ctx->parser->expectToken(TokenSubType_e::Arrow);

				functionDecl->returnTypeDecl = ParserObjectTypeDecl::parse(ctx);
			} else {
				functionDecl->returnTypeDecl = std::make_unique<ast::TypeDeclVoid>(
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn()
				);
			}

			// Consome '{'
			ctx->parser->expectToken(TokenSubType_e::LBracket);

			// Consome o block de codigo.
			if (!ctx->parser->isRightBracket())
			{
				functionDecl->blockDecl = ParserObjectBlockDecl::parseBlockOrExprBlock(ctx);
			}

			// Consome '}'
			ctx->parser->expectToken(TokenSubType_e::RBracket);

			return functionDecl;
		}

		// Processa this.
		if (ctx->parser->isThis())
		{
			// Processa superficialmente.
			if (skipOnly) {
				ctx->parser->expectToken(TokenSubType_e::This);
				return nullptr;
			}

			// Consome 'this'
			ctx->parser->expectToken(TokenSubType_e::This);

			return std::make_unique<ExpressionThisDecl>(line,column);
		}

		// Processa super.
		if (ctx->parser->isSuper())
		{
			// Processa superficialmente.
			if (skipOnly) {
				ctx->parser->expectToken(TokenSubType_e::Super);
				return nullptr;
			}

			// Consome 'super'
			ctx->parser->expectToken(TokenSubType_e::Super);

			return std::make_unique<ExpressionSuperDecl>(line, column);
		}

		// Processa null.
		if (ctx->parser->isNull())
		{
			// Processa superficialmente.
			if (skipOnly) {
				ctx->parser->expectToken(TokenSubType_e::Null);
				return nullptr;
			}

			// Consome 'null'
			ctx->parser->expectToken(TokenSubType_e::Null);

			return std::make_unique<ExpressionConstantNullDecl>(line, column);
		}

		// Processa superficialmente.
		if (skipOnly) {
			// Processa algumas constantes
			switch (ctx->parser->getTokenSubType())
			{
			case TokenSubType_e::False:
			case TokenSubType_e::True:
				{
					ctx->parser->nextToken();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantI8:
				{
					ctx->parser->expectConstantI8();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantU8:
				{
					ctx->parser->expectConstantU8();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantI16:
				{
					ctx->parser->expectConstantI16();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantU16:
				{
					ctx->parser->expectConstantU16();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantI32:
				{
					ctx->parser->expectConstantI32();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantU32:
				{
					ctx->parser->expectConstantU32();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantI64:
				{
					ctx->parser->expectConstantI64();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantU64:
				{
					ctx->parser->expectConstantU64();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantFp32:
				{
					ctx->parser->expectConstantFp32();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantFp64:
				{
					ctx->parser->expectConstantFp64();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantString:
				{
					ctx->parser->expectConstantString();
					return nullptr;
				}
				break;
			case TokenSubType_e::ConstantChar:
				{
					ctx->parser->expectConstantChar();
					return nullptr;
				}
				break;
			default:
				break;
			}

			// Resolucao de escopo unaria, indica acesso ao escopo global.
			if (ctx->parser->isScopeResolution())
			{
				ctx->parser->expectToken(TokenSubType_e::ScopeResolution);
				ctx->parser->expectIdentifier();
				return nullptr;
			}

			// Processa expressao named
			if (ctx->parser->isIdentifier())
			{
				ctx->parser->expectIdentifier();
				return nullptr;
			}
		
			throw exceptions::unexpected_token_exception(
				ctx->parser->getTokenValue(),
				line,
				column
			);
		}

		// Processa algumas constantes
		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::False:
		case TokenSubType_e::True:
			{
				auto constantBoolDecl = std::make_unique<ExpressionConstantBoolDecl>(line, column);
				constantBoolDecl->valueDecl = ctx->parser->getTokenSubType() == TokenSubType_e::True ? true : false;
				ctx->parser->nextToken();
				return constantBoolDecl;
			}
			break;
		case TokenSubType_e::ConstantI8:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::I8;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantI8();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU8:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::U8;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantU8();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantI16:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::I16;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantI16();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU16:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::U16;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantU16();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantI32:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::I32;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantI32();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU32:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::U32;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantU32();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantI64:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::I64;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantI64();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU64:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = TypeDeclID_e::U64;
				constantIntegerDecl->valueDecl = ctx->parser->expectConstantU64();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantFp32:
			{
				auto constantRealDecl = std::make_unique<ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = TypeDeclID_e::Fp32;
				constantRealDecl->valueDecl = ctx->parser->expectConstantFp32();
				return constantRealDecl;
			}
			break;
		case TokenSubType_e::ConstantFp64:
			{
				auto constantRealDecl = std::make_unique<ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = TypeDeclID_e::Fp64;
				constantRealDecl->valueDecl = ctx->parser->expectConstantFp64();
				return constantRealDecl;
			}
			break;
		case TokenSubType_e::ConstantString:
			{
				auto constantStringDecl = std::make_unique<ExpressionConstantStringDecl>(line, column);
				constantStringDecl->valueDecl = ctx->parser->expectConstantString();
				return constantStringDecl;
			}
			break;
		case TokenSubType_e::ConstantChar:
			{
				auto constantCharDecl = std::make_unique<ExpressionConstantCharDecl>(line, column);
				constantCharDecl->valueDecl = ctx->parser->expectConstantChar();
				return constantCharDecl;
			}
			break;
		default:
			break;
		}

		// Resolucao de escopo unaria, indica acesso ao escopo global.
		if (ctx->parser->isScopeResolution())
		{
			// Consome '::'
			ctx->parser->expectToken(TokenSubType_e::ScopeResolution);

			auto namedExpressionDecl = std::make_unique<ExpressionConstantIdentifierDecl>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			namedExpressionDecl->identifierDecl = ctx->parser->expectIdentifier();
			namedExpressionDecl->startFromRoot = true;
			return namedExpressionDecl;
		}

		// Processa expressao named
		if (ctx->parser->isIdentifier())
		{
			auto namedExpressionDecl = std::make_unique<ExpressionConstantIdentifierDecl>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			namedExpressionDecl->identifierDecl = ctx->parser->expectIdentifier();
			return namedExpressionDecl;
		}
		
		throw exceptions::unexpected_token_exception(
			ctx->parser->getTokenValue(),
			line,
			column
		);
	}
} }