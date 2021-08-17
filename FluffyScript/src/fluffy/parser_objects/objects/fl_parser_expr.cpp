#include "../../fl_ast_expr.h"
#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy {
	/**
	 * OperatorPrecLevel_e
	 */

	enum OperatorPrecLevel_e : U32
	{
		MinPrec			= 1,
		Interrogation	= 2,
		Unary			= 12,
		Increment		= 14,
		Max				= 15
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

			// Processa chamada de funcao ou indexacao de dados.
			switch (op)
			{
			case TokenSubType_e::LParBracket:
				{
					// Se nao ha parametros e uma expressao unaria posfixa f().
					if (parser->isRightParBracket())
					{
						// Consome ')'
						parser->expectToken([parser]() { return parser->isRightParBracket(); });

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
					functionCallExpr->rightDecl = parseExpression(parser, OperatorPrecLevel_e::MinPrec);

					// Consome ')'
					parser->expectToken([parser]() { return parser->isRightParBracket(); });

					lhs = std::move(functionCallExpr);
					continue;
				}
				break;
			case TokenSubType_e::LSquBracket:
				{
					auto indexAddressExpr = std::make_unique<ExpressionIndexAddress>(
						line,
						column
					);

					indexAddressExpr->leftDecl = std::move(lhs);
					indexAddressExpr->rightDecl = parseExpression(parser, OperatorPrecLevel_e::MinPrec);

					// Consome ']'
					parser->expectToken([parser]() { return parser->isRightSquBracket(); });

					lhs = std::move(indexAddressExpr);
					continue;
				}
				break;
			default:
				break;
			}

			// Processa expressao a direira.
			rhs = parseExpression(parser, nextMinPrec);

			// Verifica se a operacao e ternaria
			if (op == TokenSubType_e::Interrogation)
			{
				auto ternaryExprDecl = std::make_unique<ExpressionTernaryDecl>(
					line,
					column
				);

				// Consome ':'
				parser->expectToken([parser]() { return parser->isColon(); });

				ternaryExprDecl->conditionDecl = std::move(lhs);
				ternaryExprDecl->leftDecl = std::move(rhs);
				ternaryExprDecl->rightDecl = parseExpression(parser, OperatorPrecLevel_e::MinPrec);

				lhs = std::move(ternaryExprDecl);
				break;
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

	ExpressionDeclPtr ParserObjectExpressionDecl::parseAtom(Parser* parser)
	{
		const U32 line = parser->getTokenLine();
		const U32 column = parser->getTokenColumn();

		// Processa expressao entre parenteses.
		if (parser->isLeftParBracket())
		{
			// Consome '('
			parser->expectToken([parser]() { return parser->isLeftParBracket(); });

			// Processa expressao.
			auto expr = parseExpression(parser, OperatorPrecLevel_e::MinPrec);

			// Ajusta a posicao inicial
			expr->line = line;
			expr->column = column;

			// Consome ')'
			parser->expectToken([parser]() { return parser->isRightParBracket(); });

			return expr;
		}

		// Processa declaracao de funcao anonima.
		if (parser->isFn())
		{
			auto functionDecl = std::make_unique<ExpressionFunctionDecl>(
				line,
				column
			);

			// Consome 'fn'
			parser->expectToken([parser]() { return parser->isFn(); });

			// Consome '('
			parser->expectToken([parser]() { return parser->isLeftParBracket(); });

			// Uma funcao anonima pode ter parametros tipados ou nao, depende do contexto.
			// Por exemplo se a funcao esta sendo passada como parametro, seus tipos podem ser
			// deduzidos com base no tipo do parametro, o mesmo pode ocorrer na declaracao de variaveis
			// ou ainda atribui-la a um membro de classe, struct.
			// Porem ha casos onde a tipagem de faz necessaria por exemplo ao usar uma declaracao de variavel
			// ou constate onde o tipo e omitido, logo, nao se poderia fazer a deducao.

			if (parser->isIdentifier()) {

				// Consome o identificador.
				auto identifier = parser->expectIdentifier();

				// Verifica se o proximo token e ':'
				if (parser->isColon())
				{
					// Consome ':'
					parser->expectToken([parser]() { return parser->isColon(); });

					U32 paramLine = parser->getTokenLine();
					U32 paramColumn = parser->getTokenColumn();

					auto paramDecl = std::make_unique<ExpressionFunctionParameterDecl>(
						paramLine,
						paramColumn
					);

					paramDecl->identifierDecl = identifier;
					paramDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

					// Parametro nao podem ter tipo nulo.
					if (paramDecl->typeDecl->typeID == ast::TypeDeclID_e::Void)
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
						if (parser->isRightParBracket())
						{
							break;
						}

						// Consome ','
						parser->expectToken([parser]() { return parser->isComma(); });

						U32 paramLine = parser->getTokenLine();
						U32 paramColumn = parser->getTokenColumn();

						auto paramDecl = std::make_unique<ExpressionFunctionParameterDecl>(
							paramLine,
							paramColumn
						);

						paramDecl->identifierDecl = parser->expectIdentifier();

						// Consome ':'
						parser->expectToken([parser]() { return parser->isColon(); });

						paramDecl->typeDecl = ParserObjectTypeDecl::parse(parser);

						// Parametro nao podem ter tipo nulo.
						if (paramDecl->typeDecl->typeID == ast::TypeDeclID_e::Void)
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
						parser->getTokenLine(),
						parser->getTokenColumn()
					);

					paramDecl->identifierDecl = identifier;

					// Adiciona o parametro a lista
					functionDecl->parametersDecl.push_back(std::move(paramDecl));

					// Processa o restante do parametros se houver.
					while (true)
					{
						if (parser->isRightParBracket())
						{
							break;
						}

						// Consome ','
						parser->expectToken([parser]() { return parser->isComma(); });

						auto paramDecl = std::make_unique<ExpressionFunctionParameterDecl>(
							parser->getTokenLine(),
							parser->getTokenColumn()
						);

						paramDecl->identifierDecl = parser->expectIdentifier();

						// Adiciona o parametro a lista
						functionDecl->parametersDecl.push_back(std::move(paramDecl));
					}
				}
			}

			// Consome ')'
			parser->expectToken([parser]() { return parser->isRightParBracket(); });

			// Verifica se o tipo de retorno e explicito.
			if (parser->isArrow()) {
				// Consome '->'
				parser->expectToken([parser]() { return parser->isArrow(); });

				functionDecl->returnTypeDecl = ParserObjectTypeDecl::parse(parser);
			} else {
				functionDecl->returnTypeDecl = std::make_unique<ast::TypeDeclVoid>();
			}

			// Consome '{'
			parser->expectToken([parser]() { return parser->isLeftBracket(); });

			// Consome o block de codigo.
			if (!parser->isRightBracket())
			{
				functionDecl->blockDecl = ParserObjectBlockDecl::parseBlockOrExprBlock(parser);
			}

			// Consome '}'
			parser->expectToken([parser]() { return parser->isRightBracket(); });

			return functionDecl;
		}

		// Processa this.
		if (parser->isThis())
		{
			// Consome 'this'
			parser->expectToken([parser]() { return parser->isThis(); });

			return std::make_unique<ExpressionThisDecl>(line,column);
		}

		// Processa super.
		if (parser->isSuper())
		{
			// Consome 'super'
			parser->expectToken([parser]() { return parser->isSuper(); });

			return std::make_unique<ExpressionSuperDecl>(line, column);
		}

		// Processa null.
		if (parser->isNull())
		{
			// Consome 'null'
			parser->expectToken([parser]() { return parser->isNull(); });

			return std::make_unique<ExpressionConstantNullDecl>(line, column);
		}

		// Processa algumas constantes
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::False:
		case TokenSubType_e::True:
			{
				auto constantBoolDecl = std::make_unique<ExpressionConstantBoolDecl>(line, column);
				constantBoolDecl->valueDecl = parser->getTokenSubType() == TokenSubType_e::True ? true : false;
				parser->nextToken();
				return constantBoolDecl;
			}
			break;
		case TokenSubType_e::ConstantI8:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::I8;
				constantIntegerDecl->valueDecl = parser->expectConstantI8();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU8:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::U8;
				constantIntegerDecl->valueDecl = parser->expectConstantU8();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantI16:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::I16;
				constantIntegerDecl->valueDecl = parser->expectConstantI16();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU16:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::U16;
				constantIntegerDecl->valueDecl = parser->expectConstantU16();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantI32:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::I32;
				constantIntegerDecl->valueDecl = parser->expectConstantI32();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU32:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::U32;
				constantIntegerDecl->valueDecl = parser->expectConstantU32();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantI64:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::I64;
				constantIntegerDecl->valueDecl = parser->expectConstantI64();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantU64:
			{
				auto constantIntegerDecl = std::make_unique<ExpressionConstantIntegerDecl>(line, column);
				constantIntegerDecl->valueType = ast::TypeDeclID_e::U64;
				constantIntegerDecl->valueDecl = parser->expectConstantU64();
				return constantIntegerDecl;
			}
			break;
		case TokenSubType_e::ConstantFp32:
			{
				auto constantRealDecl = std::make_unique<ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = ast::TypeDeclID_e::Fp32;
				constantRealDecl->valueDecl = parser->expectConstantFp32();
				return constantRealDecl;
			}
			break;
		case TokenSubType_e::ConstantFp64:
			{
				auto constantRealDecl = std::make_unique<ExpressionConstantRealDecl>(line, column);
				constantRealDecl->valueType = ast::TypeDeclID_e::Fp64;
				constantRealDecl->valueDecl = parser->expectConstantFp64();
				return constantRealDecl;
			}
			break;
		case TokenSubType_e::ConstantString:
			{
				auto constantStringDecl = std::make_unique<ExpressionConstantStringDecl>(line, column);
				constantStringDecl->valueDecl = parser->expectConstantString();
				return constantStringDecl;
			}
			break;
		case TokenSubType_e::ConstantChar:
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
			parser->expectToken([parser]() { return parser->isScopeResolution(); });

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