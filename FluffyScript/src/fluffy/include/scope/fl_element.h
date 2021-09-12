#pragma once
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace scope {
	/**
	 * Element
	 */

	class Element
	{
	public:
		Element(ast::AstNode* const node);
		~Element();
	};
} }