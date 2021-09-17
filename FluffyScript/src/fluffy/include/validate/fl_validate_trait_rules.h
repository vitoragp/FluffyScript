#pragma once
#include "fl_defs.h"
#include "scope\fl_scope_manager.h"
namespace fluffy { namespace validations {
	/**
	 * TraitRules
	 */

	class TraitRules : public scope::NodeProcessor
	{
	public:
		TraitRules();
		virtual ~TraitRules();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node) override;

	private:
		void
		validateTraitFor(ast::TraitForDecl* const traitFor);

		void
		validateTraitForDeclaration(ast::TraitForDecl* const traitFor);

		void
		validateTraitForRequiredFunctions(ast::TraitForDecl* const traitFor);

	private:
		scope::ScopeManager*
		mScopeManager;

	};
} }
