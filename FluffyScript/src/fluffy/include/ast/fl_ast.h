#pragma once
#include "fl_string.h"
#include "fl_defs.h"
namespace fluffy { namespace ast {
	/**
	 * AstNode
	 */

	class AstNode
	{
	protected:
		AstNode(const AstNodeType_e	nodeType, U32 line, U32 column)
			: nodeType(nodeType)
			, line(line)
			, column(column)
		{}

	public:
		virtual ~AstNode()
		{}

		const AstNodeType_e		nodeType;
		U32						line;
		U32						column;
	};

	/**
	 * AstNodeIdentified
	 */

	class AstNodeIdentified : public AstNode
	{
	public:
		AstNodeIdentified(const AstNodeType_e nodeType, U32 line, U32 column)
			: AstNode(nodeType, line, column)
		{}

		virtual ~AstNodeIdentified()
		{}

		TString					identifier;
	};
} }