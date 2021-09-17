#include <functional>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "utils\fl_polymorphic_utils.h"
#include "attributes\fl_reference.h"
#include "attributes\fl_scope.h"
#include "attributes\fl_included_scope.h"
#include "attributes\fl_implemented_trait_list.h"
#include "validate\fl_validate_trait_rules.h"
namespace fluffy { namespace validations {
	/**
	 * TraitRules
	 */

	TraitRules::TraitRules()
		: mScopeManager(nullptr)
	{}

	TraitRules::~TraitRules()
	{}

	void
	TraitRules::onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node)
	{
		if (mScopeManager == nullptr)
		{
			mScopeManager = scopeManager;
		}

		if (node->nodeType == AstNodeType_e::TraitForDecl && event == scope::NodeProcessorEvent_e::onBegin)
		{
			validateTraitFor(node->to<ast::TraitForDecl>());
		}
	}

	void
	TraitRules::validateTraitFor(ast::TraitForDecl* const traitFor)
	{
		auto typeDef = traitFor->typeDefinitionDecl.get();

		// Como trait for e uma definicao de um trait, esta funcao valida se o trait esta devidamente
		// declarado no escopo.
		validateTraitForDeclaration(traitFor);

		// Inclui a declaracao do trait no tipo especificado no for.
		if (typeDef->nodeType == AstNodeType_e::NamedType)
		{
			auto referenceDefType = traitFor->typeDefinitionDecl->getAttribute<attributes::Reference>();
			auto implementedTraitList = referenceDefType->get()->getOrCreateAttribute<attributes::ImplementedTraitList>();
			auto referenceTrait = traitFor->getAttribute<attributes::Reference>();

			implementedTraitList->insertTrait(
				referenceTrait->get()->identifier,
				referenceTrait->to<ast::TraitDecl>()
			);
		}
		else if (typeDef->nodeType == AstNodeType_e::PrimitiveType)
		{
			auto primitiveType = typeDef->to<ast::TypeDeclPrimitive>()->primitiveTypeRef;
			auto implementedTraitList = primitiveType->getOrCreateAttribute<attributes::ImplementedTraitList>();
			auto referenceTrait = traitFor->getAttribute<attributes::Reference>();

			implementedTraitList->insertTrait(
				referenceTrait->get()->identifier,
				referenceTrait->to<ast::TraitDecl>()
			);
		}

		// Valida se o trait for implementa todas as funcoes do trait.
		validateTraitForRequiredFunctions(traitFor);
	}

	void
	TraitRules::validateTraitForDeclaration(ast::TraitForDecl* const traitFor)
	{
		if (auto reference = traitFor->getAttribute<attributes::Reference>())
		{
			if (reference->get()->nodeType != AstNodeType_e::TraitDecl)
			{
				throw exceptions::custom_exception(
					"The '%s' trait has an invalid reference for trait",
					traitFor->identifier.str()
				);
			}
			else if (reference->get()->identifier != traitFor->identifier)
			{
				throw exceptions::custom_exception(
					"The '%s' trait has an invalid reference for trait",
					traitFor->identifier.str()
				);
			}

			auto implementedTraitForList = reference->get()->getOrCreateAttribute<attributes::ImplementedTraitForList>();

			if (!implementedTraitForList->findTraitFor(traitFor))
			{
				implementedTraitForList->insertTraitFor(traitFor);
			}
		}
		else
		{
			auto findResult = mScopeManager->findNodeById(traitFor->identifier, AstNodeType_e::TraitForDecl, false);

			if (!findResult.foundResult)
			{
				throw exceptions::custom_exception(
					"%s error: Trait '%s' not found in scope",
					traitFor->line, traitFor->column,
					mScopeManager->getCodeUnitName().str(),
					traitFor->identifier.str()
				);
			}

			// Verifica se ha ambiguidade no resultado.
			if (findResult.nodeList.size() > 1)
			{
				throw exceptions::custom_exception(
					"%s error: Failed to resolve trait, invalid search result",
					traitFor->line, traitFor->column,
					mScopeManager->getCodeUnitName().str()
				);
			}

			// Verifica se ha ambiguidade no resultado.
			if (findResult.nodeList[0]->nodeType != AstNodeType_e::TraitDecl)
			{
				throw exceptions::custom_exception(
					"%s error: Failed to implement '%s' trait, '%s' trait doesn't exists",
					traitFor->line, traitFor->column,
					mScopeManager->getCodeUnitName().str(),
					traitFor->identifier.str(), traitFor->identifier.str()
				);
			}

			// Insere a referencia de trait no elemento.
			// Verifica se o no e de alguma include.
			ast::TraitDecl* referencedTrait = nullptr;
			if (findResult.scope->nodeType == AstNodeType_e::CodeUnit)
			{
				auto includeScope = findResult.scope->getAttribute<attributes::IncludedScope>();
				traitFor->insertAttribute(new attributes::Reference(
					includeScope->findScopeFromItem(findResult.nodeList[0]),
					findResult.nodeList[0]
				));

				referencedTrait = findResult.nodeList[0]->to<ast::TraitDecl>();
			}
			else
			{
				traitFor->insertAttribute(new attributes::Reference(findResult.scope, findResult.nodeList[0]));
				referencedTrait = findResult.nodeList[0]->to<ast::TraitDecl>();
			}

			// Insere a referencia ao trait for -> trait.
			if (referencedTrait != nullptr)
			{
				auto implementedTraitForList = referencedTrait->getOrCreateAttribute<attributes::ImplementedTraitForList>();

				if (!implementedTraitForList->findTraitFor(traitFor))
				{
					implementedTraitForList->insertTraitFor(traitFor);
				}
			}
		}
	}

	void
	TraitRules::validateTraitForRequiredFunctions(ast::TraitForDecl* const traitFor)
	{
		auto reference = traitFor->getAttribute<attributes::Reference>();
		auto trait = ast::safe_cast<ast::TraitDecl>(reference->get());
		auto functionList = NodeList();

		ast::for_each(trait->functionDeclList, [&functionList](ast::TraitFunctionDecl* const traitFunction) {
			functionList.push_back(traitFunction);
		});

		ast::for_each(traitFor->functionDeclList, [&functionList](ast::TraitFunctionDecl* const traitFunction) {
			for (auto it = functionList.begin(); it != functionList.end();)
			{
				if (utils::AstUtils::same(traitFunction, *it))
				{
					it = functionList.erase(it);
				}
				else
				{
					it++;
				}
			}
		});

		if (functionList.size())
		{
			throw exceptions::custom_exception(
				"%s error: Trait definition '%s' trait, must implement all functions: '%s' was not implemented",
				functionList[0]->line, functionList[0]->column,
				mScopeManager->getCodeUnitName().str(),
				traitFor->identifier.str(), functionList[0]->identifier.str()
			);
		}
	}
} }
