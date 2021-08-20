#pragma once
#include "fl_ast.h"
namespace fluffy { namespace ast { namespace pattern {
	/**
	 * PatternDecl
	 */

	class PatternDecl : public AstNode
	{
	protected:
		PatternDecl(PatterType_e type, U32 line, U32 column)
			: AstNode(line, column)
			, type(type)
		{}

	public:
		virtual ~PatternDecl()
		{}

		const PatterType_e						type;
	};
} } }