#include <functional>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "utils\fl_polymorphic_utils.h"
#include "attributes\fl_reference.h"
#include "attributes\fl_scope.h"
#include "attributes\fl_included_scope.h"
#include "attributes\fl_implemented_trait_list.h"
#include "validate\fl_validate_generic_rules.h"
namespace fluffy { namespace validations {
	/**
	 * GenericRules
	 */

	GenericRules::GenericRules()
		: mScopeManager(nullptr)
	{}

	GenericRules::~GenericRules()
	{}

	void
	GenericRules::onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node)
	{
		if (mScopeManager == nullptr)
		{
			mScopeManager = scopeManager;
		}

		if (node->nodeType == AstNodeType_e::NamedType && event == scope::NodeProcessorEvent_e::onBegin)
		{
			auto type = node->to<ast::TypeDeclNamed>();
			auto reference = type->getAttribute<attributes::Reference>();

			ast::GenericDecl* genericDecl = nullptr;
			switch (reference->get()->nodeType)
			{
			case AstNodeType_e::ClassDecl:
				validateArgCountAndWhereClause(reference->to<ast::ClassDecl>()->genericDecl.get(), type, reference);
				break;

			case AstNodeType_e::InterfaceDecl:
				validateArgCountAndWhereClause(reference->to<ast::InterfaceDecl>()->genericDecl.get(), type, reference);
				break;

			case AstNodeType_e::StructDecl:
				validateArgCountAndWhereClause(reference->to<ast::StructDecl>()->genericDecl.get(), type, reference);
				break;

			case AstNodeType_e::TraitDecl:
				validateArgCountAndWhereClause(genericDecl = reference->to<ast::TraitDecl>()->genericDecl.get(), type, reference);
				break;

			case AstNodeType_e::EnumDecl:
				validateArgCountAndWhereClause(genericDecl = reference->to<ast::EnumDecl>()->genericDecl.get(), type, reference);
				break;

			default:
				break;
			}
		}
	}

	void
	GenericRules::validateArgCountAndWhereClause(ast::GenericDecl* const genericDecl, ast::TypeDeclNamed* const type, attributes::Reference* const reference)
	{
		if (genericDecl != nullptr)
		{
			// Valida o numero de argumentos.
			if (type->genericDefinitionList.size() != genericDecl->genericDeclItemList.size())
			{
				throw exceptions::custom_exception(
					"%s error: Generic '%s' requires %d type arguments",
					type->line, type->column,
					mScopeManager->getCodeUnitName().str(),
					reference->get()->identifier.str(),
					genericDecl->genericDeclItemList.size()
				);
			}

			auto typeGenericIt = type->genericDefinitionList.begin();
			auto referenceGenericIt = genericDecl->genericDeclItemList.begin();

			// Verifica se os argumentos atendem os requisitos das clausuras where.
			while (typeGenericIt != type->genericDefinitionList.end())
			{
				auto typeGeneric = typeGenericIt->get();
				auto referenceGeneric = referenceGenericIt->get();

				if (referenceGeneric->whereTypeList.size())
				{
					Bool acceptedType = false;
					for (auto& whereIt : referenceGeneric->whereTypeList)
					{
						if (utils::AstUtils::equals(whereIt.get(), typeGeneric))
						{
							acceptedType = true;
							break;
						}
						else if (whereIt->nodeType == AstNodeType_e::NamedType && typeGeneric->nodeType == AstNodeType_e::NamedType)
						{
							auto referenceA = typeGeneric->getAttribute<attributes::Reference>();
							auto referenceB = whereIt->getAttribute<attributes::Reference>();

							if (utils::PolymorphicUtils::canBe(
								referenceA->get(),
								referenceB->get()))
							{
								acceptedType = true;
								break;
							}
						}
					}

					if (!acceptedType)
					{
						throw exceptions::custom_exception(
							"%s error: Type generic item doesn't match where clause",
							type->line, type->column,
							mScopeManager->getCodeUnitName().str()
						);
					}
				}
				typeGenericIt++;
				referenceGenericIt++;
			}
		}
	}
} }
