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

	using PatternDeclPtr				= std::unique_ptr<PatternDecl>;
	using PatternDeclPtrList			= std::vector<PatternDeclPtr>;

	using MatchWhenStmtDeclPtr			= std::unique_ptr<class MatchWhenStmtDecl>;
	using MatchWhenStmtDeclPtrList		= std::vector<MatchWhenStmtDeclPtr>;

	using ExpressionDeclPtr				= std::unique_ptr<ExpressionDecl>;
	using BlockDeclPtr					= std::unique_ptr<BlockDecl>;
	using TypeDeclPtr					= std::unique_ptr<TypeDecl>;
			
	using StmtForInitDeclPtr			= std::unique_ptr<class StmtForInitDecl>;

	using StmtCatchBlockDeclPtr			= std::unique_ptr<class StmtCatchBlockDecl>;
	using StmtCatchBlockDeclPtrList		= std::vector<StmtCatchBlockDeclPtr>;	

	/**
	 * StmtDecl
	 */

	class StmtDecl : public AstNode
	{
	protected:
		StmtDecl(AstNodeType_e nodeType, StmtDeclType_e type, U32 line, U32 column)
			: AstNode(nodeType, line, column)
			, type(type)
		{}

	public:
		virtual ~StmtDecl()
		{}

		const StmtDeclType_e					type;
	};

	/**
	 * StmtIfDecl
	 */

	class StmtIfDecl : public StmtDecl
	{
	public:
		StmtIfDecl(U32 line, U32 column)
			: StmtDecl(AstNodeType_e::IfStmt, StmtDeclType_e::If, line, column)
		{}

		virtual ~StmtIfDecl()
		{}

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
		StmtIfLetDecl(U32 line, U32 column)
			: StmtDecl(AstNodeType_e::IfLetStmt, StmtDeclType_e::IfLet, line, column)
		{}

		virtual ~StmtIfLetDecl()
		{}

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
		 StmtForDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::ForStmt, StmtDeclType_e::For, line, column)
		 {}

		 virtual ~StmtForDecl()
		 {}

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
		 StmtWhileDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::WhileStmt, StmtDeclType_e::While, line, column)
		 {}

		 virtual ~StmtWhileDecl()
		 {}

		 ExpressionDeclPtr					conditionExprDecl;
		 BlockDeclPtr						blockDecl;
	 };

	 /**
	 * StmtDoWhileDecl
	 */

	 class StmtDoWhileDecl : public StmtDecl
	 {
	 public:
		 StmtDoWhileDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::DoWhileStmt, StmtDeclType_e::DoWhile, line, column)
		 {}

		 virtual ~StmtDoWhileDecl()
		 {}

		 ExpressionDeclPtr					conditionExprDecl;
		 BlockDeclPtr						blockDecl;
	 };

	 /**
	 * StmtMatchDecl
	 */

	 class StmtMatchDecl : public StmtDecl
	 {
	 public:
		 StmtMatchDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::MatchStmt, StmtDeclType_e::Match, line, column)
		 {}

		 virtual ~StmtMatchDecl()
		 {}

		 ExpressionDeclPtr					conditionExprDecl;
		 MatchWhenStmtDeclPtrList			whenDeclList;
	 };

	 /**
	 * StmtReturnDecl
	 */

	 class StmtReturnDecl : public StmtDecl
	 {
	 public:
		 StmtReturnDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::ReturnStmt, StmtDeclType_e::Return, line, column)
		 {}

		 virtual ~StmtReturnDecl()
		 {}

		 ExpressionDeclPtr					exprDecl;
	 };

	 /**
	 * StmtContinueDecl
	 */

	 class StmtContinueDecl : public StmtDecl
	 {
	 public:
		 StmtContinueDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::ContinueStmt, StmtDeclType_e::Continue, line, column)
		 {}

		 virtual ~StmtContinueDecl()
		 {}
	 };

	 /**
	 * StmtBreakDecl
	 */

	 class StmtBreakDecl : public StmtDecl
	 {
	 public:
		 StmtBreakDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::BreakStmt, StmtDeclType_e::Break, line, column)
		 {}

		 virtual ~StmtBreakDecl()
		 {}
	 };

	 /**
	 * StmtGotoDecl
	 */

	 class StmtGotoDecl : public StmtDecl
	 {
	 public:
		 StmtGotoDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::GotoStmt, StmtDeclType_e::Goto, line, column)
		 {}

		 virtual ~StmtGotoDecl()
		 {}

		 TString							labelIdentifier;
	 };

	 /**
	 * StmtLabelDecl
	 */

	 class StmtLabelDecl : public StmtDecl
	 {
	 public:
		 StmtLabelDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::LabelStmt, StmtDeclType_e::Label, line, column)
		 {}

		 virtual ~StmtLabelDecl()
		 {}

		 TString							identifier;
	 };

	 /**
	 * StmtTryDecl
	 */

	 class StmtTryDecl : public StmtDecl
	 {
	 public:
		 StmtTryDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::TryStmt, StmtDeclType_e::Try, line, column)
		 {}

		 virtual ~StmtTryDecl()
		 {}

		 BlockDeclPtr						blockDecl;
		 StmtCatchBlockDeclPtrList		catchDeclList;
	 };

	 /**
	 * StmtPanicDecl
	 */

	 class StmtPanicDecl : public StmtDecl
	 {
	 public:
		 StmtPanicDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::PanicStmt, StmtDeclType_e::Panic, line, column)
		 {}

		 virtual ~StmtPanicDecl()
		 {}

		 ExpressionDeclPtr					exprDecl;
	 };

	 /**
	 * StmtVariableDecl
	 */

	 class StmtVariableDecl : public StmtDecl
	 {
	 public:
		 StmtVariableDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::VariableStmt, StmtDeclType_e::Variable, line, column)
			 , isShared(false)
			 , isConst(false)
			 , isReference(false)
		 {}

		 virtual ~StmtVariableDecl()
		 {}

		 Bool								isShared;
		 TString							identifier;
		 Bool								isConst;
		 Bool								isReference;
		 TypeDeclPtr						typeDecl;
		 ExpressionDeclPtr					initExpression;
		 PatternDeclPtr						destructuringPatternDecl;
	 };

	 /**
	 * StmtExprDecl
	 */

	 class StmtExprDecl : public StmtDecl
	 {
	 public:
		 StmtExprDecl(U32 line, U32 column)
			 : StmtDecl(AstNodeType_e::ExprStmt, StmtDeclType_e::Expr, line, column)
		 {}

		 virtual ~StmtExprDecl()
		 {}

		 ExpressionDeclPtr					exprDecl;
	 };

	 /**
	  * StmtForInitDecl
	  */

	 class StmtForInitDecl : public AstNode
	 {
	 public:
		 StmtForInitDecl(U32 line, U32 column)
			 : AstNode(AstNodeType_e::ForInitDecl, line, column)
		 {}

		 virtual ~StmtForInitDecl()
		 {}

		 TString							identifier;
		 TypeDeclPtr						type;
		 ExpressionDeclPtr					initExpression;
	 };

	 /**
	  * MatchWhenStmtDecl
	  */

	 class MatchWhenStmtDecl : public AstNode
	 {
	 public:
		 MatchWhenStmtDecl(U32 line, U32 column)
			 : AstNode(AstNodeType_e::MatchWhenStmtDecl, line, column)
		 {}

		 virtual ~MatchWhenStmtDecl()
		 {}

		 PatternDeclPtr						patternDecl;		 
		 BlockDeclPtr						blockDecl;
	 };

	 /**
	  * StmtCatchBlockDecl
	  */

	 class StmtCatchBlockDecl : public AstNode
	 {
	 public:
		 StmtCatchBlockDecl(U32 line, U32 column)
			 : AstNode(AstNodeType_e::CatchBlockStmtDecl, line, column)
		 {}

		 virtual ~StmtCatchBlockDecl()
		 {}

		 TypeDeclPtr						typeDecl;
		 TString							identifier;
		 BlockDeclPtr						blockDecl;
	 };
} } }