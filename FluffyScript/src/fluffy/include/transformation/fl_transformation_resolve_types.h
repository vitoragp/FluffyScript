#pragma once
#include "scope\fl_scope_manager.h"
namespace fluffy { namespace transformations {
	/**
	 * ResolveTypes
	 */

	class ResolveTypes : public scope::NodeProcessor
	{
	public:
		ResolveTypes();
		virtual ~ResolveTypes();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node) override;

	private:
		void
		validateResult(fluffy::scope::FindResult_t& findResult, ast::AstNode* const namedType);

		Bool
		canBeType(ast::AstNode* const node);

		void
		validateScope(ast::AstNode* const node);

	private:
		scope::ScopeManager*
		mScopeManager;

	};
} }