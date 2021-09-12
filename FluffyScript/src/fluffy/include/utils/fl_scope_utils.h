#pragma once
#include "fl_defs.h"
#include "fl_collection.h"
namespace fluffy { namespace utils {
	/**
	 * ScopeUtils
	 */

	class ScopeUtils
	{
	public:
		/*
		static NodeMultiMap
		getNodeChildrenMap(ast::AstNode* const node);

		static NodeMultiMapList
		getSimplifiedScopeChildrenMap(ast::AstNode* const node);
		*/

		static NodeMultiMap
		getPositionalSimplifiedNodeChildrenMap(ast::AstNode* const parent, ast::AstNode* const node);
	};
} }