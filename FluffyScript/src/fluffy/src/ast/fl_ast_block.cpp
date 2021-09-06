#include "ast\fl_ast_stmt.h"
#include "ast\fl_ast_block.h"

namespace fluffy { namespace ast {

	/**
	 * BlockDecl
	 */

	BlockDecl::BlockDecl(U32 line, U32 column)
		: AstNode(AstNodeType_e::Block, line, column)
		, beginPosition(0)
		, endPosition(0)
	{}

	BlockDecl::BlockDecl(U32 beginPosition, U32 endPosition, U32 line, U32 column)
		: AstNode(AstNodeType_e::Block, line, column)
		, beginPosition(beginPosition)
		, endPosition(endPosition)
	{}

	BlockDecl::~BlockDecl()
	{}

	Bool
	BlockDecl::hasChildren()
	{
		return true;
	}

	std::vector<AstNode*>
	BlockDecl::getChildren()
	{
		std::vector<AstNode*> children;
		for (auto& it : this->stmtList)
		{
			std::vector<AstNode*> stmtChildren = it->getChildren();
			children.insert(children.end(), stmtChildren.begin(), stmtChildren.end());
		}
		return children;
	}
} }