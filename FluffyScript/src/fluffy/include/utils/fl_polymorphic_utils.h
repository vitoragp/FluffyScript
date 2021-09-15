#pragma once
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace utils {
	/**
	 * PolymorphicUtils
	 */

	class PolymorphicUtils
	{
	public:
		static Bool
		canBe(ast::AstNode* nodeA, ast::AstNode* nodeB);
	};
} }