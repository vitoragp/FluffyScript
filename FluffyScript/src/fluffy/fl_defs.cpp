#include <tuple>
#include "fl_defs.h"
namespace fluffy {
	static const std::tuple<const I8*, TokenSubType_e> keywords[] = {
		std::make_tuple("include", 		TokenSubType_e::Include),		// ok
		std::make_tuple("from", 		TokenSubType_e::From),			// ok
		std::make_tuple("export", 		TokenSubType_e::Export),		// ok
		std::make_tuple("namespace",	TokenSubType_e::Namespace),		// ok
		std::make_tuple("class", 		TokenSubType_e::Class),			// ok
		std::make_tuple("extends", 		TokenSubType_e::Extends),		// ok
		std::make_tuple("implements",	TokenSubType_e::Implements),	// ok
		std::make_tuple("where", 		TokenSubType_e::Where),			// ok
		std::make_tuple("constructor",	TokenSubType_e::Constructor),	// ok
		std::make_tuple("destructor",	TokenSubType_e::Destructor),	// ok
		std::make_tuple("super",		TokenSubType_e::Super),			// ok
		std::make_tuple("public", 		TokenSubType_e::Public),		// ok
		std::make_tuple("protected",	TokenSubType_e::Protected),		// ok
		std::make_tuple("private", 		TokenSubType_e::Private),		// ok
		std::make_tuple("final",		TokenSubType_e::Final),			// ok
		std::make_tuple("override", 	TokenSubType_e::Override),		// ok
		std::make_tuple("abstract", 	TokenSubType_e::Abstract),		// ok
		std::make_tuple("virtual", 		TokenSubType_e::Virtual),		// ok
		std::make_tuple("interface",	TokenSubType_e::Interface),		// ok
		std::make_tuple("struct",		TokenSubType_e::Struct),		// ok
		std::make_tuple("enum", 		TokenSubType_e::Enum),			// ok
		std::make_tuple("match", 		TokenSubType_e::Match),			// ok
		std::make_tuple("when", 		TokenSubType_e::When),			// ok
		std::make_tuple("trait", 		TokenSubType_e::Trait),			// ok
		std::make_tuple("static", 		TokenSubType_e::Static),		// ok
		std::make_tuple("const", 		TokenSubType_e::Const),			// ok
		std::make_tuple("true", 		TokenSubType_e::True),			// ok
		std::make_tuple("false", 		TokenSubType_e::False),			// ok
		std::make_tuple("void", 		TokenSubType_e::Void),			// ok
		std::make_tuple("bool", 		TokenSubType_e::Bool),			// ok
		std::make_tuple("i8", 			TokenSubType_e::I8),			// ok
		std::make_tuple("u8", 			TokenSubType_e::U8),			// ok
		std::make_tuple("i16", 			TokenSubType_e::I16),			// ok
		std::make_tuple("u16", 			TokenSubType_e::U16),			// ok
		std::make_tuple("i32", 			TokenSubType_e::I32),			// ok
		std::make_tuple("u32", 			TokenSubType_e::U32),			// ok
		std::make_tuple("i64", 			TokenSubType_e::I64),			// ok
		std::make_tuple("u64", 			TokenSubType_e::U64),			// ok
		std::make_tuple("fp32",			TokenSubType_e::Fp32),			// ok
		std::make_tuple("fp64",			TokenSubType_e::Fp64),			// ok
		std::make_tuple("string", 		TokenSubType_e::String),		// ok
		std::make_tuple("vector",		TokenSubType_e::Vector),		// ok
		std::make_tuple("set", 			TokenSubType_e::Set),			// ok
		std::make_tuple("map", 			TokenSubType_e::Map),			// ok
		std::make_tuple("object", 		TokenSubType_e::Object),		// ok
		std::make_tuple("fn", 			TokenSubType_e::Fn),			// ok
		std::make_tuple("let", 			TokenSubType_e::Let),			// ok
		std::make_tuple("this", 		TokenSubType_e::This),			// ok
		std::make_tuple("ref", 			TokenSubType_e::Ref),			// ok
		std::make_tuple("self", 		TokenSubType_e::Self),			// ok
		std::make_tuple("new", 			TokenSubType_e::New),			// ok
		std::make_tuple("as", 			TokenSubType_e::As),			// ok
		std::make_tuple("is", 			TokenSubType_e::Is),			// ok
		std::make_tuple("goto", 		TokenSubType_e::Goto),			// ok
		std::make_tuple("if", 			TokenSubType_e::If),			// ok
		std::make_tuple("else", 		TokenSubType_e::Else),			// ok
		std::make_tuple("continue", 	TokenSubType_e::Continue),		// ok
		std::make_tuple("break", 		TokenSubType_e::Break),			// ok
		std::make_tuple("for", 			TokenSubType_e::For),			// ok
		std::make_tuple("in",			TokenSubType_e::In),			// ok
		std::make_tuple("while", 		TokenSubType_e::While),			// ok
		std::make_tuple("do", 			TokenSubType_e::Do),			// ok
		std::make_tuple("try", 			TokenSubType_e::Try),			// ok
		std::make_tuple("catch",		TokenSubType_e::Catch),			// ok
		std::make_tuple("finally",		TokenSubType_e::Finally),		// ok
		std::make_tuple("panic", 		TokenSubType_e::Panic),			// ok
		std::make_tuple("return", 		TokenSubType_e::Return),		// ok
		std::make_tuple("null", 		TokenSubType_e::Null)			// ok
	};

	/**
	 * getTokenString
	 */

	const I8* getTokenString(const TokenSubType_e tokenSubType)
	{
		// Busca nas palavras chaves.
		for (auto& token : keywords)
		{
			if (std::get<1>(token) == tokenSubType)
			{
				return std::get<0>(token);
			}
		}

		// Busca nos simbolos.
		switch (tokenSubType)
		{
		case TokenSubType_e::ScopeResolution: 			return "::";
		case TokenSubType_e::Increment:					return "++";
		case TokenSubType_e::Decrement:					return "--";
		case TokenSubType_e::Arrow:						return "->";
		case TokenSubType_e::LParBracket:				return "(";
		case TokenSubType_e::RParBracket:				return ")";
		case TokenSubType_e::LSquBracket:				return "[";
		case TokenSubType_e::RSquBracket:				return "]";
		case TokenSubType_e::LBracket:					return "{";
		case TokenSubType_e::RBracket:					return "}";
		case TokenSubType_e::Plus:						return "+";
		case TokenSubType_e::Minus:						return "-";
		case TokenSubType_e::Division:					return "/";
		case TokenSubType_e::Multiplication:			return "*";
		case TokenSubType_e::Modulo:					return "%";
		case TokenSubType_e::Assign:					return "=";
		case TokenSubType_e::PlusAssign:				return "+=";
		case TokenSubType_e::MinusAssign:				return "-=";
		case TokenSubType_e::DivAssign:					return "/=";
		case TokenSubType_e::MultAssign:				return "*=";
		case TokenSubType_e::BitWiseAndAssign:			return "&=";
		case TokenSubType_e::BitWiseOrAssign:			return "|=";
		case TokenSubType_e::BitWiseXorAssign:			return "^=";
		case TokenSubType_e::ModAssign:					return "%=";
		case TokenSubType_e::BitWiseLShiftAssign:		return "<<=";
		case TokenSubType_e::BitWiseRShiftAssign:		return ">>=";
		case TokenSubType_e::GreaterThan:				return ">";
		case TokenSubType_e::LessThan:					return "<";
		case TokenSubType_e::GreaterThanOrEqual:		return ">=";
		case TokenSubType_e::LessThanOrEqual:			return "<=";
		case TokenSubType_e::Equal:						return "==";
		case TokenSubType_e::NotEqual:					return "!=";
		case TokenSubType_e::BitWiseLShift:				return "<<";
		case TokenSubType_e::BitWiseRShift:				return ">>";
		case TokenSubType_e::BitWiseAnd:				return "&";
		case TokenSubType_e::BitWiseOr:					return "|";
		case TokenSubType_e::BitWiseXor:				return "^";
		case TokenSubType_e::BitWiseNot:				return "~";
		case TokenSubType_e::Colon:						return ":";
		case TokenSubType_e::SemiColon:					return ";";
		case TokenSubType_e::LogicalAnd:				return "&&";
		case TokenSubType_e::LogicalOr:					return "||";
		case TokenSubType_e::LogicalNot:				return "!";
		case TokenSubType_e::Interrogation:				return "?";
		case TokenSubType_e::Comma:						return ":";
		case TokenSubType_e::Dot:						return ".";
		case TokenSubType_e::ConstantI8:				return "i8 constant";
		case TokenSubType_e::ConstantU8:				return "u8 constant";
		case TokenSubType_e::ConstantI16:				return "i16 constant";
		case TokenSubType_e::ConstantU16:				return "u16 constant";
		case TokenSubType_e::ConstantI32:				return "i32 constant";
		case TokenSubType_e::ConstantU32:				return "u32 constant";
		case TokenSubType_e::ConstantI64:				return "i64 constant";
		case TokenSubType_e::ConstantU64:				return "u64 constant";
		case TokenSubType_e::ConstantFp32:				return "fp32 constant";
		case TokenSubType_e::ConstantFp64:				return "fp64 constant";
		case TokenSubType_e::ConstantBin:				return "binary constant";
		case TokenSubType_e::ConstantHex:				return "hex constant";
		case TokenSubType_e::ConstantChar:				return "char constant";
		case TokenSubType_e::ConstantString:			return "string constant";
		default:
			return "Unknown token";
		}
	}
}