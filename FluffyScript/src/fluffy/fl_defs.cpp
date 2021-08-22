#include <tuple>
#include "fl_defs.h"
namespace fluffy {
	static const std::tuple<const I8*, TokenType_e> keywords[] = {
		std::make_tuple("include", 		TokenType_e::Include),		// ok
		std::make_tuple("from", 		TokenType_e::From),			// ok
		std::make_tuple("export", 		TokenType_e::Export),		// ok
		std::make_tuple("namespace",	TokenType_e::Namespace),	// ok
		std::make_tuple("class", 		TokenType_e::Class),		// ok
		std::make_tuple("extends", 		TokenType_e::Extends),		// ok
		std::make_tuple("implements",	TokenType_e::Implements),	// ok
		std::make_tuple("where", 		TokenType_e::Where),		// ok
		std::make_tuple("constructor",	TokenType_e::Constructor),	// ok
		std::make_tuple("destructor",	TokenType_e::Destructor),	// ok
		std::make_tuple("super",		TokenType_e::Super),		// ok
		std::make_tuple("public", 		TokenType_e::Public),		// ok
		std::make_tuple("protected",	TokenType_e::Protected),	// ok
		std::make_tuple("private", 		TokenType_e::Private),		// ok
		std::make_tuple("final",		TokenType_e::Final),		// ok
		std::make_tuple("override", 	TokenType_e::Override),		// ok
		std::make_tuple("abstract", 	TokenType_e::Abstract),		// ok
		std::make_tuple("virtual", 		TokenType_e::Virtual),		// ok
		std::make_tuple("interface",	TokenType_e::Interface),	// ok
		std::make_tuple("struct",		TokenType_e::Struct),		// ok
		std::make_tuple("enum", 		TokenType_e::Enum),			// ok
		std::make_tuple("match", 		TokenType_e::Match),		// ok
		std::make_tuple("when", 		TokenType_e::When),			// ok
		std::make_tuple("trait", 		TokenType_e::Trait),		// ok
		std::make_tuple("static", 		TokenType_e::Static),		// ok
		std::make_tuple("const", 		TokenType_e::Const),		// ok
		std::make_tuple("true", 		TokenType_e::True),			// ok
		std::make_tuple("false", 		TokenType_e::False),		// ok
		std::make_tuple("void", 		TokenType_e::Void),			// ok
		std::make_tuple("bool", 		TokenType_e::Bool),			// ok
		std::make_tuple("i8", 			TokenType_e::I8),			// ok
		std::make_tuple("u8", 			TokenType_e::U8),			// ok
		std::make_tuple("i16", 			TokenType_e::I16),			// ok
		std::make_tuple("u16", 			TokenType_e::U16),			// ok
		std::make_tuple("i32", 			TokenType_e::I32),			// ok
		std::make_tuple("u32", 			TokenType_e::U32),			// ok
		std::make_tuple("i64", 			TokenType_e::I64),			// ok
		std::make_tuple("u64", 			TokenType_e::U64),			// ok
		std::make_tuple("fp32",			TokenType_e::Fp32),			// ok
		std::make_tuple("fp64",			TokenType_e::Fp64),			// ok
		std::make_tuple("string", 		TokenType_e::String),		// ok
		std::make_tuple("object", 		TokenType_e::Object),		// ok
		std::make_tuple("fn", 			TokenType_e::Fn),			// ok
		std::make_tuple("let", 			TokenType_e::Let),			// ok
		std::make_tuple("this", 		TokenType_e::This),			// ok
		std::make_tuple("ref", 			TokenType_e::Ref),			// ok
		std::make_tuple("self", 		TokenType_e::Self),			// ok
		std::make_tuple("new", 			TokenType_e::New),			// ok
		std::make_tuple("as", 			TokenType_e::As),			// ok
		std::make_tuple("is", 			TokenType_e::Is),			// ok
		std::make_tuple("goto", 		TokenType_e::Goto),			// ok
		std::make_tuple("if", 			TokenType_e::If),			// ok
		std::make_tuple("else", 		TokenType_e::Else),			// ok
		std::make_tuple("continue", 	TokenType_e::Continue),		// ok
		std::make_tuple("break", 		TokenType_e::Break),		// ok
		std::make_tuple("for", 			TokenType_e::For),			// ok
		std::make_tuple("in",			TokenType_e::In),			// ok
		std::make_tuple("while", 		TokenType_e::While),		// ok
		std::make_tuple("do", 			TokenType_e::Do),			// ok
		std::make_tuple("try", 			TokenType_e::Try),			// ok
		std::make_tuple("catch",		TokenType_e::Catch),		// ok
		std::make_tuple("finally",		TokenType_e::Finally),		// ok
		std::make_tuple("panic", 		TokenType_e::Panic),		// ok
		std::make_tuple("return", 		TokenType_e::Return),		// ok
		std::make_tuple("null", 		TokenType_e::Null)			// ok
	};

	/**
	 * getTokenString
	 */

	const I8* getTokenString(const TokenType_e tokentype)
	{
		// Busca nas palavras chaves.
		for (auto& token : keywords)
		{
			if (std::get<1>(token) == tokentype)
			{
				return std::get<0>(token);
			}
		}

		// Busca nos simbolos.
		switch (tokentype)
		{
		case TokenType_e::ScopeResolution: 			return "::";
		case TokenType_e::Increment:				return "++";
		case TokenType_e::Decrement:				return "--";
		case TokenType_e::Arrow:					return "->";
		case TokenType_e::LParBracket:				return "(";
		case TokenType_e::RParBracket:				return ")";
		case TokenType_e::LSquBracket:				return "[";
		case TokenType_e::RSquBracket:				return "]";
		case TokenType_e::LBracket:					return "{";
		case TokenType_e::RBracket:					return "}";
		case TokenType_e::Plus:						return "+";
		case TokenType_e::Minus:					return "-";
		case TokenType_e::Division:					return "/";
		case TokenType_e::Multiplication:			return "*";
		case TokenType_e::Modulo:					return "%";
		case TokenType_e::Assign:					return "=";
		case TokenType_e::PlusAssign:				return "+=";
		case TokenType_e::MinusAssign:				return "-=";
		case TokenType_e::DivAssign:				return "/=";
		case TokenType_e::MultAssign:				return "*=";
		case TokenType_e::BitWiseAndAssign:			return "&=";
		case TokenType_e::BitWiseOrAssign:			return "|=";
		case TokenType_e::BitWiseXorAssign:			return "^=";
		case TokenType_e::ModAssign:				return "%=";
		case TokenType_e::BitWiseLShiftAssign:		return "<<=";
		case TokenType_e::BitWiseRShiftAssign:		return ">>=";
		case TokenType_e::GreaterThan:				return ">";
		case TokenType_e::LessThan:					return "<";
		case TokenType_e::GreaterThanOrEqual:		return ">=";
		case TokenType_e::LessThanOrEqual:			return "<=";
		case TokenType_e::Equal:					return "==";
		case TokenType_e::NotEqual:					return "!=";
		case TokenType_e::BitWiseLShift:			return "<<";
		case TokenType_e::BitWiseRShift:			return ">>";
		case TokenType_e::BitWiseAnd:				return "&";
		case TokenType_e::BitWiseOr:				return "|";
		case TokenType_e::BitWiseXor:				return "^";
		case TokenType_e::BitWiseNot:				return "~";
		case TokenType_e::Colon:					return ":";
		case TokenType_e::SemiColon:				return ";";
		case TokenType_e::LogicalAnd:				return "&&";
		case TokenType_e::LogicalOr:				return "||";
		case TokenType_e::LogicalNot:				return "!";
		case TokenType_e::Interrogation:			return "?";
		case TokenType_e::Comma:					return ":";
		case TokenType_e::Dot:						return ".";
		case TokenType_e::ConstantInteger:			return "i8 constant";
		case TokenType_e::ConstantFp32:				return "fp32 constant";
		case TokenType_e::ConstantFp64:				return "fp64 constant";
		case TokenType_e::ConstantBin:				return "binary constant";
		case TokenType_e::ConstantHex:				return "hex constant";
		case TokenType_e::ConstantChar:				return "char constant";
		case TokenType_e::ConstantString:			return "string constant";
		case TokenType_e::Identifier:				return "<identifier>";
		default:
			return "Unknown token";
		}
	}
}