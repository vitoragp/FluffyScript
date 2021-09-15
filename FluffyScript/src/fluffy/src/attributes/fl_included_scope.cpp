#include "fl_exceptions.h"
#include "ast\fl_ast.h"
#include "attributes\fl_included_scope.h"
namespace fluffy { namespace attributes {
	/**
	 * IncludedScope
	 */

	IncludedScope::IncludedScope()
	{}

	IncludedScope::~IncludedScope()
	{}

	void
	IncludedScope::insertIncludedNode(const TString& identifier, ast::AstNode* const scope, ast::AstNode* const node)
	{
		mIncludedMap.emplace(identifier, IncludeEntry_s { scope, node });
	}

	void
	IncludedScope::insertWeakIncludedNode(const TString& identifier, ast::AstNode* const scope, ast::AstNode* const node)
	{
		mWeakIncludedMap.emplace(identifier, IncludeEntry_s{ scope, node });
	}

	void
	IncludedScope::insertTraitDefinitionNode(const TString& identifier,  ast::AstNode* const node)
	{
		mTraitDefinitionMap.emplace(identifier, node);
	}

	ast::AstNode* const
	IncludedScope::findScopeFromItem(ast::AstNode* const node)
	{
		auto includeList = findInclude(node->identifier);

		if (!includeList.size())
		{
			throw exceptions::custom_exception(
				"Failed to retrieve included scope from '%s",
				node->identifier.str()
			);
		}

		for (auto it : includeList)
		{
			if (it.node == node)
			{
				return it.scope;
			}
		}
		return nullptr;
	}

	IncludeEntryList
	IncludedScope::findInclude(const TString& identifier)
	{
		IncludeEntryList resultList;
		auto res = mIncludedMap.equal_range(identifier);
		if (res.first != res.second)
		{
			while (res.first != res.second)
			{
				resultList.push_back(res.first->second);
				res.first++;
			}
		}
		else
		{
			res = mWeakIncludedMap.equal_range(identifier);
			while (res.first != res.second)
			{
				resultList.push_back(res.first->second);
				res.first++;
			}
		}
		return resultList;
	}

	const NodeList
	IncludedScope::traitDefinitionList()
	{
		NodeList list;
		for (const auto& it : mTraitDefinitionMap)
		{
			list.emplace_back(it.second);
		}
		return list;
	}
		
	const NodeMultiMap&
	IncludedScope::traitDefinitionMap()
	{
		return mTraitDefinitionMap;
	}

	IncludeEntryList
	IncludedScope::weakIncludeList()
	{
		IncludeEntryList list;
		for (const auto& it : mWeakIncludedMap)
		{
			list.emplace_back(it.second);
		}
		return list;
	}
		
	const IncludeEntryMultiMap&
	IncludedScope::weakIncludeMap()
	{
		return mWeakIncludedMap;
	}

	IncludeEntryList
	IncludedScope::includeList()
	{
		IncludeEntryList list;
		for (const auto& it : mIncludedMap)
		{
			list.emplace_back(it.second);
		}
		return list;
	}

	const IncludeEntryMultiMap&
	IncludedScope::includeMap()
	{
		return mIncludedMap;
	}
} }