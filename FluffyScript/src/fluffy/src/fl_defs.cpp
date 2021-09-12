#include <tuple>
#include "fl_defs.h"
namespace fluffy {
	static const std::tuple<const I8*, TokenType_e> keywords[] = {
		std::make_tuple("include", 		TokenType_e::Include),		// ok
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
		case TokenType_e::Comma:					return ",";
		case TokenType_e::Dot:						return ".";
		case TokenType_e::SafeDot:					return "?.";
		case TokenType_e::ConstantInteger:			return "i8 constant";
		case TokenType_e::ConstantFp32:				return "fp32 constant";
		case TokenType_e::ConstantFp64:				return "fp64 constant";
		case TokenType_e::ConstantBin:				return "binary constant";
		case TokenType_e::ConstantHex:				return "hex constant";
		case TokenType_e::ConstantChar:				return "char constant";
		case TokenType_e::ConstantString:			return "string constant";
		case TokenType_e::Identifier:				return "<identifier>";
		default:
			break;
		}
		return "Unknown token";
	}

	const I8* getTokenString(const AstNodeType_e nodeType)
	{
		switch (nodeType)
		{
		case AstNodeType_e::ClassConstructorDecl:
			return "constructor";
		case AstNodeType_e::ClassDestructorDecl:
			return "destructor";
		case AstNodeType_e::GenericDecl:
			return "generic";
		case AstNodeType_e::Block:
			return "block";
		case AstNodeType_e::TernaryExpr:
			return "ternary expr";
		case AstNodeType_e::BinaryExpr:
			return "binary expr";
		case AstNodeType_e::UnaryExpr:
			return "unary expr";
		case AstNodeType_e::AsExpr:
			return "as expr";
		case AstNodeType_e::IsExpr:
			return "is expr";
		case AstNodeType_e::MatchExpr:
			return "match expr";
		case AstNodeType_e::FunctionDeclExpr:
			return "function decl expr";
		case AstNodeType_e::FunctionCallExpr:
			return "function call expr";
		case AstNodeType_e::GenericCallExpr:
			return "generic call expr";
		case AstNodeType_e::IndexExpr:
			return "index expr";
		case AstNodeType_e::NewExpr:
			return "new expr";
		case AstNodeType_e::ConstantBoolExpr:
			return "constant bool expr";
		case AstNodeType_e::ConstantIntegerExpr:
			return "constant integer expr";
		case AstNodeType_e::ConstantRealExpr:
			return "constant real expr";
		case AstNodeType_e::ConstantStringExpr:
			return "constant string expr";
		case AstNodeType_e::ConstantCharExpr:
			return "constant char expr";
		case AstNodeType_e::ConstantNullExpr:
			return "constant null expr";
		case AstNodeType_e::ThisExpr:
			return "this expr";
		case AstNodeType_e::SuperExpr:
			return "super expr";
		case AstNodeType_e::AnomClassDeclExpr:
			return "anom class decl expr";
		case AstNodeType_e::PrimitiveTypeExpr:
			return "primitive type expr";
		case AstNodeType_e::ArrayInitExpr:
			return "array init expr";
		case AstNodeType_e::MatchWhenExpr:
			return "match when expr";
		case AstNodeType_e::NewBlockExpr:
			return "new block expr";
		case AstNodeType_e::NewItemExpr:
			return "new item expr";
		case AstNodeType_e::LiteralPattern:
			return "literal pattern";
		case AstNodeType_e::TuplePatternDecl:
			return "tuple pattern";
		case AstNodeType_e::StructurePatternDecl:
			return "structure pattern";
		case AstNodeType_e::EnumerablePatternDecl:
			return "enumerable pattern";
		case AstNodeType_e::StructureItemPatternDecl:
			return "structure item pattern";
		case AstNodeType_e::StmtIf:
			return "if stmt";
		case AstNodeType_e::StmtIfLet:
			return "if let stmt";
		case AstNodeType_e::StmtFor:
			return "for stmt";
		case AstNodeType_e::StmtWhile:
			return "while stmt";
		case AstNodeType_e::StmtDoWhile:
			return "do while stmt";
		case AstNodeType_e::StmtMatch:
			return "match stmt";
		case AstNodeType_e::StmtReturn:
			return "return stmt";
		case AstNodeType_e::StmtContinue:
			return "continue stmt";
		case AstNodeType_e::StmtBreak:
			return "break stmt";
		case AstNodeType_e::StmtTry:
			return "try stmt";
		case AstNodeType_e::StmtPanic:
			return "panic stmt";
		case AstNodeType_e::StmtExpr:
			return "expr stmt";
		case AstNodeType_e::StmtForInitDecl:
			return "stmt for init decl";
		case AstNodeType_e::StmtMatchWhenDecl:
			return "stmt match when decl";
		case AstNodeType_e::StmtCatchBlockDecl:
			return "stmt catch block decl";
		case AstNodeType_e::VoidType:
			return "void type";
		case AstNodeType_e::BoolType:
			return "bool type";
		case AstNodeType_e::I8Type:
			return "i8 type";
		case AstNodeType_e::U8Type:
			return "u8 type";
		case AstNodeType_e::I16Type:
			return "i16 type";
		case AstNodeType_e::U16Type:
			return "u16 type";
		case AstNodeType_e::I32Type:
			return "i32 type";
		case AstNodeType_e::U32Type:
			return "u32 type";
		case AstNodeType_e::I64Type:
			return "i64 type";
		case AstNodeType_e::U64Type:
			return "u64 type";
		case AstNodeType_e::Fp32Type:
			return "fp32 type";
		case AstNodeType_e::Fp64Type:
			return "fp64 type";
		case AstNodeType_e::StringType:
			return "string type";
		case AstNodeType_e::ObjectType:
			return "object type";
		case AstNodeType_e::ArrayType:
			return "array type";
		case AstNodeType_e::FunctionType:
			return "function type";
		case AstNodeType_e::SelfType:
			return "self type";
		case AstNodeType_e::TupleType:
			return "tuple type";
		case AstNodeType_e::SizedArray:
			return "sized type";
		case AstNodeType_e::UnsizedArray:
			return "unsized type";
		default:
			break;
		}
		return nullptr;
	}
}