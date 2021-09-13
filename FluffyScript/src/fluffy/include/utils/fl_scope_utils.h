#pragma once
#include "fl_defs.h"
#include "fl_collections.h"

namespace fluffy { namespace scope {
	class ScopeManager;
} }

namespace fluffy { namespace utils {
	/**
	 * ScopeUtils
	 */

	class ScopeUtils
	{
	public:
		static NodeMultiMap
		getPositionalSimplifiedNodeChildrenMap(scope::ScopeManager* const scopeManager, ast::AstNode* const parent, ast::AstNode* const node);

		static ast::AstNode*
		resolveExtendsClass(scope::ScopeManager* scopeManager, ast::AstNode* const node);
	};
} }