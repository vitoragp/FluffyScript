#pragma once
#include "fl_defs.h"
#include "scope\fl_scope_manager.h"
namespace fluffy { namespace validations {
	/**
	 * 
	 */

	class GeneralRules : public scope::NodeProcessor
	{
	public:
		GeneralRules();
		virtual ~GeneralRules();

		virtual void
		onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node) override;

	private:
		void
		validateClassDecl(ast::ClassDecl* const classDecl);

		void
		validateTraitFor(ast::TraitForDecl* const traitFor);

		void
		validateTraitForDeclaration(ast::TraitForDecl* const traitFor);

		void
		validateTraitForRequiredFunctions(ast::TraitForDecl* const traitFor);

		void
		validateNamedTypeGeneric(ast::TypeDeclNamed* const type);

		void
		validateClassRequiredInterfaceFunctions(ast::ClassDecl* const classDecl);

	private:
		scope::ScopeManager*
		mScopeManager;

	};
} }
