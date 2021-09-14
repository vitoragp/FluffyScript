#pragma once
#include <memory>
#include "fl_defs.h"
#include "fl_string.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace attributes {
	class Attribute;
} }

namespace fluffy {
	/**
	 * TAttributeHash
	 */

	class TAttributeHash {
	public:
		size_t operator() (AttributeType_e const& key) const
		{
			return static_cast<size_t>(key);
		}
	};

	/**
	 * TAttributeEqual
	 */

	class TAttributeEqual {
	public:
		size_t operator() (AttributeType_e const& a, AttributeType_e const& b) const
		{
			return static_cast<size_t>(a) == static_cast<size_t>(b);
		}
	};

	/**
	 * Typedefs auxiliares
	 */

	typedef std::vector<ast::AstNode*> NodeList;

	typedef std::unordered_multimap<const TString, ast::AstNode* const, TStringHash, TStringEqual> NodeMultiMap;
	typedef std::vector<NodeMultiMap> NodeMultiMapList;

	typedef std::unique_ptr<attributes::Attribute> AttributePtr;
	typedef std::unordered_map<AttributeType_e, AttributePtr, TAttributeHash, TAttributeEqual> AttributeMap;
}