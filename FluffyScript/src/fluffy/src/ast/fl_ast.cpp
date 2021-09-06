#include "ast\fl_ast.h"
#include "utils\fl_ast_utils.h"
#include "fl_string.h"
#include "fl_defs.h"

namespace fluffy { namespace utils {
	static U32
	g_totalNodeCount = 0;

	U32
	AstUtils::getNodeCount()
	{
		return g_totalNodeCount;
	}

	void
	AstUtils::resetNodeCount()
	{
		g_totalNodeCount = 0;
	}
} }

namespace fluffy { namespace ast {	

	/**
	 * AstNode
	 */

	AstNode::AstNode(const AstNodeType_e	nodeType, U32 line, U32 column)
		: nodeType(nodeType)
		, line(line)
		, column(column)
	{
		utils::g_totalNodeCount++;
	}

	AstNode::~AstNode()
	{}

	Bool
	AstNode::hasChildren()
	{
		return false;
	}

	std::vector<AstNode*>
	AstNode::getChildren()
	{
		return std::vector<AstNode*>();
	}

	const I8*
	AstNode::getIdentifier()
	{
		return "";
	}

	/**
	 * AstNodeIdentified
	 */


	AstNodeIdentified::AstNodeIdentified(const AstNodeType_e nodeType, U32 line, U32 column)
		: AstNode(nodeType, line, column)
	{}

	AstNodeIdentified::~AstNodeIdentified()
	{}

	const I8*
	AstNodeIdentified::getIdentifier()
	{
		return identifier.str();
	}
} }