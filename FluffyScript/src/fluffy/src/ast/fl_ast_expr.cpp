#include <memory>
#include <vector>
#include "ast\fl_ast_expr.h"
#include "ast\fl_ast_decl.h"
#include "ast\fl_ast_block.h"
#include "ast\fl_ast_pattern.h"
#include "ast\fl_ast_type.h"
#include "fl_defs.h"

namespace fluffy { namespace ast { namespace expr {
	/**
	 * ExpressionDecl
	 */

	ExpressionDecl::ExpressionDecl(AstNodeType_e nodeType, ExpressionDeclType_e type, const U32 line, const U32 column)
		: AstNode(nodeType, line, column)
		, type(type)
		, needGenericValidation(false)
		, beginPosition(0)
		, endPosition(0)
	{}

	ExpressionDecl::~ExpressionDecl()
	{}

	/**
	 * ExpressionTernaryDecl
	 */

	ExpressionTernaryDecl::ExpressionTernaryDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::TernaryExpr, ExpressionDeclType_e::Ternary, line, column)
	{}

	ExpressionTernaryDecl::~ExpressionTernaryDecl()
	{}

	/**
	 * ExpressionBinaryDecl
	 */

	ExpressionBinaryDecl::ExpressionBinaryDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::BinaryExpr, ExpressionDeclType_e::Binary, line, column)
		, op(TokenType_e::Unknown)
	{}

	ExpressionBinaryDecl::~ExpressionBinaryDecl()
	{}

	/**
	 * ExpressionUnaryDecl
	 */

	ExpressionUnaryDecl::ExpressionUnaryDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::UnaryExpr, ExpressionDeclType_e::Unary, line, column)
		, unaryType(ExpressionUnaryType_e::Unknown)
		, op(TokenType_e::Unknown)
	{}

	ExpressionUnaryDecl::~ExpressionUnaryDecl()
	{}

	/**
	 * ExpressionAsDecl
	 */

	ExpressionAsDecl::ExpressionAsDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::AsExpr, ExpressionDeclType_e::As, line, column)
	{}

	ExpressionAsDecl::~ExpressionAsDecl()
	{}

	/**
	 * ExpressionIsDecl
	 */

	ExpressionIsDecl::ExpressionIsDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::IsExpr, ExpressionDeclType_e::Is, line, column)
	{}

	ExpressionIsDecl::~ExpressionIsDecl()
	{}

	/**
	 * ExpressionMatchDecl
	 */

	ExpressionMatchDecl::ExpressionMatchDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::MatchExpr, ExpressionDeclType_e::Match, line, column)
	{}

	ExpressionMatchDecl::~ExpressionMatchDecl()
	{}

	/**
	 * ExpressionFunctionDecl
	 */

	ExpressionFunctionDecl::ExpressionFunctionDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::FunctionDeclExpr, ExpressionDeclType_e::FunctionDecl, line, column)
		, inferenceType(InferenceType_e::None)
	{}

	ExpressionFunctionDecl::~ExpressionFunctionDecl()
	{}

	Bool
	ExpressionFunctionDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	ExpressionFunctionDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->parametersDeclList)
		{
			children.push_back((AstNode*) it.get());
		}
		auto blockChildren = blockDecl->getChildren();
		children.insert(children.end(), blockChildren.begin(), blockChildren.end());
		return children;
	}

	/**
	 * ExpressionFunctionCall
	 */

	ExpressionFunctionCall::ExpressionFunctionCall(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::FunctionCallExpr, ExpressionDeclType_e::FunctionCall, line, column)
	{}

	ExpressionFunctionCall::~ExpressionFunctionCall()
	{}

	/**
	 * ExpressionGenericCallDecl
	 */

	ExpressionGenericCallDecl::ExpressionGenericCallDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::GenericCallExpr, ExpressionDeclType_e::GenericDecl, line, column)
	{}

	ExpressionGenericCallDecl::~ExpressionGenericCallDecl()
	{}

	/**
	 * ExpressionIndexDecl
	 */

	ExpressionIndexDecl::ExpressionIndexDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::IndexExpr, ExpressionDeclType_e::Index, line, column)
	{}

	ExpressionIndexDecl::~ExpressionIndexDecl()
	{}

	/**
	 * ExpressionNewDecl
	 */

	ExpressionNewDecl::ExpressionNewDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::NewExpr, ExpressionDeclType_e::New, line, column)
	{}

	ExpressionNewDecl::~ExpressionNewDecl()
	{}

	/**
	 * ExpressionConstantBoolDecl
	 */

	ExpressionConstantBoolDecl::ExpressionConstantBoolDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ConstantBoolExpr, ExpressionDeclType_e::ConstantBool, line, column)
		, valueDecl(false)
	{}

	ExpressionConstantBoolDecl::~ExpressionConstantBoolDecl()
	{}

	/**
	 * ExpressionConstantIntegerDecl
	 */

	ExpressionConstantIntegerDecl::ExpressionConstantIntegerDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ConstantIntegerExpr, ExpressionDeclType_e::ConstantInteger, line, column)
		, valueDecl(0)
		, valueType(TypeDeclID_e::Unknown)
	{}

	ExpressionConstantIntegerDecl::~ExpressionConstantIntegerDecl()
	{}

	/**
	 * ExpressionConstantRealDecl
	 */

	ExpressionConstantRealDecl::ExpressionConstantRealDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ConstantRealExpr, ExpressionDeclType_e::ConstantReal, line, column)
		, valueDecl(0.0)
		, valueType(TypeDeclID_e::Unknown)
	{}

	ExpressionConstantRealDecl::~ExpressionConstantRealDecl()
	{}

	/**
	 * ExpressionConstantStringDecl
	 */

	ExpressionConstantStringDecl::ExpressionConstantStringDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ConstantStringExpr, ExpressionDeclType_e::ConstantString, line, column)
	{}

	ExpressionConstantStringDecl::~ExpressionConstantStringDecl()
	{}

	/**
	 * ExpressionConstantCharDecl
	 */

	ExpressionConstantCharDecl::ExpressionConstantCharDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ConstantCharExpr, ExpressionDeclType_e::ConstantChar, line, column)
		, valueDecl(0)
	{}

	ExpressionConstantCharDecl::~ExpressionConstantCharDecl()
	{}

	/**
	 * ExpressionConstantNullDecl
	 */

	ExpressionConstantNullDecl::ExpressionConstantNullDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ConstantNullExpr, ExpressionDeclType_e::ConstantNull, line, column)
	{}

	ExpressionConstantNullDecl::~ExpressionConstantNullDecl()
	{}
	
	/**
	 * ExpressionThisDecl
	 */

	ExpressionThisDecl::ExpressionThisDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ThisExpr, ExpressionDeclType_e::This, line, column)
	{}

	ExpressionThisDecl::~ExpressionThisDecl()
	{}

	/**
	 * ExpressionSuperDecl
	 */

	ExpressionSuperDecl::ExpressionSuperDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::SuperExpr, ExpressionDeclType_e::Super, line, column)
	{}

	ExpressionSuperDecl::~ExpressionSuperDecl()
	{}

	/**
	 * ExpressionAnomClassDecl
	 */

	ExpressionAnomClassDecl::ExpressionAnomClassDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::AnomClassDeclExpr, ExpressionDeclType_e::AnomClassDecl, line, column)
	{}

	ExpressionAnomClassDecl::~ExpressionAnomClassDecl()
	{}

	Bool
	ExpressionAnomClassDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	ExpressionAnomClassDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->functionList)
		{
			children.push_back(it.get());
		}
		for (auto& it : this->variableList)
		{
			children.push_back(it.get());
		}
		for (auto& it : this->constructorList)
		{
			children.push_back(it.get());
		}
		if (this->destructorDecl) {
			children.push_back(this->destructorDecl.get());
		}
		return children;
	}

	/**
	 * ExpressionPrimitiveTypeDecl
	 */

	ExpressionPrimitiveTypeDecl::ExpressionPrimitiveTypeDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::PrimitiveTypeExpr, ExpressionDeclType_e::PrimitiveType, line, column)
	{}

	ExpressionPrimitiveTypeDecl::~ExpressionPrimitiveTypeDecl()
	{}

	/**
	 * ExpressionConstantIdentifierDecl
	 */

	ExpressionConstantIdentifierDecl::ExpressionConstantIdentifierDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::IdentifierExpr, ExpressionDeclType_e::ConstantIdentifier, line, column)
		, startFromRoot(false)
	{}

	ExpressionConstantIdentifierDecl::~ExpressionConstantIdentifierDecl()
	{}

	/**
	 * ExpressionArrayInitDecl
	 */

	ExpressionArrayInitDecl::ExpressionArrayInitDecl(const U32 line, const U32 column)
		: ExpressionDecl(AstNodeType_e::ArrayInitExpr, ExpressionDeclType_e::ArrayInit, line, column)
	{}

	ExpressionArrayInitDecl::~ExpressionArrayInitDecl()
	{}

	/**
	 * ExpressionMatchWhenDecl
	 */

	ExpressionMatchWhenDecl::ExpressionMatchWhenDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::MatchWhenExpr, line, column)
	{}

	ExpressionMatchWhenDecl::~ExpressionMatchWhenDecl()
	{}

	/**
	 * ExpressionFunctionParameterDecl
	 */

	ExpressionFunctionParameterDecl::ExpressionFunctionParameterDecl(const U32 line, const U32 column)
		: AstNodeIdentified(AstNodeType_e::ExprFunctionParameterDecl, line, column)
		, isEllipsis(false)
	{}

	ExpressionFunctionParameterDecl::~ExpressionFunctionParameterDecl()
	{}

	/**
	 * ExpressionNewBlock
	 */

	ExpressionNewBlockDecl::ExpressionNewBlockDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::NewBlockExpr, line, column)
	{}

	ExpressionNewBlockDecl::~ExpressionNewBlockDecl()
	{}

	/**
	 * ExpressionNewItemDecl
	 */

	ExpressionNewItemDecl::ExpressionNewItemDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::NewItemExpr, line, column)
	{}

	ExpressionNewItemDecl::~ExpressionNewItemDecl()
	{}

} } }