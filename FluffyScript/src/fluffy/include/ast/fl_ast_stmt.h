#pragma once
#include <memory>
#include "fl_ast.h"
#include "fl_ast_type.h"
#include "fl_ast_pattern.h"
#include "fl_ast_block.h"
#include "fl_ast_expr.h"
#include "fl_defs.h"

namespace fluffy { namespace ast { namespace expr {
			class ExpressionDecl;
} } }

namespace fluffy { namespace ast { namespace stmt {
	using expr::ExpressionDecl;
	using pattern::PatternDecl;

	using PatternDeclPtr			= std::unique_ptr<PatternDecl>;
	using PatternDeclPtrList		= std::vector<PatternDeclPtr>;

	using MatchWhenStmtDeclPtr		= std::unique_ptr<class StmtMatchWhenDecl>;
	using MatchWhenStmtDeclPtrList	= std::vector<MatchWhenStmtDeclPtr>;

	using ExpressionDeclPtr			= std::unique_ptr<ExpressionDecl>;
	using BlockDeclPtr				= std::unique_ptr<BlockDecl>;
	using TypeDeclPtr				= std::unique_ptr<TypeDecl>;

	using StmtForInitDeclPtr		= std::unique_ptr<class StmtForInitDecl>;

	using StmtCatchBlockDeclPtr		= std::unique_ptr<class StmtCatchBlockDecl>;
	using StmtCatchBlockDeclPtrList = std::vector<StmtCatchBlockDeclPtr>;

	/**
	 * StmtDecl
	 */

	class StmtDecl : public AstNode
	{
	protected:
		StmtDecl(AstNodeType_e nodeType, StmtDeclType_e type, U32 line, U32 column);

	public:
		virtual ~StmtDecl();

		const StmtDeclType_e					type;
	};

	/**
	 * StmtIfDecl
	 */

	class StmtIfDecl : public StmtDecl
	{
	public:
		StmtIfDecl(U32 line, U32 column);
		virtual ~StmtIfDecl();

		ExpressionDeclPtr					conditionExprDecl;
		BlockDeclPtr						ifBlockDecl;
		BlockDeclPtr						elseBlockDecl;
	};

	/**
	 * StmtIfLetDecl
	 */

	class StmtIfLetDecl : public StmtDecl
	{
	public:
		StmtIfLetDecl(U32 line, U32 column);
		virtual ~StmtIfLetDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		PatternDeclPtr						patternDecl;
		ExpressionDeclPtr					expressionDecl;
		BlockDeclPtr						ifBlockDecl;
		BlockDeclPtr						elseBlockDecl;
	};

	/**
	 * StmtForDecl
	 */

	class StmtForDecl : public StmtDecl
	{
	public:
		StmtForDecl(U32 line, U32 column);
		virtual ~StmtForDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		StmtForInitDeclPtr					initStmtDecl;
		ExpressionDeclPtr					initExprDecl;
		ExpressionDeclPtr					conditionExprDecl;
		ExpressionDeclPtr					updateExprDecl;
		BlockDeclPtr						blockDecl;
	};

	/**
	 * StmtWhileDecl
	 */

	class StmtWhileDecl : public StmtDecl
	{
	public:
		StmtWhileDecl(U32 line, U32 column);
		virtual ~StmtWhileDecl();

		ExpressionDeclPtr					conditionExprDecl;
		BlockDeclPtr						blockDecl;
	};

	/**
	 * StmtDoWhileDecl
	 */

	class StmtDoWhileDecl : public StmtDecl
	{
	public:
		StmtDoWhileDecl(U32 line, U32 column);
		virtual ~StmtDoWhileDecl();

		ExpressionDeclPtr					conditionExprDecl;
		BlockDeclPtr						blockDecl;
	};

	/**
	 * StmtMatchDecl
	 */

	class StmtMatchDecl : public StmtDecl
	{
	public:
		StmtMatchDecl(U32 line, U32 column);
		virtual ~StmtMatchDecl();

		ExpressionDeclPtr					conditionExprDecl;
		MatchWhenStmtDeclPtrList			whenDeclList;
	};

	/**
	 * StmtReturnDecl
	 */

	class StmtReturnDecl : public StmtDecl
	{
	public:
		StmtReturnDecl(U32 line, U32 column);
		virtual ~StmtReturnDecl();

		ExpressionDeclPtr					exprDecl;
	};

	/**
	 * StmtContinueDecl
	 */

	class StmtContinueDecl : public StmtDecl
	{
	public:
		StmtContinueDecl(U32 line, U32 column);
		virtual ~StmtContinueDecl();
	};

	/**
	 * StmtBreakDecl
	 */

	class StmtBreakDecl : public StmtDecl
	{
	public:
		StmtBreakDecl(U32 line, U32 column);
		virtual ~StmtBreakDecl();
	};

	/**
	 * StmtGotoDecl
	 */

	class StmtGotoDecl : public StmtDecl
	{
	public:
		StmtGotoDecl(U32 line, U32 column);
		virtual ~StmtGotoDecl();

		TString							labelIdentifier;
	};

	/**
	 * StmtLabelDecl
	 */

	class StmtLabelDecl : public StmtDecl
	{
	public:
		StmtLabelDecl(U32 line, U32 column);
		virtual ~StmtLabelDecl();

	};

	/**
	 * StmtTryDecl
	 */

	class StmtTryDecl : public StmtDecl
	{
	public:
		StmtTryDecl(U32 line, U32 column);
		virtual ~StmtTryDecl();

		BlockDeclPtr						blockDecl;
		StmtCatchBlockDeclPtrList			catchDeclList;
	};

	/**
	 * StmtPanicDecl
	 */

	class StmtPanicDecl : public StmtDecl
	{
	public:
		StmtPanicDecl(U32 line, U32 column);
		virtual ~StmtPanicDecl();

		ExpressionDeclPtr					exprDecl;
	};

	/**
	 * StmtVariableDecl
	 */

	class StmtVariableDecl : public StmtDecl
	{
	public:
		StmtVariableDecl(U32 line, U32 column);
		virtual ~StmtVariableDecl();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		Bool								isShared;
		Bool								isUnique;
		Bool								isConst;
		Bool								isReference;
		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpr;
		PatternDeclPtr						patternDecl;
	};

	/**
	 * StmtExprDecl
	 */

	class StmtExprDecl : public StmtDecl
	{
	public:
		StmtExprDecl(U32 line, U32 column);
		virtual ~StmtExprDecl();

		ExpressionDeclPtr					exprDecl;
	};

	/**
	 * StmtForInitDecl
	 */

	class StmtForInitDecl : public AstNode
	{
	public:
		StmtForInitDecl(U32 line, U32 column);
		virtual ~StmtForInitDecl();

		TypeDeclPtr							typeDecl;
		ExpressionDeclPtr					initExpr;
	};

	/**
	 * StmtMatchWhenDecl
	 */

	class StmtMatchWhenDecl : public AstNode
	{
	public:
		StmtMatchWhenDecl(U32 line, U32 column);
		virtual ~StmtMatchWhenDecl();

		PatternDeclPtr						patternDecl;
		BlockDeclPtr						blockDecl;
	};

	/**
	 * StmtCatchBlockDecl
	 */

	class StmtCatchBlockDecl : public AstNode
	{
	public:
		StmtCatchBlockDecl(U32 line, U32 column);
		virtual ~StmtCatchBlockDecl();

		TypeDeclPtr							typeDecl;
		PatternDeclPtr						patternDecl;
		BlockDeclPtr						blockDecl;
	};
} } }