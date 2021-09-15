#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "attributes\fl_reference.h"
#include "attributes\fl_implemented_trait_list.h"
#include "validation\fl_validation_general_rules.h"
namespace fluffy { namespace validations {
	/**
	 * GeneralRules
	 */

	GeneralRules::GeneralRules()
		: mScopeManager(nullptr)
	{}

	GeneralRules::~GeneralRules()
	{}

	void
	GeneralRules::onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
	{
		if (mScopeManager == nullptr)
		{
			mScopeManager = scopeManager;
		}

		switch (node->nodeType)
		{
		case AstNodeType_e::ClassDecl:
			validateClassDecl(ast::safe_cast<ast::ClassDecl>(node));
			break;
		
		case AstNodeType_e::TraitForDecl:
			validateTraitFor(ast::safe_cast<ast::TraitForDecl>(node));
			break;
		
		default:
			break;
		}
	}

	void
	GeneralRules::validateClassDecl(ast::ClassDecl* const classDecl)
	{
		if (classDecl->baseClass)
		{
			if (auto reference = classDecl->baseClass->getAttribute<attributes::Reference>())
			{
				if (reference->referencedNode->nodeType != AstNodeType_e::ClassDecl)
				{
					throw exceptions::custom_exception(
						"%s error: The '%s' class must be extended by another class",
						classDecl->line, classDecl->column,
						mScopeManager->getCodeUnitName().str(),
						classDecl->identifier.str()
					);
				}
			}
			else
			{
				throw exceptions::custom_exception(
					"%s error: Failed '%s' to retrieve baseClass reference attribute",
					mScopeManager->getCodeUnitName().str(),
					classDecl->identifier.str()
				);
			}
		}

		for (auto& implement : classDecl->interfaceList)
		{
			if (auto reference = implement->getAttribute<attributes::Reference>())
			{
				if (reference->referencedNode->nodeType != AstNodeType_e::InterfaceDecl)
				{
					throw exceptions::custom_exception(
						"%s error: The '%s' class must implement only interfaces: '%s' is not a interface",
						classDecl->line, classDecl->column,
						mScopeManager->getCodeUnitName().str(),
						classDecl->identifier.str(),
						reference->referencedNode->identifier.str()
					);
				}
			}
			else
			{
				throw exceptions::custom_exception(
					"%s error: Failed '%s' to retrieve baseClass reference attribute",
					mScopeManager->getCodeUnitName().str(),
					classDecl->identifier.str()
				);
			}
		}
	}

	void
	GeneralRules::validateTraitFor(ast::TraitForDecl* const traitFor)
	{
		if (traitFor->typeDefinitionDecl->nodeType == AstNodeType_e::NamedType)
		{
			auto reference = traitFor->typeDefinitionDecl->getAttribute<attributes::Reference>();
			auto implementedTraitList = reference->referencedNode->getOrCreateAttribute<attributes::ImplementedTraitList>();

			implementedTraitList->insertTrait(traitFor->identifier, traitFor);
		}
		else
		{
			auto implementedTraitList = traitFor->typeDefinitionDecl->getOrCreateAttribute<attributes::ImplementedTraitList>();
		}
	}
} }
