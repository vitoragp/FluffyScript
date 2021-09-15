#pragma once
#include "fl_defs.h"
#include "scope\fl_scope_manager.h"
namespace fluffy { namespace validations {
	/**
	 * DuplicatedNodes
	 */

	class DuplicatedNodes : public scope::NodeProcessor
	{
	public:
		DuplicatedNodes();
		virtual ~DuplicatedNodes();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node) override;

	private:
		void
		validateDuplication(scope::ScopeManager* const scopeManager, ast::AstNode* const node);

	private:
		NodeMultiMap
		mIncludedScope;
	};
} }
