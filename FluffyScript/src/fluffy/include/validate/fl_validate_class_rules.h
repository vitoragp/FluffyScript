#pragma once
#include "fl_defs.h"
#include "scope\fl_scope_manager.h"
namespace fluffy { namespace validations {
	/**
	 * ClassRules
	 */

	class ClassRules : public scope::NodeProcessor
	{
	public:
		ClassRules();
		virtual ~ClassRules();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node) override;

	private:
		void
		validateClassDecl(ast::ClassDecl* const classDecl);

		void
		validateClassRequiredInterfaceFunctions(ast::ClassDecl* const classDecl);

		void
		resolveClassBaseAndInterfacesWithGenerics(ast::ClassDecl* const classDecl);

		void
		cleanupClassBaseAndInterfacesWithGenerics(ast::ClassDecl* const classDecl);

	private:
		scope::ScopeManager*
		mScopeManager;

		Bool
		mNeedCleanGenerics;

	};
} }
