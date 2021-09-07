#pragma once
#include <memory>
#include <vector>
#include "fl_ast.h"
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class ClassFunctionDecl;
	class ClassVariableDecl;
	class ClassConstructorDecl;
	class ClassDestructorDecl;

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

	using ClassFunctionDeclPtr						= std::unique_ptr<ClassFunctionDecl>;
	using ClassFunctionDeclPtrList					= std::vector<ClassFunctionDeclPtr>;

	using ClassVariableDeclPtr						= std::unique_ptr<ClassVariableDecl>;
	using ClassVariableDeclPtrList					= std::vector<ClassVariableDeclPtr>;

	using ClassConstructorDeclPtr					= std::unique_ptr<ClassConstructorDecl>;
	using ClassConstructorDeclPtrList				= std::vector<ClassConstructorDeclPtr>;

	using ClassDestructorDeclPtr					= std::unique_ptr<ClassDestructorDecl>;
	using ClassDestructorDeclPtrList				= std::vector<ClassDestructorDeclPtr>;

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
		ExpressionDecl(AstNodeType_e nodeType, ExpressionDeclType_e type, const U32 line, const U32 column);

	public:
		virtual ~ExpressionDecl();

		const ExpressionDeclType_e				type;

		Bool									needGenericValidation;
		U32										beginPosition;
		U32										endPosition;
	};

	/**
	 * ExpressionTernaryDecl
	 */

	class ExpressionTernaryDecl : public ExpressionDecl
	{
	public:
		ExpressionTernaryDecl(const U32 line, const U32 column);
		virtual ~ExpressionTernaryDecl();

		ExpressionDeclPtr					conditionDecl;
		ExpressionDeclPtr					leftDecl;
		ExpressionDeclPtr					rightDecl;
	};

	/**
	 * ExpressionBinaryDecl
	 */

	class ExpressionBinaryDecl : public ExpressionDecl
	{
	public:
		ExpressionBinaryDecl(const U32 line, const U32 column);
		virtual ~ExpressionBinaryDecl();

		ExpressionDeclPtr					leftDecl;
		ExpressionDeclPtr					rightDecl;
		TokenType_e							op;
	};

	/**
	 * ExpressionUnaryDecl
	 */

	class ExpressionUnaryDecl : public ExpressionDecl
	{
	public:
		ExpressionUnaryDecl(const U32 line, const U32 column);
		virtual ~ExpressionUnaryDecl();

		ExpressionDeclPtr					exprDecl;
		ExpressionUnaryType_e				unaryType;
		TokenType_e							op;
	};

	/**
	 * ExpressionAsDecl
	 */

	class ExpressionAsDecl : public ExpressionDecl
	{
	public:
		ExpressionAsDecl(const U32 line, const U32 column);
		virtual ~ExpressionAsDecl();

		ExpressionDeclPtr						exprDecl;
		TypeDeclPtr								typeDecl;

	};

	/**
	 * ExpressionIsDecl
	 */

	class ExpressionIsDecl : public ExpressionDecl
	{
	public:
		ExpressionIsDecl(const U32 line, const U32 column);
		virtual ~ExpressionIsDecl();

		ExpressionDeclPtr						exprDecl;
		TypeDeclPtr								typeDecl;
	};

	/**
	 * ExpressionMatchDecl
	 */

	class ExpressionMatchDecl : public ExpressionDecl
	{
	public:
		ExpressionMatchDecl(const U32 line, const U32 column);
		virtual ~ExpressionMatchDecl();

		ExpressionDeclPtr						exprDecl;
		ExpressionMatchWhenDeclPtrList			whenDeclList;
	};

	/**
	 * ExpressionFunctionDecl
	 */

	class ExpressionFunctionDecl : public ExpressionDecl
	{
	public:
		ExpressionFunctionDecl(const U32 line, const U32 column);
		virtual ~ExpressionFunctionDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		TypeDeclPtr								returnTypeDecl;
		ExpressionFunctionParameterDeclPtrList	parametersDeclList;
		BlockDeclPtr							blockDecl;
		ExpressionDeclPtr						exprDecl;
		InferenceType_e							inferenceType;
	};

	/**
	 * ExpressionFunctionCall
	 */

	class ExpressionFunctionCall : public ExpressionDecl
	{
	public:
		ExpressionFunctionCall(const U32 line, const U32 column);
		virtual ~ExpressionFunctionCall();

		ExpressionDeclPtr						lhsDecl;
		ExpressionDeclPtr						rhsDecl;
	};

	/**
	 * ExpressionGenericCallDecl
	 */

	class ExpressionGenericCallDecl : public ExpressionDecl
	{
	public:
		ExpressionGenericCallDecl(const U32 line, const U32 column);
		virtual ~ExpressionGenericCallDecl();

		TypeDeclPtrList							genericTypeList;
		ExpressionDeclPtr						lhsDecl;
		ExpressionDeclPtr						rhsDecl;
	};

	/**
	 * ExpressionIndexDecl
	 */

	class ExpressionIndexDecl : public ExpressionDecl
	{
	public:
		ExpressionIndexDecl(const U32 line, const U32 column);
		virtual ~ExpressionIndexDecl();

		ExpressionDeclPtr						lhsDecl;
		ExpressionDeclPtr						rhsDecl;
	};

	/**
	 * ExpressionNewDecl
	 */

	class ExpressionNewDecl : public ExpressionDecl
	{
	public:
		ExpressionNewDecl(const U32 line, const U32 column);
		virtual ~ExpressionNewDecl();

		TypeDeclPtr								objTypeDecl;
		ExpressionDeclPtr						exprDecl;
		ExpressionNewBlockDeclPtr				objInitBlockDecl;
	};

	/**
	 * ExpressionConstantBoolDecl
	 */

	class ExpressionConstantBoolDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantBoolDecl(const U32 line, const U32 column);
		virtual ~ExpressionConstantBoolDecl();

		Bool									valueDecl;
	};

	/**
	 * ExpressionConstantIntegerDecl
	 */

	class ExpressionConstantIntegerDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantIntegerDecl(const U32 line, const U32 column);
		virtual ~ExpressionConstantIntegerDecl();

		long long int							valueDecl;
		TypeDeclID_e							valueType;
	};

	/**
	 * ExpressionConstantRealDecl
	 */

	class ExpressionConstantRealDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantRealDecl(const U32 line, const U32 column);
		virtual ~ExpressionConstantRealDecl();

		double									valueDecl;
		TypeDeclID_e							valueType;
	};

	/**
	 * ExpressionConstantStringDecl
	 */

	class ExpressionConstantStringDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantStringDecl(const U32 line, const U32 column);
		virtual ~ExpressionConstantStringDecl();

		String									valueDecl;
	};

	/**
	 * ExpressionConstantCharDecl
	 */

	class ExpressionConstantCharDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantCharDecl(const U32 line, const U32 column);
		virtual ~ExpressionConstantCharDecl();

		I8										valueDecl;
	};

	/**
	 * ExpressionConstantNullDecl
	 */

	class ExpressionConstantNullDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantNullDecl(const U32 line, const U32 column);
		virtual ~ExpressionConstantNullDecl();
	};
	
	/**
	 * ExpressionThisDecl
	 */

	class ExpressionThisDecl : public ExpressionDecl
	{
	public:
		ExpressionThisDecl(const U32 line, const U32 column);
		virtual ~ExpressionThisDecl();
	};

	/**
	 * ExpressionSuperDecl
	 */

	class ExpressionSuperDecl : public ExpressionDecl
	{
	public:
		ExpressionSuperDecl(const U32 line, const U32 column);
		virtual ~ExpressionSuperDecl();
	};

	/**
	 * ExpressionAnomClassDecl
	 */

	class ExpressionAnomClassDecl : public ExpressionDecl
	{
	public:
		ExpressionAnomClassDecl(const U32 line, const U32 column);
		virtual ~ExpressionAnomClassDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		TypeDeclPtr							baseReferencedDecl;
		ClassFunctionDeclPtrList			functionList;
		ClassVariableDeclPtrList			variableList;
		ClassConstructorDeclPtrList			constructorList;
		ClassDestructorDeclPtr				destructorDecl;

	};

	/**
	 * ExpressionPrimitiveTypeDecl
	 */

	class ExpressionPrimitiveTypeDecl : public ExpressionDecl
	{
	public:
		ExpressionPrimitiveTypeDecl(const U32 line, const U32 column);
		virtual ~ExpressionPrimitiveTypeDecl();

		TypeDeclPtr							typeDecl;
	};

	/**
	 * ExpressionConstantIdentifierDecl
	 */

	class ExpressionConstantIdentifierDecl : public ExpressionDecl
	{
	public:
		ExpressionConstantIdentifierDecl(const U32 line, const U32 column);
		virtual ~ExpressionConstantIdentifierDecl();

		Bool									startFromRoot;
	};

	/**
	 * ExpressionArrayInitDecl
	 */

	class ExpressionArrayInitDecl : public ExpressionDecl
	{
	public:
		ExpressionArrayInitDecl(const U32 line, const U32 column);
		virtual ~ExpressionArrayInitDecl();

		ExpressionDeclPtrList					arrayElementDeclList;
	};

	/**
	 * ExpressionMatchWhenDecl
	 */

	class ExpressionMatchWhenDecl : public AstNode
	{
	public:
		ExpressionMatchWhenDecl(U32 line, U32 column);
		virtual ~ExpressionMatchWhenDecl();

		PatternDeclPtr							patternDecl;
		ExpressionDeclPtr						exprDecl;
	};

	/**
	 * ExpressionFunctionParameterDecl
	 */

	class ExpressionFunctionParameterDecl : public AstNode
	{
	public:
		ExpressionFunctionParameterDecl(const U32 line, const U32 column);
		virtual ~ExpressionFunctionParameterDecl();

		TypeDeclPtr								typeDecl;
		Bool									isEllipsis;
		PatternDeclPtr							patternDecl;
	};

	/**
	 * ExpressionNewBlock
	 */

	class ExpressionNewBlockDecl : public AstNode
	{
	public:
		ExpressionNewBlockDecl(U32 line, U32 column);
		virtual ~ExpressionNewBlockDecl();

		ExpressionNewItemDeclPtrList			itemDeclList;
	};

	/**
	 * ExpressionNewItemDecl
	 */

	class ExpressionNewItemDecl : public AstNode
	{
	public:
		ExpressionNewItemDecl(U32 line, U32 column);
		virtual ~ExpressionNewItemDecl();

		ExpressionDeclPtr						exprDecl;
	};

} } }