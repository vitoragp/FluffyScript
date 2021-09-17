#pragma once
#include "fl_defs.h"
#include "scope\fl_scope_manager.h"

namespace fluffy { namespace attributes {
	class Reference;
} }

namespace fluffy { namespace validations {
	/**
	 * GenericRules
	 */

	class GenericRules : public scope::NodeProcessor
	{
	public:
		GenericRules();
		virtual ~GenericRules();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node) override;

	private:
		void
		validateArgCountAndWhereClause(ast::GenericDecl* const genericDecl, ast::TypeDeclNamed* const type, attributes::Reference* const reference);

	private:
		scope::ScopeManager*
		mScopeManager;

	};
} }
