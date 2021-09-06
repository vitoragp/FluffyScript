#include <memory>
#include "ast\fl_ast.h"
#include "ast\fl_ast_stmt.h"
#include "ast\fl_ast_type.h"
#include "ast\fl_ast_pattern.h"
#include "ast\fl_ast_block.h"
#include "ast\fl_ast_expr.h"
#include "fl_defs.h"

namespace fluffy { namespace ast { namespace stmt {
	/**
	 * StmtDecl
	 */

	StmtDecl::StmtDecl(AstNodeType_e nodeType, StmtDeclType_e type, U32 line, U32 column)
		: AstNode(nodeType, line, column)
		, type(type)
	{}

	StmtDecl::~StmtDecl()
	{}


	/**
	 * StmtIfDecl
	 */

	StmtIfDecl::StmtIfDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::IfStmt, StmtDeclType_e::If, line, column)
	{}

	StmtIfDecl::~StmtIfDecl()
	{}

	/**
	 * StmtIfLetDecl
	 */

	StmtIfLetDecl::StmtIfLetDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::IfLetStmt, StmtDeclType_e::IfLet, line, column)
	{}

	StmtIfLetDecl::~StmtIfLetDecl()
	{}

	Bool
	StmtIfLetDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	StmtIfLetDecl::getChildren()
	{
		return std::move(this->patternDecl->getChildren());
	}

	/**
	 * StmtForDecl
	 */

	StmtForDecl::StmtForDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::ForStmt, StmtDeclType_e::For, line, column)
	{}

	StmtForDecl::~StmtForDecl()
	{}

	Bool
	StmtForDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	StmtForDecl::getChildren()
	{
		return std::move(this->initStmtDecl->getChildren());
	}

	/**
	 * StmtWhileDecl
	 */

	StmtWhileDecl::StmtWhileDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::WhileStmt, StmtDeclType_e::While, line, column)
	{}

	StmtWhileDecl::~StmtWhileDecl()
	{}

	/**
	 * StmtDoWhileDecl
	 */

	StmtDoWhileDecl::StmtDoWhileDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::DoWhileStmt, StmtDeclType_e::DoWhile, line, column)
	{}

	StmtDoWhileDecl::~StmtDoWhileDecl()
	{}

	/**
	 * StmtMatchDecl
	 */

	StmtMatchDecl::StmtMatchDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::MatchStmt, StmtDeclType_e::Match, line, column)
	{}

	StmtMatchDecl::~StmtMatchDecl()
	{}

	/**
	 * StmtReturnDecl
	 */

	StmtReturnDecl::StmtReturnDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::ReturnStmt, StmtDeclType_e::Return, line, column)
	{}

	StmtReturnDecl::~StmtReturnDecl()
	{}

	/**
	 * StmtContinueDecl
	 */

	StmtContinueDecl::StmtContinueDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::ContinueStmt, StmtDeclType_e::Continue, line, column)
	{}

	StmtContinueDecl::~StmtContinueDecl()
	{}

	/**
	 * StmtBreakDecl
	 */

	StmtBreakDecl::StmtBreakDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::BreakStmt, StmtDeclType_e::Break, line, column)
	{}

	StmtBreakDecl::~StmtBreakDecl()
	{}

	/**
	 * StmtGotoDecl
	 */

	StmtGotoDecl::StmtGotoDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::GotoStmt, StmtDeclType_e::Goto, line, column)
	{}

	StmtGotoDecl::~StmtGotoDecl()
	{}

	/**
	 * StmtLabelDecl
	 */

	StmtLabelDecl::StmtLabelDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::LabelStmt, StmtDeclType_e::Label, line, column)
	{}

	StmtLabelDecl::~StmtLabelDecl()
	{}

	/**
	 * StmtTryDecl
	 */

	StmtTryDecl::StmtTryDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::TryStmt, StmtDeclType_e::Try, line, column)
	{}

	StmtTryDecl::~StmtTryDecl()
	{}

	/**
	 * StmtPanicDecl
	 */

	StmtPanicDecl::StmtPanicDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::PanicStmt, StmtDeclType_e::Panic, line, column)
	{}

	StmtPanicDecl::~StmtPanicDecl()
	{}

	/**
	 * StmtVariableDecl
	 */

	StmtVariableDecl::StmtVariableDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::VariableStmt, StmtDeclType_e::Variable, line, column)
		, isShared(false)
		, isUnique(false)
		, isConst(false)
		, isReference(false)
	{}

	StmtVariableDecl::~StmtVariableDecl()
	{}

	Bool
	StmtVariableDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	StmtVariableDecl::getChildren()
	{
		if (this->patternDecl)
		{
			return this->patternDecl->getChildren();
		}
		return std::vector<AstNode*>();
	}

	/**
	 * StmtExprDecl
	 */

	StmtExprDecl::StmtExprDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::ExprStmt, StmtDeclType_e::Expr, line, column)
	{}

	StmtExprDecl::~StmtExprDecl()
	{}

	/**
	 * StmtForInitDecl
	 */

	StmtForInitDecl::StmtForInitDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::ForInitDecl, line, column)
	{}

	StmtForInitDecl::~StmtForInitDecl()
	{}

	/**
	 * MatchWhenStmtDecl
	 */

	MatchWhenStmtDecl::MatchWhenStmtDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::MatchWhenStmtDecl, line, column)
	{}

	MatchWhenStmtDecl::~MatchWhenStmtDecl()
	{}

	/**
	 * StmtCatchBlockDecl
	 */

	StmtCatchBlockDecl::StmtCatchBlockDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::CatchBlockStmtDecl, line, column)
	{}

	StmtCatchBlockDecl::~StmtCatchBlockDecl()
	{}

} } }