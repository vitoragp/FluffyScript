#pragma once
#include "scope\fl_scope_manager.h"
namespace fluffy { namespace transformations {
	/**
	 * ResolveInclude
	 */

	class ResolveInclude : public scope::NodeProcessor
	{
	public:
		ResolveInclude();
		virtual ~ResolveInclude();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node) override;

	private:
		void
		processIncludeItem(ast::BaseIncludeItemDecl* const baseIncludeItemDecl, scope::ScopeManager* const scopeManager, ast::IncludeDecl* const includeDecl, scope::Scope& includeRootScope);

		Bool
		checkNodeVisibility(scope::ScopeManager* const scopeManager, ast::AstNode* node);

	private:
		std::vector<ast::WeakIncludeItemDecl*>
		mWeakIncludeItemList;
	};
} }