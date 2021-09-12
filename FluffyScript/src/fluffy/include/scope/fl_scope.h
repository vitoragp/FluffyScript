#pragma once
#include "fl_defs.h"
#include "fl_collection.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace scope {
	/**
	 * Scope
	 */

	class Scope
	{
	public:
		Scope(ast::AstNode* const scope, ast::AstNode* const node);
		~Scope();

		ast::AstNode* const
		getNode();

		ast::AstNode* const
		getScope();

		NodeList
		findNodeById(const TString& identifier);

		void
		fetch();

	private:
		ast::AstNode* const
		mScope;

		ast::AstNode* const
		mNode;

		NodeMultiMap
		mMap;

		Bool
		mFetched;

	};
} }