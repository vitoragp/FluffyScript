#include "../../fl_ast.h"
#include "../../fl_exceptions.h"
#include "../fl_parser_objects.h"

namespace fluffy { namespace parser_objects {
	/**
	 * ParserObjectTypeDecl
	 */

	TypeDeclPtr ParserObjectTypeDecl::parse(Parser* parser)
	{
		TypeDeclPtr typeDecl;

		switch (parser->getTokenSubType())
		{
		case TokenSubType_e::eTST_Void:
			typeDecl = std::make_unique<ast::TypeDeclVoid>();
			break;
		case TokenSubType_e::eTST_I8:
			typeDecl = std::make_unique<ast::TypeDeclI8>();
			break;
		case TokenSubType_e::eTST_U8:
			typeDecl = std::make_unique<ast::TypeDeclU8>();
			break;
		case TokenSubType_e::eTST_I16:
			typeDecl = std::make_unique<ast::TypeDeclI16>();
			break;
		case TokenSubType_e::eTST_U16:
			typeDecl = std::make_unique<ast::TypeDeclU16>();
			break;
		case TokenSubType_e::eTST_I32:
			typeDecl = std::make_unique<ast::TypeDeclI32>();
			break;
		case TokenSubType_e::eTST_U32:
			typeDecl = std::make_unique<ast::TypeDeclU32>();
			break;
		case TokenSubType_e::eTST_I64:
			typeDecl = std::make_unique<ast::TypeDeclI64>();
			break;
		case TokenSubType_e::eTST_U64:
			typeDecl = std::make_unique<ast::TypeDeclU64>();
			break;
		case TokenSubType_e::eTST_Fp32:
			typeDecl = std::make_unique<ast::TypeDeclFp32>();
			break;
		case TokenSubType_e::eTST_Fp64:
			typeDecl = std::make_unique<ast::TypeDeclFp64>();
			break;
		case TokenSubType_e::eTST_String:
			typeDecl = std::make_unique<ast::TypeDeclString>();
			break;
		case TokenSubType_e::eTST_Object:
			typeDecl = std::make_unique<ast::TypeDeclObject>();
			break;
		case TokenSubType_e::eTST_Vector:
			typeDecl = ParserObjectTypeDecl::parserVectorType(parser);
			break;
		case TokenSubType_e::eTST_Set:
			typeDecl = ParserObjectTypeDecl::parserSetType(parser);
			break;
		case TokenSubType_e::eTST_Map:
			typeDecl = ParserObjectTypeDecl::parserMapType(parser);
			break;
		case TokenSubType_e::eTST_Fn:
			typeDecl = ParserObjectTypeDecl::parserFunctionType(parser);
			break;
		default:
			throw exceptions::unexpected_token_exception(
				parser->getTokenValue(),
				parser->getTokenLine(),
				parser->getTokenColumn()
			);
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
			return arrayType;
		}
		return typeDecl;
	}

	TypeDeclPtr ParserObjectTypeDecl::parserVectorType(Parser* parser)
	{
		return nullptr;
	}

	TypeDeclPtr ParserObjectTypeDecl::parserSetType(Parser* parser)
	{
		return nullptr;
	}

	TypeDeclPtr ParserObjectTypeDecl::parserMapType(Parser* parser)
	{
		return nullptr;
	}

	TypeDeclPtr ParserObjectTypeDecl::parserFunctionType(Parser* parser)
	{
		return nullptr;
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
} }