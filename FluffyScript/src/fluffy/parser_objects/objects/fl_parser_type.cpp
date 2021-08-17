#include "../../fl_ast_decl.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectTypeDecl
	 */

	TypeDeclPtr ParserObjectTypeDecl::parse(Parser* parser)
	{
		TypeDeclPtr typeDecl;

		const U32 line = parser->getTokenLine();
		const U32 column = parser->getTokenColumn();

		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::Void:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclVoid>();
			break;
		case TokenSubType_e::I8:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI8>();
			break;
		case TokenSubType_e::U8:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU8>();
			break;
		case TokenSubType_e::I16:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI16>();
			break;
		case TokenSubType_e::U16:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU16>();
			break;
		case TokenSubType_e::I32:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI32>();
			break;
		case TokenSubType_e::U32:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU32>();
			break;
		case TokenSubType_e::I64:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclI64>();
			break;
		case TokenSubType_e::U64:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclU64>();
			break;
		case TokenSubType_e::Fp32:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclFp32>();
			break;
		case TokenSubType_e::Fp64:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclFp64>();
			break;
		case TokenSubType_e::String:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclString>();
			break;
		case TokenSubType_e::Object:
			parser->nextToken();
			typeDecl = std::make_unique<ast::TypeDeclObject>();
			break;
		case TokenSubType_e::Vector:
			typeDecl = ParserObjectTypeDecl::parseVectorType(parser);
			break;
		case TokenSubType_e::Set:
			typeDecl = ParserObjectTypeDecl::parseSetType(parser);
			break;
		case TokenSubType_e::Map:
			typeDecl = ParserObjectTypeDecl::parseMapType(parser);
			break;
		case TokenSubType_e::Fn:
			typeDecl = ParserObjectTypeDecl::parseFunctionType(parser);
			break;
		default:
			// NamedType
			if (parser->isIdentifier() || parser->isScopeResolution())
			{
				typeDecl = ParserObjectTypeDecl::parseNamedType(parser);
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
			if (typeDecl->typeID == TypeDeclID_e::Void)
			{
				throw exceptions::custom_exception("'void' type can't be nullable",
					line,
					column
				);
			}

			// Consome '?'
			parser->expectToken([parser]() { return parser->isInterrogation(); });

			typeDecl->nullable = true;
		}

		// Verifica se e um array
		if (parser->isLeftSquBracket())
		{
			auto arrayType = std::make_unique<ast::TypeDeclArray>();

			// Adiciona o tipo base ao tipo array
			arrayType->valueType = std::move(typeDecl);

			while (true)
			{
				if (!parser->isLeftSquBracket())
				{
					break;
				}
				arrayType->arrayDeclList.push_back(ParserObjectTypeDecl::parseArrayDecl(parser));
			}

			// Verifica se o array e anulavel.
			if (parser->isInterrogation())
			{
				// Consome '?'
				parser->expectToken([parser]() { return parser->isInterrogation(); });

				arrayType->nullable = true;
			}
			typeDecl = std::move(arrayType);
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseOnlyNamedType(Parser* parser)
	{
		auto typeDecl = ParserObjectTypeDecl::parseNamedType(parser);

		if (typeDecl->typeID == TypeDeclID_e::Void)
		{
			throw exceptions::unexpected_type_exception("void", parser->getTokenLine(), parser->getTokenColumn());
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseVectorType(Parser* parser)
	{
		auto vectorTypeDecl = std::make_unique<ast::TypeDeclVector>();

		// Consome 'vector'
		parser->expectToken([parser]() { return parser->isVector(); });

		// Consome '<'
		parser->expectToken([parser]() { return parser->isLessThan(); });

		// Consome o tipo do valor
		vectorTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(parser);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(parser);
		}

		// Consome '>'
		parser->expectToken([parser]() { return parser->isGreaterThan(); });

		return vectorTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseSetType(Parser* parser)
	{
		auto setTypeDecl = std::make_unique<ast::TypeDeclSet>();

		// Consome 'set'
		parser->expectToken([parser]() { return parser->isSet(); });

		// Consome '<'
		parser->expectToken([parser]() { return parser->isLessThan(); });

		// Consome o tipo do valor
		setTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(parser);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(parser);
		}

		// Consome '>'
		parser->expectToken([parser]() { return parser->isGreaterThan(); });

		return setTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseMapType(Parser* parser)
	{
		auto mapTypeDecl = std::make_unique<ast::TypeDeclMap>();

		// Consome 'map'
		parser->expectToken([parser]() { return parser->isMap(); });

		// Consome '<'
		parser->expectToken([parser]() { return parser->isLessThan(); });

		// Consome o tipo da chave.
		mapTypeDecl->keyType = ParserObjectTypeDecl::parseVariableType(parser);

		// Consome ','
		parser->expectToken([parser]() { return parser->isComma(); });

		// Consome o tipo do valor
		mapTypeDecl->valueType = ParserObjectTypeDecl::parseVariableType(parser);

		// Para evitar conflitos durando o processamento de tipo, os caracteres >> e >>=
		// serao quebrados em tokens menores.
		if (parser->isBitWiseRightShift() || parser->isBitWiseRightShiftAssign())
		{
			ParserObjectTypeDecl::reinterpretToken(parser);
		}

		// Consome '>'
		parser->expectToken([parser]() { return parser->isGreaterThan(); });

		return mapTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseFunctionType(Parser* parser)
	{
		auto functionTypeDecl = std::make_unique<ast::TypeDeclFunction>();

		// Consome 'fn'
		parser->expectToken([parser]() { return parser->isFn(); });

		// Consome '('
		parser->expectToken([parser]() { return parser->isLeftParBracket(); });

		Bool hasReturnExplicit = false;
		while (true)
		{
			if (parser->isRightParBracket())
			{
				break;
			}

			// Consome o tipo do parametro.
			if (auto paramType = ParserObjectTypeDecl::parseVariableType(parser))
			{
				functionTypeDecl->parameterTypeList.push_back(std::move(paramType));

				// Verifica se ha mais parametros e consome ','
				if (parser->isComma()) {
					parser->expectToken([parser]() { return parser->isComma(); });
					continue;
				}
			}

			// Processa o retorno
			if (parser->isArrow())
			{
				// Consome '->'
				parser->expectToken([parser]() { return parser->isArrow(); });

				functionTypeDecl->returnType = parse(parser);

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
			functionTypeDecl->returnType = std::make_unique<ast::TypeDeclVoid>();
		}

		// Consome ')'
		parser->expectToken([parser]() { return parser->isRightParBracket(); });

		return functionTypeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseVariableType(Parser* parser)
	{
		auto typeDecl = parse(parser);

		if (typeDecl->typeID == TypeDeclID_e::Void)
		{
			throw exceptions::unexpected_type_exception("void",
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parseNamedType(Parser* parser)
	{
		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>();

		// Verifica se inicia pelo escopo global.
		if (parser->isScopeResolution())
		{
			// Consome '::'.
			parser->expectToken([parser]() { return parser->isScopeResolution(); });
			namedTypeDecl->startFromRoot = true;
		}

		// Consome o identificador.
		namedTypeDecl->identifier = parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (parser->isLessThan())
		{
			// Consome '<'.
			parser->expectToken([parser]() { return parser->isLessThan(); });

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
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parse(parser));

				// Verifica se ha mais definicoes de generic
				if (parser->isComma())
				{
					// Consome ','.
					parser->expectToken([parser]() { return parser->isComma(); });
					continue;
				}
			}

			// Consome '>'.
			parser->expectToken([parser]() { return parser->isGreaterThan(); });
		}

		// Verifica se ha identificadores internos.
		if (parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(parser);
		}
		return namedTypeDecl;
	}

	ArrayDeclPtr ParserObjectTypeDecl::parseArrayDecl(Parser* parser)
	{
		// Consome '['
		parser->expectToken([parser]() { return parser->isLeftSquBracket(); });

		// Unsized array
		if (parser->isRightSquBracket()) {
			// Consome ']'
			parser->expectToken([parser]() { return parser->isRightSquBracket(); });
			return std::make_unique<ast::UnsizedArrayDecl>();
		}
		auto sizedArrayDecl = std::make_unique<ast::SizedArrayDecl>();
		
		// TODO: Futuramente vai fazer o parse de uma expressao em tempode compilacao.
		// mas por hora vai fazer o parse de um I32.
		sizedArrayDecl->size = parser->expectConstantI32();

		// Consome ']'
		parser->expectToken([parser]() { return parser->isRightSquBracket(); });

		return sizedArrayDecl;
	}

	TypeDeclNamedPtr ParserObjectTypeDecl::parseInternalNamedType(Parser* parser)
	{
		auto namedTypeDecl = std::make_unique<ast::TypeDeclNamed>();

		// Consome '::'.
		parser->expectToken([parser]() { return parser->isScopeResolution(); });

		// Consome o identificador.
		namedTypeDecl->identifier = parser->expectIdentifier();

		// Verefica se a definicao de generic.
		if (parser->isLessThan())
		{
			// Consome '<'.
			parser->expectToken([parser]() { return parser->isLessThan(); });

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
				namedTypeDecl->genericDefinitionList.push_back(ParserObjectTypeDecl::parse(parser));

				// Verifica se ha mais definicoes de generic
				if (parser->isComma())
				{
					// Consome ','.
					parser->expectToken([parser]() { return parser->isComma(); });
					continue;
				}
			}

			// Consome '>'.
			parser->expectToken([parser]() { return parser->isGreaterThan(); });
		}

		// Verifica se ha identificadores internos.
		if (parser->isScopeResolution())
		{
			namedTypeDecl->internalIdentifier = ParserObjectTypeDecl::parseInternalNamedType(parser);
		}
		return namedTypeDecl;
	}

	void ParserObjectTypeDecl::reinterpretToken(Parser* parser)
	{
		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::BitWiseRShift:		// >>
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