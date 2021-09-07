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
		: StmtDecl(AstNodeType_e::StmtIf, StmtDeclType_e::If, line, column)
	{}

	StmtIfDecl::~StmtIfDecl()
	{}

	/**
	 * StmtIfLetDecl
	 */

	StmtIfLetDecl::StmtIfLetDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtIfLet, StmtDeclType_e::IfLet, line, column)
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
		: StmtDecl(AstNodeType_e::StmtFor, StmtDeclType_e::For, line, column)
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
		: StmtDecl(AstNodeType_e::StmtWhile, StmtDeclType_e::While, line, column)
	{}

	StmtWhileDecl::~StmtWhileDecl()
	{}

	/**
	 * StmtDoWhileDecl
	 */

	StmtDoWhileDecl::StmtDoWhileDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtDoWhile, StmtDeclType_e::DoWhile, line, column)
	{}

	StmtDoWhileDecl::~StmtDoWhileDecl()
	{}

	/**
	 * StmtMatchDecl
	 */

	StmtMatchDecl::StmtMatchDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtMatch, StmtDeclType_e::Match, line, column)
	{}

	StmtMatchDecl::~StmtMatchDecl()
	{}

	/**
	 * StmtReturnDecl
	 */

	StmtReturnDecl::StmtReturnDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtReturn, StmtDeclType_e::Return, line, column)
	{}

	StmtReturnDecl::~StmtReturnDecl()
	{}

	/**
	 * StmtContinueDecl
	 */

	StmtContinueDecl::StmtContinueDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtContinue, StmtDeclType_e::Continue, line, column)
	{}

	StmtContinueDecl::~StmtContinueDecl()
	{}

	/**
	 * StmtBreakDecl
	 */

	StmtBreakDecl::StmtBreakDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtBreak, StmtDeclType_e::Break, line, column)
	{}

	StmtBreakDecl::~StmtBreakDecl()
	{}

	/**
	 * StmtGotoDecl
	 */

	StmtGotoDecl::StmtGotoDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtGoto, StmtDeclType_e::Goto, line, column)
	{}

	StmtGotoDecl::~StmtGotoDecl()
	{}

	/**
	 * StmtLabelDecl
	 */

	StmtLabelDecl::StmtLabelDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtLabel, StmtDeclType_e::Label, line, column)
	{}

	StmtLabelDecl::~StmtLabelDecl()
	{}

	/**
	 * StmtTryDecl
	 */

	StmtTryDecl::StmtTryDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtTry, StmtDeclType_e::Try, line, column)
	{}

	StmtTryDecl::~StmtTryDecl()
	{}

	/**
	 * StmtPanicDecl
	 */

	StmtPanicDecl::StmtPanicDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtPanic, StmtDeclType_e::Panic, line, column)
	{}

	StmtPanicDecl::~StmtPanicDecl()
	{}

	/**
	 * StmtVariableDecl
	 */

	StmtVariableDecl::StmtVariableDecl(U32 line, U32 column)
		: StmtDecl(AstNodeType_e::StmtVariable, StmtDeclType_e::Variable, line, column)
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
		: StmtDecl(AstNodeType_e::StmtExpr, StmtDeclType_e::Expr, line, column)
	{}

	StmtExprDecl::~StmtExprDecl()
	{}

	/**
	 * StmtForInitDecl
	 */

	StmtForInitDecl::StmtForInitDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::StmtForInitDecl, line, column)
	{}

	StmtForInitDecl::~StmtForInitDecl()
	{}

	/**
	 * StmtMatchWhenDecl
	 */

	StmtMatchWhenDecl::StmtMatchWhenDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::StmtMatchWhenDecl, line, column)
	{}

	StmtMatchWhenDecl::~StmtMatchWhenDecl()
	{}

	/**
	 * StmtCatchBlockDecl
	 */

	StmtCatchBlockDecl::StmtCatchBlockDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::StmtCatchBlockDecl, line, column)
	{}

	StmtCatchBlockDecl::~StmtCatchBlockDecl()
	{}

} } }