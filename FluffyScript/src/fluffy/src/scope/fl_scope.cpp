#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "scope\fl_scope.h"
#include "scope\fl_scope_manager.h"
#include "utils\fl_scope_utils.h"
#include "fl_exceptions.h"
namespace fluffy { namespace scope {
	/**
	 * Scope
	 */

	Scope::Scope(ast::AstNode* const scope, ast::AstNode* const node)
		: mScope(scope)
		, mNode(node)
		, mFetched(false)
	{}

	Scope::~Scope()
	{}

	ast::AstNode* const
	Scope::getNode()
	{
		return mNode;
	}

	ast::AstNode* const
	Scope::getScope()
	{
		return mScope;
	}

	NodeList
	Scope::findNodeById(const TString& identifier)
	{
		NodeList children;
		if (!mFetched) {
			fetch();
		}
		auto res = mMap.equal_range(identifier);
		while (res.first != res.second) {
			children.emplace_back(res.first->second);
			res.first++;
		}
		return children;
	}

	void
	Scope::fetch()
	{
		mMap = utils::ScopeUtils::getPositionalSimplifiedNodeChildrenMap(mScope, mNode);
		mFetched = true;
	}
} }