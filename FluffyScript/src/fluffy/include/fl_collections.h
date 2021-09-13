#pragma once
#include "fl_defs.h"
#include "fl_string.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy {
	/**
	 * Typedefs auxiliares
	 */

	typedef std::vector<ast::AstNode*> NodeList;

	typedef std::unordered_multimap<const TString, ast::AstNode* const, TStringHash, TStringEqual> NodeMultiMap;
	typedef std::vector<NodeMultiMap> NodeMultiMapList;
}