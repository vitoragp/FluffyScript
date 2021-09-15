#pragma once
#include "fl_collections.h"
#include "attributes\fl_attribute.h"
namespace fluffy { namespace attributes {
	/**
	 * IncludeEntry_s
	 */

	struct IncludeEntry_s
	{
		ast::AstNode* const scope;
		ast::AstNode* const node;
	};

	typedef std::vector<IncludeEntry_s> IncludeEntryList;
	typedef std::unordered_multimap<const TString, IncludeEntry_s, TStringHash, TStringEqual> IncludeEntryMultiMap;

	/**
	 * IncludedScope
	 */

	class IncludedScope : public AttributeTemplate<AttributeType_e::IncludedScope>
	{
	public:
		IncludedScope();
		virtual ~IncludedScope();

		void
		insertIncludedNode(const TString& identifier, ast::AstNode* const scope, ast::AstNode* const node);

		void
		insertWeakIncludedNode(const TString& identifier, ast::AstNode* const scope, ast::AstNode* const node);

		void
		insertTraitDefinitionNode(const TString& identifier,  ast::AstNode* const node);

		ast::AstNode* const
		findScopeFromItem(ast::AstNode* const node);

		IncludeEntryList
		findInclude(const TString& identifier);

		const NodeList
		traitDefinitionList();
		
		const NodeMultiMap&
		traitDefinitionMap();

		IncludeEntryList
		weakIncludeList();
		
		const IncludeEntryMultiMap&
		weakIncludeMap();

		IncludeEntryList
		includeList();

		const IncludeEntryMultiMap&
		includeMap();

	private:
		IncludeEntryMultiMap
		mIncludedMap;

		IncludeEntryMultiMap
		mWeakIncludedMap;

		NodeMultiMap
		mTraitDefinitionMap;
	};
} }