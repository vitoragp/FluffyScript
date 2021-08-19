#pragma once
#include "..\fl_defs.h"
namespace fluffy { namespace ast {
	/**
	 * AstNode
	 */

	class AstNode
	{
	public:
		AstNode(U32 line, U32 column)
			: line(line)
			, column(column)
		{}

		virtual ~AstNode()
		{}

		U32						line;
		U32						column;
	};

	/**
	 * AstNodeIdentified
	 */

	class AstNodeIdentified : public AstNode
	{
	public:
		AstNodeIdentified(U32 line, U32 column)
			: AstNode(line, column)
		{}

		virtual ~AstNodeIdentified()
		{}

		String					identifier;
	};
} }