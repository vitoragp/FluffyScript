#pragma once
#include <memory>
#include <vector>
#include "fl_ast.h"
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class TypeDecl;
	class TypeDeclFunction;
	class BlockDecl;
} }

namespace fluffy { namespace ast { namespace pattern {
	class PatternDecl;
} } }

namespace fluffy { namespace ast { namespace expr {
	using ast::pattern::PatternDecl;

	using TypeDeclPtr								= std::unique_ptr<TypeDecl>;
	using TypeDeclPtrList							= std::vector<TypeDeclPtr>;

	using TypeDeclFunctionPtr						= std::unique_ptr<TypeDeclFunction>;

	using BlockDeclPtr								= std::unique_ptr<BlockDecl>;

	using ExpressionDeclPtr							= std::unique_ptr<class ExpressionDecl>;
	using ExpressionDeclPtrList						= std::vector<ExpressionDeclPtr>;

	using ExpressionFunctionParameterDeclPtr		= std::unique_ptr<class ExpressionFunctionParameterDecl>;
	using ExpressionFunctionParameterDeclPtrList	= std::vector<ExpressionFunctionParameterDeclPtr>;

	using ExpressionMatchWhenDeclPtr				= std::unique_ptr<class ExpressionMatchWhenDecl>;
	using ExpressionMatchWhenDeclPtrList			= std::vector<ExpressionMatchWhenDeclPtr>;

	using PatternDeclPtr							= std::unique_ptr<PatternDecl>;

	using ExpressionNewBlockDeclPtr					= std::unique_ptr<class ExpressionNewBlockDecl>;
	using ExpressionNewBlockDeclPtrList				= std::vector<ExpressionNewBlockDeclPtr>;

	using ExpressionNewItemDeclPtr					= std::unique_ptr<class ExpressionNewItemDecl>;
	using ExpressionNewItemDeclPtrList				= std::vector<ExpressionNewItemDeclPtr>;
	

	/**
	 * ExpressionDecl
	 */

	class ExpressionDecl : public AstNode
	{
	protected:
		ExpressionDecl(AstNodeType_e nodeType, ExpressionDeclType_e type, const U32 line, const U32 column)
			: AstNode(nodeType, line, column)
			, type(type)
		{}

	public:

		virtual ~ExpressionDecl()
		{}

		const ExpressionDeclType_e				type;
	};

	/**
	 * ExpressionTernaryDecl
	 */

	class ExpressionTernaryDecl : public ExpressionDecl
	{
	public:
		ExpressionTernaryDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::TernaryExpr, ExpressionDeclType_e::Ternary, line, column)
		{}

		virtual ~ExpressionTernaryDecl()
		{}

		ExpressionDeclPtr						conditionDecl;
		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
	};

	/**
	 * ExpressionBinaryDecl
	 */

	class ExpressionBinaryDecl : public ExpressionDecl
	{
	public:
		ExpressionBinaryDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::BinaryExpr, ExpressionDeclType_e::Binary, line, column)
			, op(TokenType_e::Unknown)
		{}

		virtual ~ExpressionBinaryDecl()
		{}

		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
		TokenType_e							op;
	};

	/**
	 * ExpressionUnaryDecl
	 */

	class ExpressionUnaryDecl : public ExpressionDecl
	{
	public:
		ExpressionUnaryDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::UnaryExpr, ExpressionDeclType_e::Unary, line, column)
			, unaryType(ExpressionUnaryType_e::Unknown)
			, op(TokenType_e::Unknown)
		{}

		virtual ~ExpressionUnaryDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		ExpressionUnaryType_e					unaryType;
		TokenType_e							op;
	};

	/**
	 * ExpressionAsDecl
	 */

	class ExpressionAsDecl : public ExpressionDecl
	{
	public:
		ExpressionAsDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::AsExpr, ExpressionDeclType_e::As, line, column)
		{}

		virtual ~ExpressionAsDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		TypeDeclPtr								typeDecl;

	};

	/**
	 * ExpressionIsDecl
	 */

	class ExpressionIsDecl : public ExpressionDecl
	{
	public:
		ExpressionIsDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::IsExpr, ExpressionDeclType_e::Is, line, column)
		{}

		virtual ~ExpressionIsDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		TypeDeclPtr								typeDecl;
	};

	/**
	 * ExpressionMatchDecl
	 */

	class ExpressionMatchDecl : public ExpressionDecl
	{
	public:
		ExpressionMatchDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::MatchExpr, ExpressionDeclType_e::Match, line, column)
		{}

		virtual ~ExpressionMatchDecl()
		{}

		ExpressionDeclPtr						exprDecl;
		ExpressionMatchWhenDeclPtrList			whenDeclList;
	};

	/**
	 * ExpressionFunctionDecl
	 */

	class ExpressionFunctionDecl : public ExpressionDecl
	{
	public:
		ExpressionFunctionDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::FunctionDeclExpr, ExpressionDeclType_e::FunctionDecl, line, column)
			, inferenceType(InferenceType_e::None)
		{}

		virtual ~ExpressionFunctionDecl()
		{}

		TypeDeclPtr								returnTypeDecl;
		ExpressionFunctionParameterDeclPtrList	parametersDecl;
		BlockDeclPtr							blockDecl;
		InferenceType_e							inferenceType;
	};

	/**
	 * ExpressionFunctionCall
	 */

	class ExpressionFunctionCall : public ExpressionDecl
	{
	public:
		ExpressionFunctionCall(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::FunctionCallExpr, ExpressionDeclType_e::FunctionCall, line, column)
		{}

		virtual ~ExpressionFunctionCall()
		{}

		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
	};

	/**
	 * ExpressionGenericDecl
	 */

	class ExpressionGenericDecl : public ExpressionDecl
	{
	public:
		ExpressionGenericDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::GenericFunctionCallExpr, ExpressionDeclType_e::GenericDecl, line, column)
		{}

		virtual ~ExpressionGenericDecl()
		{}

		TypeDeclPtrList							genericTypeList;
		ExpressionDeclPtr						lhs;
		ExpressionDeclPtr						rhs;
	};

	/**
	 * ExpressionIndexDecl
	 */

	class ExpressionIndexDecl : public ExpressionDecl
	{
	public:
		ExpressionIndexDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::IndexExpr, ExpressionDeclType_e::Index, line, column)
		{}

		virtual ~ExpressionIndexDecl()
		{}

		ExpressionDeclPtr						leftDecl;
		ExpressionDeclPtr						rightDecl;
	};

	/**
	 * ExpressionNewDecl
	 */

	class ExpressionNewDecl : public ExpressionDecl
	{
	public:
		ExpressionNewDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::NewExpr, ExpressionDeclType_e::New, line, column)
		{}

		virtual ~ExpressionNewDecl()
		{}

		TypeDeclPtr								objectTypeDecl;
		ExpressionDeclPtr						expressionDecl;
		ExpressionNewBlockDeclPtr				objectInitBlockDecl;
	};

	/**
	 * ExpressionConstantBoolDecl
	 */

	class ExpressionConstantBoolDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantBoolDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ConstantBoolExpr, ExpressionDeclType_e::ConstantBool, line, column)
			, valueDecl(false)
		{}

		virtual ~ExpressionConstantBoolDecl()
		{}

		Bool									valueDecl;
	};

	/**
	 * ExpressionConstantIntegerDecl
	 */

	class ExpressionConstantIntegerDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantIntegerDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ConstantIntegerExpr, ExpressionDeclType_e::ConstantInteger, line, column)
			, valueDecl(0)
			, valueType(TypeDeclID_e::Unknown)
		{}

		virtual ~ExpressionConstantIntegerDecl()
		{}

		long long int							valueDecl;
		TypeDeclID_e							valueType;
	};

	/**
	 * ExpressionConstantRealDecl
	 */

	class ExpressionConstantRealDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantRealDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ConstantRealExpr, ExpressionDeclType_e::ConstantReal, line, column)
			, valueDecl(0.0)
			, valueType(TypeDeclID_e::Unknown)
		{}

		virtual ~ExpressionConstantRealDecl()
		{}

		double									valueDecl;
		TypeDeclID_e							valueType;
	};

	/**
	 * ExpressionConstantStringDecl
	 */

	class ExpressionConstantStringDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantStringDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ConstantStringExpr, ExpressionDeclType_e::ConstantString, line, column)
		{}

		virtual ~ExpressionConstantStringDecl()
		{}

		String									valueDecl;
	};

	/**
	 * ExpressionConstantCharDecl
	 */

	class ExpressionConstantCharDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantCharDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ConstantCharExpr, ExpressionDeclType_e::ConstantChar, line, column)
			, valueDecl(0)
		{}

		virtual ~ExpressionConstantCharDecl()
		{}

		I8										valueDecl;
	};

	/**
	 * ExpressionConstantNullDecl
	 */

	class ExpressionConstantNullDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantNullDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ConstantNullExpr, ExpressionDeclType_e::ConstantNull, line, column)
		{}

		virtual ~ExpressionConstantNullDecl()
		{}
	};
	
	/**
	 * ExpressionThisDecl
	 */

	class ExpressionThisDecl : public ExpressionDecl
	{
	public:
		ExpressionThisDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ThisExpr, ExpressionDeclType_e::This, line, column)
		{}

		virtual ~ExpressionThisDecl()
		{}
	};

	/**
	 * ExpressionSuperDecl
	 */

	class ExpressionSuperDecl : public ExpressionDecl
	{
	public:
		ExpressionSuperDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::SuperExpr, ExpressionDeclType_e::Super, line, column)
		{}

		virtual ~ExpressionSuperDecl()
		{}
	};

	/**
	 * ExpressionPrimitiveTypeDecl
	 */

	class ExpressionPrimitiveTypeDecl : public ExpressionDecl
	{
	public:
		ExpressionPrimitiveTypeDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::PrimitiveTypeExpr, ExpressionDeclType_e::PrimitiveType, line, column)
		{}

		virtual ~ExpressionPrimitiveTypeDecl()
		{}

		TypeDeclPtr							typeDecl;
	};

	/**
	 * ExpressionConstantIdentifierDecl
	 */

	class ExpressionConstantIdentifierDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantIdentifierDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::IdentifierExpr, ExpressionDeclType_e::ConstantIdentifier, line, column)
			, startFromRoot(false)
		{}

		virtual ~ExpressionConstantIdentifierDecl()
		{}

		TString									identifierDecl;
		Bool									startFromRoot;
	};

	/**
	 * ExpressionDeclMark
	 */

	class ExpressionDeclMark : public ExpressionDecl
	{
	public:
		ExpressionDeclMark(const U32 line, const U32 column, const U32 beginPosition, const U32 endPopsition)
			: ExpressionDecl(AstNodeType_e::MarkExpr, ExpressionDeclType_e::Mark, line, column)
			, beginPosition(beginPosition)
			, endPopsition(endPopsition)
		{}

		~ExpressionDeclMark()
		{}

		const U32								beginPosition;
		const U32								endPopsition;
	};

	/**
	 * ExpressionArrayInitDecl
	 */

	class ExpressionArrayInitDecl : public ExpressionDecl
	{
	public:
		ExpressionArrayInitDecl(const U32 line, const U32 column)
			: ExpressionDecl(AstNodeType_e::ArrayInitExpr, ExpressionDeclType_e::ArrayInit, line, column)
		{}

		~ExpressionArrayInitDecl()
		{}

		ExpressionDeclPtrList					arrayElementDeclList;
	};

	/**
	 * ExpressionMatchWhenDecl
	 */

	class ExpressionMatchWhenDecl : public AstNode
	{
	public:
		ExpressionMatchWhenDecl(U32 line, U32 column)
			: AstNode(AstNodeType_e::MatchWhenExprDecl, line, column)
		{}

		~ExpressionMatchWhenDecl()
		{}

		PatternDeclPtr							patternDecl;
		ExpressionDeclPtr						exprDecl;
	};

	/**
	 * ExpressionFunctionParameterDecl
	 */

	class ExpressionFunctionParameterDecl : public AstNodeIdentified
	{
	public:
		ExpressionFunctionParameterDecl(const U32 line, const U32 column)
			: AstNodeIdentified(AstNodeType_e::FunctionParameterDecl, line, column)
			, isEllipsis(false)
		{}

		~ExpressionFunctionParameterDecl()
		{}


		TypeDeclPtr								typeDecl;
		Bool									isEllipsis;
	};

	/**
	 * ExpressionNewBlock
	 */

	class ExpressionNewBlockDecl : public AstNode
	{
	public:
		ExpressionNewBlockDecl(U32 line, U32 column)
			: AstNode(AstNodeType_e::NewBlockDecl, line, column)
		{}

		virtual ~ExpressionNewBlockDecl()
		{}

		ExpressionNewItemDeclPtrList			itemDeclList;
	};

	/**
	 * ExpressionNewItemDecl
	 */

	class ExpressionNewItemDecl : public AstNode
	{
	public:
		ExpressionNewItemDecl(U32 line, U32 column)
			: AstNode(AstNodeType_e::NewItemDecl, line, column)
		{}

		virtual ~ExpressionNewItemDecl()
		{}

		TString									identifier;
		ExpressionDeclPtr						expressionDecl;
	};

} } }