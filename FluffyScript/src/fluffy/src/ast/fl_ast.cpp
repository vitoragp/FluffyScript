#include "ast\fl_ast.h"
#include "utils\fl_info_util.h"
#include "fl_string.h"
#include "fl_defs.h"

namespace fluffy { namespace utils {
	static U32
	g_totalNodeCount = 0;

	U32
	InfoUtil::getNodeCount()
	{
		return g_totalNodeCount;
	}

	void
	InfoUtil::resetNodeCount()
	{
		g_totalNodeCount = 0;
	}
} }

namespace fluffy { namespace ast {	

	/**
	 * AstNode
	 */

	AstNode::AstNode(const AstNodeType_e nodeType, U32 line, U32 column)
		: nodeType(nodeType)
		, identifier(getTokenString(nodeType))
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

	void
	AstNode::insertAttribute(attributes::Attribute* const attribute)
	{
		attributeMap.emplace(attribute->type, attribute);
	}

	void
	AstNode::removeAttribute(const AttributeType_e key)
	{
		attributeMap.erase(key);
	}
} }