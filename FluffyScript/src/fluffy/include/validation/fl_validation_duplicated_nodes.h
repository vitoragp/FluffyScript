#pragma once
#include "fl_defs.h"
#include "scope\fl_scope_manager.h"
namespace fluffy { namespace validations {
	/**
	 * ValidationDuplicatedNodes
	 */

	class ValidationDuplicatedNodes : public scope::NodeProcessor
	{
	public:
		ValidationDuplicatedNodes();
		virtual ~ValidationDuplicatedNodes();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node) override;

	private:
		NodeMultiMap
		mIncludedScope;
	};
} }
