#include "../../fl_ast_expr.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy {
	/**
	 * OperatorPrecLevel_e
	 */

	enum OperatorPrecLevel_e : U32
	{
		MinPrec		= 1,
		SizeOf		= 2,
		Unary		= 12,
		CaseExpr	= 14,
		Max			= 15
	};

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
		case TokenSubType_e::SizeOf:
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

	ExpressionDeclPtr ParserObjectExpressionDecl::parse(Parser* parser)
	{
		return parseExpression(parser, OperatorPrecLevel_e::MinPrec);
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::parseExpression(Parser* parser, U32 prec)
	{
		ExpressionDeclPtr lhs;
		ExpressionDeclPtr rhs;

		const unsigned int line		= parser->getTokenLine();
		const unsigned int column	= parser->getTokenColumn();

		// Processa operadores unarios Prefixo.
		if (prec > OperatorPrecLevel_e::Unary) {
			switch (parser->getTokenSubType())
			{
			case TokenSubType_e::BitWiseNot:
			case TokenSubType_e::LogicalNot:
			case TokenSubType_e::Minus:
			case TokenSubType_e::Plus:
			case TokenSubType_e::Increment:
			case TokenSubType_e::Decrement:			
				{
					auto unaryExprDecl = std::make_unique<ExpressionUnaryDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					);

					unaryExprDecl->op = parser->getTokenSubType();
					unaryExprDecl->unaryType = ExpressionUnaryType_e::Prefix;

					// Consome operator.
					parser->nextToken();

					// Processa expressao a direita.
					unaryExprDecl->exprDecl = parseExpression(parser, OperatorPrecLevel_e::Unary);
					return unaryExprDecl;
				}
				break;
			case TokenSubType_e::Match:
				{
					auto matchExprDecl = std::make_unique<ExpressionMatchDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					);

					// Consome operator.
					parser->nextToken();

					// Processa expressao a direita.
					matchExprDecl->exprDecl = parseExpression(parser, OperatorPrecLevel_e::Unary);

					// Consome '{'
					parser->expectToken([parser]() { return parser->isLeftBracket(); });

					// Processa declaracao 'when'
					while (true)
					{
						auto whenDecl = std::make_unique<ExpressionMatchWhenDecl>();

						// Consome 'when'
						parser->expectToken([parser]() { return parser->isWhen(); });

						// Consome pattern.
						whenDecl->patternDecl = ParserObjectPatternDecl::parse(parser);

						// Consome '->'
						parser->expectToken([parser]() { return parser->isArrow(); });

						// Console o bloco de codigo de expressao.
						whenDecl->blockDecl = ParserObjectBlockDecl::parseExprBlock(parser);
					}

					// Consome '}'
					parser->expectToken([parser]() { return parser->isRightBracket(); });

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
			return parseAtom(parser);
		}

		// Processa expressao a esquerda.
		lhs = parseExpression(parser, prec + 1);

		while (true)
		{
			const TokenSubType_e op = parser->getTokenSubType();

			// Se nao e um operador de expressao sai do loop.
			if (!isExprOperator(parser->getTokenSubType()))
			{
				break;
			}

			// Processa operator unario posfixo.
			switch (parser->getTokenSubType())
			{
			case TokenSubType_e::Increment:
			case TokenSubType_e::Decrement:
				{
					auto unaryExprDecl = std::make_unique<ExpressionUnaryDecl>(
						parser->getTokenLine(),
						parser->getTokenColumn()
					);

					unaryExprDecl->op = parser->getTokenSubType();
					unaryExprDecl->unaryType = ExpressionUnaryType_e::Posfix;
					unaryExprDecl->exprDecl = std::move(lhs);

					lhs = std::move(unaryExprDecl);
				}
				continue;
			default:
				break;
			}

			// Pega informacoes do operador.
			const auto& opInfo = getOperatorInfo(parser->getTokenSubType());

			// Retorna se precedencia dele e menor que a atual da funcao.
			if (opInfo.prec < prec) {
				break;
			}

			// Calcula proxima precedencia minima.
			U32 nextMinPrec = opInfo.left ? opInfo.prec + 1 : opInfo.prec;
			
			// Consome o operador.
			parser->nextToken();
		}
		return lhs;
	}

	ExpressionDeclPtr ParserObjectExpressionDecl::parseAtom(Parser* parser)
	{
		return nullptr;
	}
} }