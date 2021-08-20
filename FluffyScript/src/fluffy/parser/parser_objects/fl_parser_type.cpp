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

	TypeDeclPtr ParserObjectTypeDecl::parseType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		TypeDeclPtr typeDecl;

		const U32 line = parser->getTokenLine();
		const U32 column = parser->getTokenColumn();

		TokenSubType_e type = TokenSubType_e::Unknown;

		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Void:
			type = TokenSubType_e::Void;
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclVoid>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::I8:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI8>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::U8:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU8>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::I16:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI16>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::U16:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU16>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::I32:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI32>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::U32:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU32>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::I64:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclI64>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::U64:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclU64>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::Fp32:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclFp32>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::Fp64:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclFp64>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::String:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclString>(
				parser->getTokenLine(),
				parser->getTokenColumn()
				);
			break;
		case TokenSubType_e::Object:
			parser->nextToken();
			if (skipMode) { break; }
			typeDecl = std::make_unique<ast::TypeDeclObject>(
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
			break;
		case TokenSubType_e::Vector:
			if (skipMode) {
				ParserObjectTypeDecl::parseVectorType(parser, traitMode, skipMode);
				break;
			}
			typeDecl = ParserObjectTypeDecl::parseVectorType(parser, traitMode, skipMode);
			break;
		case TokenSubType_e::Set:
			if (skipMode) {
				typeDecl = ParserObjectTypeDecl::parseSetType(parser, traitMode, skipMode);
				break;
			}
			typeDecl = ParserObjectTypeDecl::parseSetType(parser, traitMode, skipMode);
			break;
		case TokenSubType_e::Map:
			if (skipMode) {
				ParserObjectTypeDecl::parseMapType(parser, traitMode, skipMode);
				break;
			}
			typeDecl = ParserObjectTypeDecl::parseMapType(parser, traitMode, skipMode);
			break;
		case TokenSubType_e::Fn:
			if (skipMode) {
				ParserObjectTypeDecl::parseFunctionType(parser, traitMode, skipMode);
				break;
			}
			typeDecl = ParserObjectTypeDecl::parseFunctionType(parser, traitMode, skipMode);
			break;
		case TokenSubType_e::LParBracket:
			if (skipMode) {
				ParserObjectTypeDecl::parseTupleType(parser, traitMode, skipMode);
				break;
			}
			typeDecl = ParserObjectTypeDecl::parseTupleType(parser, traitMode, skipMode);
			break;
		case TokenSubType_e::Self:
			if (traitMode)
			{
				parser->nextToken();
				if (skipMode) { break; }
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
				if (skipMode) {
					ParserObjectTypeDecl::parseNamedType(parser, traitMode, skipMode);
					break;
				}
				typeDecl = ParserObjectTypeDecl::parseNamedType(parser, traitMode, skipMode);
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
			if (skipMode)
			{
				if (type == TokenSubType_e::Void)
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
			parser->expectToken(TokenSubType_e::Interrogation);

			typeDecl->nullable = true;
		}

		// Verifica se e um array
		if (parser->isLeftSquBracket())
		{
			if (skipMode)
			{
				while (true)
				{
					if (!parser->isLeftSquBracket())
					{
						break;
					}
					ParserObjectTypeDecl::parseArrayDecl(parser, traitMode, skipMode);
				}
				if (parser->isInterrogation())
				{
					// Consome '?'
					parser->expectToken(TokenSubType_e::Interrogation);
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
				arrayType->arrayDeclList.push_back(ParserObjectTypeDecl::parseArrayDecl(parser, traitMode, skipMode));
			}

			// Verifica se o array e anulavel.
			if (parser->isInterrogation())
			{
				// Consome '?'
				parser->expectToken(TokenSubType_e::Interrogation);

				arrayType->nullable = true;
			}
			typeDecl = std::move(arrayType);
		}
		if (skipMode) {
			return nullptr;
		}
		return typeDecl;
	}


	TypeDeclPtr ParserObjectTypeDecl::parseVectorType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			parser->expectToken(TokenSubType_e::Vector);
			parser->expectToken(TokenSubType_e::LessThan);
			ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);
			if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
			{
				ParserObjectTypeDecl::reinterpretToken(parser);
			}
			parser->expectToken(TokenSubType_e::GreaterThan);
			return nullptr;
		}

		auto vectorTypeDecl = std::make_unique<ast::TypeDeclVector>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'vector'
		parser->expectToken(TokenSubType_e::Vector);

		// Consome '<'
		parser->expectToken(TokenSubType_e::LessThan);

		// Consome o tipo do valor
		vectorTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(parser);
		}

		// Consome '>'
		parser->expectToken(TokenSubType_e::GreaterThan);

		return vectorTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseSetType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			parser->expectToken(TokenSubType_e::Set);
			parser->expectToken(TokenSubType_e::LessThan);
			ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);
			if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
			{
				ParserObjectTypeDecl::reinterpretToken(parser);
			}
			parser->expectToken(TokenSubType_e::GreaterThan);
			return nullptr;
		}

		auto setTypeDecl = std::make_unique<ast::TypeDeclSet>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'set'
		parser->expectToken(TokenSubType_e::Set);

		// Consome '<'
		parser->expectToken(TokenSubType_e::LessThan);

		// Consome o tipo do valor
		setTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(parser);
		}

		// Consome '>'
		parser->expectToken(TokenSubType_e::GreaterThan);

		return setTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseMapType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			parser->expectToken(TokenSubType_e::Map);
			parser->expectToken(TokenSubType_e::LessThan);
			ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);
			parser->expectToken(TokenSubType_e::Comma);
			ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);
			if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
			{
				ParserObjectTypeDecl::reinterpretToken(parser);
			}
			parser->expectToken(TokenSubType_e::GreaterThan);
			return nullptr;
		}

		auto mapTypeDecl = std::make_unique<ast::TypeDeclMap>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'map'
		parser->expectToken(TokenSubType_e::Map);

		// Consome '<'
		parser->expectToken(TokenSubType_e::LessThan);

		// Consome o tipo da chave.
		mapTypeDecl->keyType = ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);

		// Consome ','
		parser->expectToken(TokenSubType_e::Comma);

		// Consome o tipo do valor
		mapTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(parser);
		}

		// Consome '>'
		parser->expectToken(TokenSubType_e::GreaterThan);

		return mapTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseFunctionType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			parser->expectToken(TokenSubType_e::Fn);
			parser->expectToken(TokenSubType_e::LParBracket);

			while (true)
			{
				if (parser->isRightParBracket())
				{
					break;
				}
			parseParameterTypeLabel:
				ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode);

				if (parser->isComma()) {
					parser->expectToken(TokenSubType_e::Comma);
					goto parseParameterTypeLabel;
				}

				if (parser->isArrow())
				{
					parser->expectToken(TokenSubType_e::Arrow);
					parseType(parser, traitMode, skipMode);
					break;
				}

				throw exceptions::custom_exception("Expected ')', '->' or ',' declaration",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}

			parser->expectToken(TokenSubType_e::RParBracket);
			return nullptr;
		}

		auto functionTypeDecl = std::make_unique<ast::TypeDeclFunction>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome 'fn'
		parser->expectToken(TokenSubType_e::Fn);

		// Consome '('
		parser->expectToken(TokenSubType_e::LParBracket);

		Bool hasReturnExplicit = false;
		while (true)
		{
			if (parser->isRightParBracket())
			{
				break;
			}

			// Consome o tipo do parametro.
			if (auto paramType = ParserObjectTypeDecl::parseVariableType(parser, traitMode, skipMode))
			{
				functionTypeDecl->parameterTypeList.push_back(std::move(paramType));

				// Verifica se ha mais parametros e consome ','
				if (parser->isComma()) {
					parser->expectToken(TokenSubType_e::Comma);
					continue;
				}
			}

			// Processa o retorno
			if (parser->isArrow())
			{
				// Consome '->'
				parser->expectToken(TokenSubType_e::Arrow);

				functionTypeDecl->returnType = parseType(parser, traitMode, skipMode);

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
		parser->expectToken(TokenSubType_e::RParBracket);

		return functionTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseVariableType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			if (parser->isVoid())
			{
				throw exceptions::unexpected_type_exception("void",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			parseType(parser, traitMode, skipMode);
			return nullptr;
		}
		auto typeDecl = parseType(parser, traitMode, skipMode);

		if (typeDecl->typeID == TypeDeclID_e::Void)
		{
			throw exceptions::unexpected_type_exception("void",
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseTupleType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			parser->expectToken(TokenSubType_e::LParBracket);
			if (parser->isVoid())
			{
				throw exceptions::unexpected_type_exception("void",
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}
			ParserObjectTypeDecl::parseType(parser, traitMode, skipMode);

			while (true)
			{
				if (parser->isRightBracket())
				{
					break;
				}

				if (parser->isComma())
				{
					parser->expectToken(TokenSubType_e::Comma);
					if (parser->isVoid())
					{
						throw exceptions::unexpected_type_exception("void",
							parser->getTokenLine(),
							parser->getTokenColumn()
						);
					}
					ParserObjectTypeDecl::parseType(parser, traitMode, skipMode);
					continue;
				}

				throw exceptions::unexpected_token_exception(
					parser->getTokenValue(),
					parser->getTokenLine(),
					parser->getTokenColumn()
				);
			}

			// Consome ')'.
			parser->expectToken(TokenSubType_e::RParBracket);
			return nullptr;
		}

		auto tupleTypeDecl = std::make_unique<ast::TypeDeclTuple>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome '('.
		parser->expectToken(TokenSubType_e::LParBracket);

		// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
		if (auto tupleItemDecl = ParserObjectTypeDecl::parseType(parser, traitMode, skipMode))
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
				parser->expectToken(TokenSubType_e::Comma);

				// Obrigatoriamente tuplas devem ter pelo menos 1 elemento.
				if (auto tupleItemDecl = ParserObjectTypeDecl::parseType(parser, traitMode, skipMode))
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
		parser->expectToken(TokenSubType_e::RParBracket);
		return tupleTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseNamedType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			if (parser->isScopeResolution())
			{
				parser->expectToken(TokenSubType_e::ScopeResolution);
			}
			parser->expectIdentifier();
			if (parser->isLessThan())
			{
				parser->expectToken(TokenSubType_e::LessThan);
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
					ParserObjectTypeDecl::parseType(parser, traitMode, skipMode);
					if (parser->isComma())
					{
						parser->expectToken(TokenSubType_e::Comma);
						continue;
					}
				}
				parser->expectToken(TokenSubType_e::GreaterThan);
			}

			// Verifica se ha identificadores internos.
			if (parser->isScopeResolution())
			{
				ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipMode);
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
			parser->expectToken(TokenSubType_e::ScopeResolution);
			namedTypeDecl->startFromRoot = true;
		}

		// Consome o identificador.
		namedTypeDecl->identifier = parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (parser->isLessThan())
		{
			// Consome '<'.
			parser->expectToken(TokenSubType_e::LessThan);

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
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parseType(parser, traitMode, skipMode));

				// Verifica se ha mais definicoes de generic
				if (parser->isComma())
				{
					// Consome ','.
					parser->expectToken(TokenSubType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			parser->expectToken(TokenSubType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipMode);
		}
		return namedTypeDecl;
	}

	ArrayDeclPtr ParserObjectTypeDecl::parseArrayDecl(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			parser->expectToken(TokenSubType_e::LSquBracket);
			if (parser->isRightSquBracket()) {
				parser->expectToken(TokenSubType_e::RSquBracket);
				return nullptr;
			}
			parser->expectConstantI32();
			parser->expectToken(TokenSubType_e::RSquBracket);
			return nullptr;
		}

		// Consome '['
		parser->expectToken(TokenSubType_e::LSquBracket);

		// Unsized array
		if (parser->isRightSquBracket()) {
			// Consome ']'
			parser->expectToken(TokenSubType_e::RSquBracket);
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
		sizedArrayDecl->size = parser->expectConstantI32();

		// Consome ']'
		parser->expectToken(TokenSubType_e::RSquBracket);

		return sizedArrayDecl;
	}

	TypeDeclNamedPtr ParserObjectTypeDecl::parseInternalNamedType(Parser* parser, Bool traitMode, Bool skipMode)
	{
		if (skipMode)
		{
			parser->expectToken(TokenSubType_e::ScopeResolution);
			parser->expectIdentifier();
			if (parser->isLessThan())
			{
				parser->expectToken(TokenSubType_e::LessThan);
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
					ParserObjectTypeDecl::parseType(parser, traitMode, skipMode);

					// Verifica se ha mais definicoes de generic
					if (parser->isComma())
					{
						// Consome ','.
						parser->expectToken(TokenSubType_e::Comma);
						continue;
					}
				}

				// Consome '>'.
				parser->expectToken(TokenSubType_e::GreaterThan);
			}

			// Verifica se ha identificadores internos.
			if (parser->isScopeResolution())
			{
				ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipMode);
			}
			return nullptr;
		}

		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>(
			parser->getTokenLine(),
			parser->getTokenColumn()
		);

		// Consome '::'.
		parser->expectToken(TokenSubType_e::ScopeResolution);

		// Consome o identificador.
		namedTypeDecl->identifier = parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (parser->isLessThan())
		{
			// Consome '<'.
			parser->expectToken(TokenSubType_e::LessThan);

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
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parseType(parser, traitMode, skipMode));

				// Verifica se ha mais definicoes de generic
				if (parser->isComma())
				{
					// Consome ','.
					parser->expectToken(TokenSubType_e::Comma);
					continue;
				}
			}

			// Consome '>'.
			parser->expectToken(TokenSubType_e::GreaterThan);
		}

		// Verifica se ha identificadores internos.
		if (parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(parser, traitMode, skipMode);
		}
		return namedTypeDecl;
	}

	void ParserObjectTypeDecl::reinterpretToken(Parser* parser)
	{
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::BitWiseRShift:			// >>
		case TokenSubType_e::GreaterThanOrEqual:	// >=
		case TokenSubType_e::BitWiseRShiftAssign:	// >>=
			parser->reinterpretToken(
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
