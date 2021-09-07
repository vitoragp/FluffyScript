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
		AstNode(const AstNodeType_e	nodeType, U32 line, U32 column);

	public:
		virtual ~AstNode();

		virtual Bool
		hasChildren();

		virtual std::vector<AstNode*>
		getChildren();

		virtual const I8*
		getIdentifier();

		const AstNodeType_e		nodeType;
		TString					identifier;
		U32						line;
		U32						column;
	};
} }