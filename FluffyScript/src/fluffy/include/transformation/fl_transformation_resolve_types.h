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
		onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node) override;

	private:
		void
		updateType(fluffy::scope::FindResult_t& findResult, fluffy::ast::TypeDeclNamed* namedType);

		void
		validateResult(fluffy::scope::FindResult_t& findResult, ast::AstNode* const namedType);

		Bool
		canBeType(ast::AstNode* const node);

		ast::AstNode* const
		extractTraitForFromList(NodeList& list);

	private:
		scope::ScopeManager*
		mScopeManager;

	};
} }