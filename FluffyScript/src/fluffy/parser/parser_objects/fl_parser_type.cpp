#include "..\fl_ast_decl.h"
#include "..\..\fl_exceptions.h"
#include "fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectTypeDecl
	 */

	TypeDeclPtr ParserObjectTypeDecl::parse(Parser* parser)
	{
		return parseType(parser, false, false);
	}

	TypeDeclPtr ParserObjectTypeDecl::skip(Parser* parser)
	{
		return parseType(parser, false, true);
	}
	
	TypeDeclPtr ParserObjectTypeDecl::parseWithSelf(Parser* parser)
	{
		return parseType(parser, true, false);
	}

	TypeDeclPtr ParserObjectTypeDecl::parseType(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		TypeDeclPtr typeDecl;

		const U32 line = parser->getTokenLine();
		const U32 column = parser->getTokenColumn();

		TokenType_e type = TokenType_e::Unknown;

		switch (parser->getTokenType())
		{
		case TokenType_e::Void:
			type = TokenType_e::Void;
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
			break;
		case TokenType_e::I8:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI8>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::U8:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU8>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::I16:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI16>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::U16:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU16>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::I32:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI32>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::U32:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU32>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::I64:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI64>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::U64:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU64>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::Fp32:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclFp32>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::Fp64:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclFp64>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::String:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclString>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenType_e::Object:
			parser->nextToken();
			if (skipOnly) { break; }
			typeDecl = std::make_unique<ast::TypeDeclObject>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
			break;
		case TokenType_e::Fn:
			if (skipOnly) {
				ParserObjectTypeDecl::parseFunctionType(parser, traitMode, skipOnly);
				break;
			}
			typeDecl = ParserObjectTypeDecl::parseFunctionType(parser, traitMode, skipOnly);
			break;
		case TokenType_e::LParBracket:
			if (skipOnly) {
				ParserObjectTypeDecl::parseTupleType(parser, traitMode, skipOnly);
				break;
			}
			typeDecl = ParserObjectTypeDecl::parseTupleType(parser, traitMode, skipOnly);
			break;
		case TokenType_e::Self:
			if (traitMode)
			{
				parser->nextToken();
				if (skipOnly) { break; }
				typeDecl = std::make_unique<ast::TraitSelfTypeDecl>(
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			else
			{
				throw exceptions::custom_exception(
					"Self type only can be declared in traits",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			break;
		default:
			// NamedType
			if (parser->isIdentifier() || parser->isScopeResolution())
			{
				if (skipOnly) {
					ParserObjectTypeDecl::parseNamedType(parser, traitMode, skipOnly);
					break;
				}
				typeDecl = ParserObjectTypeDecl::parseNamedType(parser, traitMode, skipOnly);
				break;
			}

			throw exceptions::unexpected_token_exception(
				parser->getTokenValue(),
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Verifica se o tipo e anulavel.
		if (parser->isInterrogation())
		{
			if (skipOnly)
			{
				if (type == TokenType_e::Void)
				{
					throw exceptions::custom_exception("'void' type can't be nullable",
						line,
						column
					);
				}
			}

			if (typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception("'void' type can't be nullable",
					line,
					column
				);
			}

			// Consome '?'
			parser->expectToken(TokenType_e::Interrogation);

			typeDecl->nullable = true;
		}

		// Verifica se e um array
		if (parser->isLeftSquBracket())
		{
			if (skipOnly)
			{
				while (true)
				{
					if (!parser->isLeftSquBracket())
					{
						break;
					}
					ParserObjectTypeDecl::parseArrayDecl(parser, traitMode, skipOnly);
				}
				if (parser->isInterrogation())
				{
					// Consome '?'
					parser->expectToken(TokenType_e::Interrogation);
				}
				return nullptr;
			}

			auto arrayType = std::make_unique<ast::TypeDeclArray>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);

			// Adiciona o tipo base ao tipo array
			arrayType->valueType = std::move(typeDecl);

			while (true)
			{
				if (!parser->isLeftSquBracket())
				{
					break;
				}
				arrayType->arrayDeclList.push_back(ParserObjectTypeDecl::parseArrayDecl(parser, traitMode, skipOnly));
			}

			// Verifica se o array e anulavel.
			if (parser->isInterrogation())
			{
				// Consome '?'
				parser->expectToken(TokenType_e::Interrogation);

				arrayType->nullable = true;
			}
			typeDecl = std::move(arrayType);
		}
		if (skipOnly) {
			return nullptr;
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseFunctionType(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::Fn);
			parser->expectToken(TokenType_e::LParBracket);

			while (true)
			{
				if (parser->isRightParBracket())
				{
					break;
				}
			parseParameterTypeLabel:
				ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipOnly);

				if (parser->isComma()) {
					parser->expectToken(TokenType_e::Comma);
					goto parseParameterTypeLabel;
				}

				if (parser->isArrow())
				{
					parser->expectToken(TokenType_e::Arrow);
					parseType(parser, traitMode, skipOnly);
					break;
				}

				throw exceptions::custom_exception("Expected ')', '->' or ',' declaration",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			parser->expectToken(TokenType_e::RParBracket);
			return nullptr;
		}

		auto functionTypeDecl = std::make_unique<ast::TypeDeclFunction>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'fn'
		parser->expectToken(TokenType_e::Fn);

		// Consome '('
		parser->expectToken(TokenType_e::LParBracket);

		Bool hasReturnExplicit = false;
		while (true)
		{
			if (parser->isRightParBracket())
			{
				break;
			}

			// Consome o tipo do parametro.
			if (auto paramType = ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipOnly))
			{
				functionTypeDecl->parameterTypeList.push_back(std::move(paramType));

				// Verifica se ha mais parametros e consome ','
				if (parser->isComma()) {
					parser->expectToken(TokenType_e::Comma);
					continue;
				}
			}

			// Processa o retorno
			if (parser->isArrow())
			{
				// Consome '->'
				parser->expectToken(TokenType_e::Arrow);

				functionTypeDecl->returnType = parseType(parser, traitMode, skipOnly);

				hasReturnExplicit = true;
				break;
			}

			throw exceptions::custom_exception("Expected ')', '->' or ',' declaration",
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Se nao ha retorno explicito void e o tipo padrao.
		if (!hasReturnExplicit)
		{
			functionTypeDecl->returnType = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome ')'
		parser->expectToken(TokenType_e::RParBracket);

		return functionTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseVariableType(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		if (skipOnly)
		{
			if (parser->isVoid())
			{
				throw exceptions::unexpected_type_exception("void",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			parseType(parser, traitMode, skipOnly);
			return nullptr;
		}
		auto typeDecl = parseType(parser, traitMode, skipOnly);

		if (typeDecl->typeID == TypeDeclID_e::Void)
		{
			throw exceptions::unexpected_type_exception("void",
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseTupleType(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::LParBracket);
			if (parser->isVoid())
			{
				throw exceptions::unexpected_type_exception("void",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly);

			while (true)
			{
				if (parser->isRightBracket())
				{
					break;
				}

				if (parser->isComma())
				{
					parser->expectToken(TokenType_e::Comma);
					if (parser->isVoid())
					{
						throw exceptions::unexpected_type_exception("void",
							parser->getTokenLine(),
							parser->getTokenColumn()
						);
					}
					ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly);
					continue;
				}

				throw exceptions::unexpected_token_exception(
					parser->getTokenValue(),
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}

			// Consome ')'.
			parser->expectToken(TokenType_e::RParBracket);
			return nullptr;
		}

		auto tupleTypeDecl = std::make_unique<ast::TypeDeclTuple>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome '('.
		parser->expectToken(TokenType_e::LParBracket);

		// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
		if (auto tupleItemDecl = ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly))
		{
			if (tupleItemDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::unexpected_type_exception("void",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			tupleTypeDecl->tupleItemList.push_back(std::move(tupleItemDecl));
		}

		while (true)
		{
			if (parser->isRightBracket())
			{
				break;
			}

			if (parser->isComma())
			{
				// Consome ','.
				parser->expectToken(TokenType_e::Comma);

				// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
				if (auto tupleItemDecl = ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly))
				{
					if (tupleItemDecl->typeID == TypeDeclID_e::Void)
					{
						throw exceptions::unexpected_type_exception("void",
							parser->getTokenLine(),
							parser->getTokenColumn()
						);
					}
					tupleTypeDecl->tupleItemList.push_back(std::move(tupleItemDecl));
				}
				continue;
			}

			throw exceptions::unexpected_token_exception(
				parser->getTokenValue(),
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		// Consome ')'.
		parser->expectToken(TokenType_e::RParBracket);
		return tupleTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseNamedType(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		if (skipOnly)
		{
			if (parser->isScopeResolution())
			{
				parser->expectToken(TokenType_e::ScopeResolution);
			}
			parser->expectIdentifier();
			if (parser->isLessThan())
			{
				parser->expectToken(TokenType_e::LessThan);
				while (true)
				{
					if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
					{
						ParserObjectTypeDecl::reinterpretToken(parser);
					}
					if (parser->isGreaterThan())
					{
						break;
					}
					ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly);
					if (parser->isComma())
					{
						parser->expectToken(TokenType_e::Comma);
						continue;
					}
				}
				parser->expectToken(TokenType_e::GreaterThan);
			}

			// Verifica se ha identificadores internos.
			if (parser->isScopeResolution())
			{
				ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipOnly);
			}
			return nullptr;
		}

		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Verifica se inicia pelo escopo global.
		if (parser->isScopeResolution())
		{
			// Consome '::'.
			parser->expectToken(TokenType_e::ScopeResolution);
			namedTypeDecl->startFromRoot = true;
		}

		// Consome o identificador.
		namedTypeDecl->identifier = parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (parser->isLessThan())
		{
			// Consome '<'.
			parser->expectToken(TokenType_e::LessThan);

			while (true)
			{
				// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
				// serao quebrados em tokens menores.
				if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
				{
					ParserObjectTypeDecl::reinterpretToken(parser);
				}

				// Verifica se terminou a definicao dos generics
				if (parser->isGreaterThan())
				{
					break;
				}

				// Processa a definicao de generic
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly));

				// Verifica se ha mais definicoes de generic
				if (parser->isComma())
				{
					// Consome ','.
					parser->expectToken(TokenType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			parser->expectToken(TokenType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipOnly);
		}
		return namedTypeDecl;
	}

	ArrayDeclPtr ParserObjectTypeDecl::parseArrayDecl(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::LSquBracket);
			if (parser->isRightSquBracket()) {
				parser->expectToken(TokenType_e::RSquBracket);
				return nullptr;
			}
			parser->expectConstantInteger();
			parser->expectToken(TokenType_e::RSquBracket);
			return nullptr;
		}

		// Consome '['
		parser->expectToken(TokenType_e::LSquBracket);

		// Unsized array
		if (parser->isRightSquBracket()) {
			// Consome ']'
			parser->expectToken(TokenType_e::RSquBracket);
			return std::make_unique<ast::UnsizedArrayDecl>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}

		auto sizedArrayDecl = std::make_unique<ast::SizedArrayDecl>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);
		
		// TODO: Futuramente vai fazer o parse de uma expressao em tempode compilacao.
		// mas por hora vai fazer o parse de um I32.
		sizedArrayDecl->size = parser->expectConstantInteger();

		// Consome ']'
		parser->expectToken(TokenType_e::RSquBracket);

		return sizedArrayDecl;
	}

	TypeDeclNamedPtr ParserObjectTypeDecl::parseInternalNamedType(Parser* parser, Bool traitMode, Bool skipOnly)
	{
		if (skipOnly)
		{
			parser->expectToken(TokenType_e::ScopeResolution);
			parser->expectIdentifier();
			if (parser->isLessThan())
			{
				parser->expectToken(TokenType_e::LessThan);
				while (true)
				{
					// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
					// serao quebrados em tokens menores.
					if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
					{
						ParserObjectTypeDecl::reinterpretToken(parser);
					}

					// Verifica se terminou a definicao dos generics
					if (parser->isGreaterThan())
					{
						break;
					}
					ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly);

					// Verifica se ha mais definicoes de generic
					if (parser->isComma())
					{
						// Consome ','.
						parser->expectToken(TokenType_e::Comma);
						continue;
					}
				}

				// Consome '>'.
				parser->expectToken(TokenType_e::GreaterThan);
			}

			// Verifica se ha identificadores internos.
			if (parser->isScopeResolution())
			{
				ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipOnly);
			}
			return nullptr;
		}

		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome '::'.
		parser->expectToken(TokenType_e::ScopeResolution);

		// Consome o identificador.
		namedTypeDecl->identifier = parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (parser->isLessThan())
		{
			// Consome '<'.
			parser->expectToken(TokenType_e::LessThan);

			while (true)
			{
				// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
				// serao quebrados em tokens menores.
				if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
				{
					ParserObjectTypeDecl::reinterpretToken(parser);
				}

				// Verifica se terminou a definicao dos generics
				if (parser->isGreaterThan())
				{
					break;
				}

				// Processa a definicao de generic
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parseType(parser, traitMode, skipOnly));

				// Verifica se ha mais definicoes de generic
				if (parser->isComma())
				{
					// Consome ','.
					parser->expectToken(TokenType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			parser->expectToken(TokenType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipOnly);
		}
		return namedTypeDecl;
	}

	void ParserObjectTypeDecl::reinterpretToken(Parser* parser)
	{
		switch (parser->getTokenType())
		{
		case TokenType_e::BitWiseRShift:			// >>
		case TokenType_e::GreaterThanOrEqual:	// >=
		case TokenType_e::BitWiseRShiftAssign:	// >>=
			parser->reinterpretToken(
				TokenType_e::GreaterThan,
				1
			);
			break;
		default:
			break;
		}
	}
} }