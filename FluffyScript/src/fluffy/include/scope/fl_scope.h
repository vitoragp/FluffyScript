#pragma once
#include "fl_defs.h"
#include "fl_collections.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace scope {
	class ScopeManager;
} }


namespace fluffy { namespace scope {
	/**
	 * FindResult_t
	 */

	struct FindResult_t
	{
		ast::AstNode* scope;
		NodeList nodeList;

		Bool foundResult;

		const FindResult_t&
		operator =(const FindResult_t& fr);
	};

	/**
	 * Scope
	 */

	class Scope
	{
	public:
		Scope(ScopeManager* scopeManager, ast::AstNode* const scope, ast::AstNode* const node);
		~Scope();

		ast::AstNode* const
		getNode();

		ast::AstNode* const
		getScope();

		const NodeMultiMap&
		toMap();

		FindResult_t
		findNodeById(const TString& identifier);

		void
		fetch();

		void
		assign(ast::AstNode* const scope, ast::AstNode* const node);

	private:
		ScopeManager*
		mScopeManager;

		ast::AstNode*
		mScope;

		ast::AstNode*
		mNode;

		NodeMultiMap
		mMap;

		Bool
		mFetched;

	};
} }