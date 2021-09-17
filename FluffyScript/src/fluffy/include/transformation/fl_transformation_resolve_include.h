#pragma once
#include "scope\fl_scope_manager.h"
#include "attributes\fl_included_scope.h"
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
		onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node) override;

	private:
		void
		processIncludeItem(ast::IncludeItemDecl* const includeItemDecl, ast::IncludeDecl* const includeDecl, scope::Scope& includeRootScope);

		void
		validateIncludedNode(fluffy::ast::AstNode* const includedNode, fluffy::ast::AstNode* const includedItem);

		void
		validateClassMember(fluffy::ast::AstNode* const includedNode, fluffy::ast::AstNode* const includedItem);

		Bool
		checkNodeVisibility(ast::AstNode* node);

		void
		validateScope(ast::AstNode* const node);

	private:
		scope::ScopeManager*
		mScopeManager;

		attributes::IncludedScope*
		mIncludedScope;

		Bool
		mCreatedIncludedScope;
	};
} }