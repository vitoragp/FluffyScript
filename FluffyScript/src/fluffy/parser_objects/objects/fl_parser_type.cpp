#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectTypeDecl
	 */

	TypeDeclPtr ParserObjectTypeDecl::parse(CompilationContext_t* ctx)
	{
		return parseType(ctx, false);
	}
	
	TypeDeclPtr ParserObjectTypeDecl::parseWithSelf(CompilationContext_t* ctx)
	{
		return parseType(ctx, true);
	}

	TypeDeclPtr ParserObjectTypeDecl::parseType(CompilationContext_t* ctx, Bool traitMode)
	{
		TypeDeclPtr typeDecl;

		const U32 line = ctx->parser->getTokenLine();
		const U32 column = ctx->parser->getTokenColumn();

		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::Void:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclVoid>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::I8:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI8>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::U8:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU8>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::I16:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI16>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::U16:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU16>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::I32:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI32>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::U32:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU32>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::I64:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI64>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::U64:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU64>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::Fp32:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclFp32>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::Fp64:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclFp64>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::String:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclString>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::Object:
			ctx->parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclObject>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::Vector:
			typeDecl = ParserObjectTypeDecl::parseVectorType(ctx, traitMode);
			break;
		case TokenSubType_e::Set:
			typeDecl = ParserObjectTypeDecl::parseSetType(ctx, traitMode);
			break;
		case TokenSubType_e::Map:
			typeDecl = ParserObjectTypeDecl::parseMapType(ctx, traitMode);
			break;
		case TokenSubType_e::Fn:
			typeDecl = ParserObjectTypeDecl::parseFunctionType(ctx, traitMode);
			break;
		case TokenSubType_e::LParBracket:
			typeDecl = ParserObjectTypeDecl::parseTupleType(ctx, traitMode);
			break;
		case TokenSubType_e::Self:
			if (traitMode)
			{
				ctx->parser->nextToken();
				typeDecl = std::make_unique<ast::TraitSelfTypeDecl>(
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn()
					);
			}
			else
			{
				throw exceptions::custom_exception(
					"Self type only can be declared in traits",
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn()
				);
			}
			break;
		default:
			// NamedType
			if (ctx->parser->isIdentifier() || ctx->parser->isScopeResolution())
			{
				typeDecl = ParserObjectTypeDecl::parseNamedType(ctx, traitMode);
				break;
			}

			throw exceptions::unexpected_token_exception(
				ctx->parser->getTokenValue(),
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Verifica se o tipo e anulavel.
		if (ctx->parser->isInterrogation())
		{
			if (typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception("'void' type can't be nullable",
					line,
					column
				);
			}

			// Consome '?'
			ctx->parser->expectToken(TokenSubType_e::Interrogation);

			typeDecl->nullable = true;
		}

		// Verifica se e um array
		if (ctx->parser->isLeftSquBracket())
		{
			auto arrayType = std::make_unique<ast::TypeDeclArray>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);

			// Adiciona o tipo base ao tipo array
			arrayType->valueType = std::move(typeDecl);

			while (true)
			{
				if (!ctx->parser->isLeftSquBracket())
				{
					break;
				}
				arrayType->arrayDeclList.push_back(ParserObjectTypeDecl::parseArrayDecl(ctx, traitMode));
			}

			// Verifica se o array e anulavel.
			if (ctx->parser->isInterrogation())
			{
				// Consome '?'
				ctx->parser->expectToken(TokenSubType_e::Interrogation);

				arrayType->nullable = true;
			}
			typeDecl = std::move(arrayType);
		}
		return typeDecl;
	}


	TypeDeclPtr ParserObjectTypeDecl::parseVectorType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto vectorTypeDecl = std::make_unique<ast::TypeDeclVector>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'vector'
		ctx->parser->expectToken(TokenSubType_e::Vector);

		// Consome '<'
		ctx->parser->expectToken(TokenSubType_e::LessThan);

		// Consome o tipo do valor
		vectorTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(ctx, traitMode);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (ctx->parser->isBitWiseRightShift() || ctx->parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(ctx);
		}

		// Consome '>'
		ctx->parser->expectToken(TokenSubType_e::GreaterThan);

		return vectorTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseSetType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto setTypeDecl = std::make_unique<ast::TypeDeclSet>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'set'
		ctx->parser->expectToken(TokenSubType_e::Set);

		// Consome '<'
		ctx->parser->expectToken(TokenSubType_e::LessThan);

		// Consome o tipo do valor
		setTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(ctx, traitMode);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (ctx->parser->isBitWiseRightShift() || ctx->parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(ctx);
		}

		// Consome '>'
		ctx->parser->expectToken(TokenSubType_e::GreaterThan);

		return setTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseMapType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto mapTypeDecl = std::make_unique<ast::TypeDeclMap>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'map'
		ctx->parser->expectToken(TokenSubType_e::Map);

		// Consome '<'
		ctx->parser->expectToken(TokenSubType_e::LessThan);

		// Consome o tipo da chave.
		mapTypeDecl->keyType = ParserObjectTypeDecl::parseVariableType(ctx, traitMode);

		// Consome ','
		ctx->parser->expectToken(TokenSubType_e::Comma);

		// Consome o tipo do valor
		mapTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(ctx, traitMode);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (ctx->parser->isBitWiseRightShift() || ctx->parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(ctx);
		}

		// Consome '>'
		ctx->parser->expectToken(TokenSubType_e::GreaterThan);

		return mapTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseFunctionType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto functionTypeDecl = std::make_unique<ast::TypeDeclFunction>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome 'fn'
		ctx->parser->expectToken(TokenSubType_e::Fn);

		// Consome '('
		ctx->parser->expectToken(TokenSubType_e::LParBracket);

		Bool hasReturnExplicit = false;
		while (true)
		{
			if (ctx->parser->isRightParBracket())
			{
				break;
			}

			// Consome o tipo do parametro.
			if (auto paramType = ParserObjectTypeDecl::parseVariableType(ctx, traitMode))
			{
				functionTypeDecl->parameterTypeList.push_back(std::move(paramType));

				// Verifica se ha mais parametros e consome ','
				if (ctx->parser->isComma()) {
					ctx->parser->expectToken(TokenSubType_e::Comma);
					continue;
				}
			}

			// Processa o retorno
			if (ctx->parser->isArrow())
			{
				// Consome '->'
				ctx->parser->expectToken(TokenSubType_e::Arrow);

				functionTypeDecl->returnType = parseType(ctx, traitMode);

				hasReturnExplicit = true;
				break;
			}

			throw exceptions::custom_exception("Expected ')', '->' or ',' declaration",
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Se nao ha retorno explicito void e o tipo padrao.
		if (!hasReturnExplicit)
		{
			functionTypeDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome ')'
		ctx->parser->expectToken(TokenSubType_e::RParBracket);

		return functionTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseVariableType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto typeDecl = parseType(ctx, traitMode);

		if (typeDecl->typeID == TypeDeclID_e::Void)
		{
			throw exceptions::unexpected_type_exception("void",
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseTupleType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto tupleTypeDecl = std::make_unique<ast::TypeDeclTuple>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome '('.
		ctx->parser->expectToken(TokenSubType_e::LParBracket);

		// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
		if (auto tupleItemDecl = ParserObjectTypeDecl::parseType(ctx, traitMode))
		{
			if (tupleItemDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::unexpected_type_exception("void",
					ctx->parser->getTokenLine(),
					ctx->parser->getTokenColumn()
				);
			}
			tupleTypeDecl->tupleItemList.push_back(std::move(tupleItemDecl));
		}

		while (true)
		{
			if (ctx->parser->isRightBracket())
			{
				break;
			}

			if (ctx->parser->isComma())
			{
				// Consome ','.
				ctx->parser->expectToken(TokenSubType_e::Comma);

				// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
				if (auto tupleItemDecl = ParserObjectTypeDecl::parseType(ctx, traitMode))
				{
					if (tupleItemDecl->typeID == TypeDeclID_e::Void)
					{
						throw exceptions::unexpected_type_exception("void",
							ctx->parser->getTokenLine(),
							ctx->parser->getTokenColumn()
						);
					}
					tupleTypeDecl->tupleItemList.push_back(std::move(tupleItemDecl));
				}
				continue;
			}

			throw exceptions::unexpected_token_exception(
				ctx->parser->getTokenValue(),
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		// Consome ')'.
		ctx->parser->expectToken(TokenSubType_e::RParBracket);
		return tupleTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseNamedType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Verifica se inicia pelo escopo global.
		if (ctx->parser->isScopeResolution())
		{
			// Consome '::'.
			ctx->parser->expectToken(TokenSubType_e::ScopeResolution);
			namedTypeDecl->startFromRoot = true;
		}

		// Consome o identificador.
		namedTypeDecl->identifier = ctx->parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (ctx->parser->isLessThan())
		{
			// Consome '<'.
			ctx->parser->expectToken(TokenSubType_e::LessThan);

			while (true)
			{
				// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
				// serao quebrados em tokens menores.
				if (ctx->parser->isBitWiseRightShift() || ctx->parser->isBitWiseRightShiftAssign())
				{
					ParserObjectTypeDecl::reinterpretToken(ctx);
				}

				// Verifica se terminou a definicao dos generics
				if (ctx->parser->isGreaterThan())
				{
					break;
				}

				// Processa a definicao de generic
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parseType(ctx, traitMode));

				// Verifica se ha mais definicoes de generic
				if (ctx->parser->isComma())
				{
					// Consome ','.
					ctx->parser->expectToken(TokenSubType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			ctx->parser->expectToken(TokenSubType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (ctx->parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(ctx, traitMode);
		}
		return namedTypeDecl;
	}

	ArrayDeclPtr ParserObjectTypeDecl::parseArrayDecl(CompilationContext_t* ctx, Bool traitMode)
	{
		// Consome '['
		ctx->parser->expectToken(TokenSubType_e::LSquBracket);

		// Unsized array
		if (ctx->parser->isRightSquBracket()) {
			// Consome ']'
			ctx->parser->expectToken(TokenSubType_e::RSquBracket);
			return std::make_unique<ast::UnsizedArrayDecl>(
				ctx->parser->getTokenLine(),
				ctx->parser->getTokenColumn()
			);
		}

		auto sizedArrayDecl = std::make_unique<ast::SizedArrayDecl>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);
		
		// TODO: Futuramente vai fazer o parse de uma expressao em tempode compilacao.
		// mas por hora vai fazer o parse de um I32.
		sizedArrayDecl->size = ctx->parser->expectConstantI32();

		// Consome ']'
		ctx->parser->expectToken(TokenSubType_e::RSquBracket);

		return sizedArrayDecl;
	}

	TypeDeclNamedPtr ParserObjectTypeDecl::parseInternalNamedType(CompilationContext_t* ctx, Bool traitMode)
	{
		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>(
			ctx->parser->getTokenLine(),
			ctx->parser->getTokenColumn()
		);

		// Consome '::'.
		ctx->parser->expectToken(TokenSubType_e::ScopeResolution);

		// Consome o identificador.
		namedTypeDecl->identifier = ctx->parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (ctx->parser->isLessThan())
		{
			// Consome '<'.
			ctx->parser->expectToken(TokenSubType_e::LessThan);

			while (true)
			{
				// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
				// serao quebrados em tokens menores.
				if (ctx->parser->isBitWiseRightShift() || ctx->parser->isBitWiseRightShiftAssign())
				{
					ParserObjectTypeDecl::reinterpretToken(ctx);
				}

				// Verifica se terminou a definicao dos generics
				if (ctx->parser->isGreaterThan())
				{
					break;
				}

				// Processa a definicao de generic
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parseType(ctx, traitMode));

				// Verifica se ha mais definicoes de generic
				if (ctx->parser->isComma())
				{
					// Consome ','.
					ctx->parser->expectToken(TokenSubType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			ctx->parser->expectToken(TokenSubType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (ctx->parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(ctx, traitMode);
		}
		return namedTypeDecl;
	}

	void ParserObjectTypeDecl::reinterpretToken(CompilationContext_t* ctx)
	{
		switch (ctx->parser->getTokenSubType())
		{
		case TokenSubType_e::BitWiseRShift:		// >>
		case TokenSubType_e::GreaterThanOrEqual:	// >=
		case TokenSubType_e::BitWiseRShiftAssign:	// >>=
			ctx->parser->reinterpretToken(
				TokenType_e::Keyword,
				TokenSubType_e::GreaterThan,
				1
			);
			break;
		default:
			break;
		}
	}
} }