#pragma once
#include <memory>
#include "fl_ast.h"
#include "fl_ast_type.h"
#include "fl_ast_pattern.h"
#include "fl_ast_block.h"
#include "fl_ast_expr.h"
#include "..\\fl_defs.h"

namespace fluffy { namespace ast { namespace expr { 
	class ExpressionDecl;
} } }

namespace fluffy { namespace ast { namespace stmt {
	using expr::ExpressionDecl;
	using pattern::PatternDecl;

	using PatternDeclPtr				= std::unique_ptr<PatternDecl>;
	using PatternDeclPtrList			= std::vector<PatternDeclPtr>;

	using MatchWhenDeclPtr				= std::unique_ptr<class MatchWhenDecl>;
	using MatchWhenDeclPtrList			= std::vector<MatchWhenDeclPtr>;

	using ExpressionDeclPtr				= std::unique_ptr<ExpressionDecl>;
	using BlockDeclPtr					= std::unique_ptr<BlockDecl>;
	using TypeDeclPtr					= std::unique_ptr<TypeDecl>;
			
	using StmtForInitDeclPtr			= std::unique_ptr<class StmtForInitDecl>;

	using StmtTryCatchBlockDeclPtr		= std::unique_ptr<class StmtTryCatchBlockDecl>;
	using StmtTryCatchBlockDeclPtrList	= std::vector<StmtTryCatchBlockDeclPtr>;	

	/**
	 * StmtDecl
	 */

	class StmtDecl : public AstNode
	{
	protected:
		StmtDecl(StmtDeclType_e type, U32 line, U32 column)
			: AstNode(line, column)
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
			: StmtDecl(StmtDeclType_e::If, line, column)
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
			: StmtDecl(StmtDeclType_e::IfLet, line, column)
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
			 : StmtDecl(StmtDeclType_e::For, line, column)
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
			 : StmtDecl(StmtDeclType_e::While, line, column)
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
			 : StmtDecl(StmtDeclType_e::DoWhile, line, column)
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
			 : StmtDecl(StmtDeclType_e::Match, line, column)
		 {}

		 virtual ~StmtMatchDecl()
		 {}

		 ExpressionDeclPtr					conditionExprDecl;
		 MatchWhenDeclPtrList				whenDeclList;
	 };

	 /**
	 * StmtReturnDecl
	 */

	 class StmtReturnDecl : public StmtDecl
	 {
	 public:
		 StmtReturnDecl(U32 line, U32 column)
			 : StmtDecl(StmtDeclType_e::Return, line, column)
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
			 : StmtDecl(StmtDeclType_e::Continue, line, column)
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
			 : StmtDecl(StmtDeclType_e::Break, line, column)
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
			 : StmtDecl(StmtDeclType_e::Goto, line, column)
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
			 : StmtDecl(StmtDeclType_e::Label, line, column)
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
			 : StmtDecl(StmtDeclType_e::Try, line, column)
		 {}

		 virtual ~StmtTryDecl()
		 {}

		 BlockDeclPtr						blockDecl;
		 StmtTryCatchBlockDeclPtrList		catchDeclList;
	 };

	 /**
	 * StmtPanicDecl
	 */

	 class StmtPanicDecl : public StmtDecl
	 {
	 public:
		 StmtPanicDecl(U32 line, U32 column)
			 : StmtDecl(StmtDeclType_e::Panic, line, column)
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
			 : StmtDecl(StmtDeclType_e::Variable, line, column)
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
			 : StmtDecl(StmtDeclType_e::Expr, line, column)
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
			 : AstNode(line, column)
		 {}

		 virtual ~StmtForInitDecl()
		 {}

		 TString							identifier;
		 TypeDeclPtr						type;
		 ExpressionDeclPtr					initExpression;
	 };

	 /**
	  * MatchWhenDecl
	  */

	 class MatchWhenDecl : public AstNode
	 {
	 public:
		 MatchWhenDecl(U32 line, U32 column)
			 : AstNode(line, column)
		 {}

		 virtual ~MatchWhenDecl()
		 {}

		 PatternDeclPtr						patternDecl;		 
		 BlockDeclPtr						blockDecl;
	 };

	 /**
	  * StmtTryCatchBlockDecl
	  */

	 class StmtTryCatchBlockDecl : public AstNode
	 {
	 public:
		 StmtTryCatchBlockDecl(U32 line, U32 column)
			 : AstNode(line, column)
		 {}

		 virtual ~StmtTryCatchBlockDecl()
		 {}

		 TypeDeclPtr						typeDecl;
		 TString							identifier;
		 BlockDeclPtr						blockDecl;
	 };
} } }