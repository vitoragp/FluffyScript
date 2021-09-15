#include <functional>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "utils\fl_polymorphic_utils.h"
#include "attributes\fl_reference.h"
#include "attributes\fl_included_scope.h"
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
			validateClassDecl(node->to<ast::ClassDecl>());
			break;
		
		case AstNodeType_e::TraitForDecl:
			validateTraitFor(node->to<ast::TraitForDecl>());
			break;

		case AstNodeType_e::NamedType:
			validateNamedTypeGeneric(node->to<ast::TypeDeclNamed>());
			break;
					
		default:
			break;
		}
	}

	void
	GeneralRules::validateClassDecl(ast::ClassDecl* const classDecl)
	{
		// Valida classe pai.
		if (classDecl->baseClass)
		{
			if (auto reference = classDecl->baseClass->getAttribute<attributes::Reference>())
			{
				if (reference->referencedNode->nodeType == AstNodeType_e::ClassDecl)
				{
					auto baseClassRef = reference->referencedNode->to<ast::ClassDecl>();

					// Valida se ha heranca ciclica.
					ast::ClassDecl* parentClass = baseClassRef;
					while (baseClassRef != nullptr)
					{
						if (baseClassRef->nodeType != AstNodeType_e::ClassDecl)
						{
							throw exceptions::custom_exception(
								"%s error: The '%s' class must be extended by another class: '%s' is not a class",
								parentClass->line, parentClass->column,
								mScopeManager->getCodeUnitName().str(), parentClass->identifier.str(),
								baseClassRef->identifier.str()
							);
						}

						if (baseClassRef == classDecl)
						{
							throw exceptions::custom_exception(
								"%s error: The '%s' class has circular extends dependency",
								classDecl->line, classDecl->column,
								mScopeManager->getCodeUnitName().str(),
								classDecl->identifier.str()
							);
						}

						if (baseClassRef->baseClass != nullptr)
						{
							parentClass = baseClassRef;
							auto baseClassRefAttrib = baseClassRef->baseClass->getAttribute<attributes::Reference>();
							baseClassRef = baseClassRefAttrib->referencedNode->to<ast::ClassDecl>();
						}
						else
						{
							baseClassRef = nullptr;
						}
					}
				}
				else
				{
					throw exceptions::custom_exception(
						"%s error: The '%s' class must be extended by another class: '%s' is not a class",
						classDecl->baseClass->line, classDecl->baseClass->column,
						mScopeManager->getCodeUnitName().str(), classDecl->identifier.str(),
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

		// Valida lista de interfaces.
		for (auto& implement : classDecl->interfaceList)
		{
			if (auto reference = implement->getAttribute<attributes::Reference>())
			{
				if (reference->referencedNode->nodeType != AstNodeType_e::InterfaceDecl)
				{
					throw exceptions::custom_exception(
						"%s error: The '%s' class must implement only interfaces: '%s' is not a interface",
						implement->line, implement->column,
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

		if (!classDecl->isAbstract)
		{
			// Valida se uma classe nao abstrata possui funcoes abstratas.
			for (auto& classFunc : classDecl->functionList)
			{
				if (classFunc->isAbstract)
				{
					throw exceptions::custom_exception(
						"%s error: The class '%s' declare '%s' function as abstract, only abstract class can have abstract functions",
						classFunc->line, classFunc->column,
						mScopeManager->getCodeUnitName().str(),
						classDecl->identifier.str(),
						classFunc->identifier.str()
					);
				}
			}
		}

		// Valida se uma classe nao abstrata implementa todas as funcoes de suas interfaces.
		validateClassRequiredInterfaceFunctions(classDecl);
	}

	void
	GeneralRules::validateTraitFor(ast::TraitForDecl* const traitFor)
	{
		auto typeDef = traitFor->typeDefinitionDecl.get();

		// Como trait for e uma definicao de um trait, esta funcao valida se o trait esta devidamente
		// declarado no escopo.
		validateTraitForDeclaration(traitFor);

		// Inclui a declaracao do trait no tipo especificado no for.
		if (typeDef->nodeType == AstNodeType_e::NamedType)
		{
			auto referenceDefType = traitFor->typeDefinitionDecl->getAttribute<attributes::Reference>();
			auto implementedTraitList = referenceDefType->referencedNode->getOrCreateAttribute<attributes::ImplementedTraitList>();
			auto referenceTrait = traitFor->getAttribute<attributes::Reference>();

			implementedTraitList->insertTrait(
				referenceTrait->referencedNode->identifier,
				referenceTrait->referencedNode->to<ast::TraitDecl>()
			);
		}
		else if (typeDef->nodeType == AstNodeType_e::PrimitiveType)
		{
			auto primitiveType = typeDef->to<ast::TypeDeclPrimitive>()->primitiveTypeRef;
			auto implementedTraitList = primitiveType->getOrCreateAttribute<attributes::ImplementedTraitList>();
			auto referenceTrait = traitFor->getAttribute<attributes::Reference>();

			implementedTraitList->insertTrait(
				referenceTrait->referencedNode->identifier,
				referenceTrait->referencedNode->to<ast::TraitDecl>()
			);
		}

		// Valida se o trait for implementa todas as funcoes do trait.
		validateTraitForRequiredFunctions(traitFor);
	}

	void
	GeneralRules::validateTraitForDeclaration(ast::TraitForDecl* const traitFor)
	{
		if (auto reference = traitFor->getAttribute<attributes::Reference>())
		{
			if (reference->referencedNode->nodeType != AstNodeType_e::TraitDecl)
			{
				throw exceptions::custom_exception(
					"The '%s' trait has an invalid reference for trait",
					traitFor->identifier.str()
				);
			}
			else if (reference->referencedNode->identifier != traitFor->identifier)
			{
				throw exceptions::custom_exception(
					"The '%s' trait has an invalid reference for trait",
					traitFor->identifier.str()
				);
			}

			auto implementedTraitForList = reference->referencedNode->getOrCreateAttribute<attributes::ImplementedTraitForList>();

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
					"%s error: Failed to implement '%s' trait, '%s' trait doesn't exists",
					traitFor->line, traitFor->column,
					mScopeManager->getCodeUnitName().str(),
					traitFor->identifier.str(), traitFor->identifier.str()
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
	GeneralRules::validateTraitForRequiredFunctions(ast::TraitForDecl* const traitFor)
	{
		auto reference = traitFor->getAttribute<attributes::Reference>();
		auto trait = ast::safe_cast<ast::TraitDecl>(reference->referencedNode);
		auto functionList = NodeList();

		ast::for_each(trait->functionDeclList, [&functionList](ast::TraitFunctionDecl* const traitFunction) {
			functionList.push_back(traitFunction);
		});

		ast::for_each(traitFor->functionDeclList, [&functionList](ast::TraitFunctionDecl* const traitFunction) {
			for (auto it = functionList.begin(); it != functionList.end();)
			{
				if (utils::AstUtils::equals(traitFunction, *it))
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

	void
	GeneralRules::validateNamedTypeGeneric(ast::TypeDeclNamed* const type)
	{
		auto reference = type->getAttribute<attributes::Reference>();

		ast::GenericDecl* genericDecl = nullptr;
		switch (reference->referencedNode->nodeType)
		{
		case AstNodeType_e::ClassDecl:
			genericDecl = reference->referencedNode->to<ast::ClassDecl>()->genericDecl.get();
			break;

		case AstNodeType_e::InterfaceDecl:
			genericDecl = reference->referencedNode->to<ast::InterfaceDecl>()->genericDecl.get();
			break;

		case AstNodeType_e::StructDecl:
			genericDecl = reference->referencedNode->to<ast::StructDecl>()->genericDecl.get();
			break;

		case AstNodeType_e::TraitDecl:
			genericDecl = reference->referencedNode->to<ast::TraitDecl>()->genericDecl.get();
			break;

		case AstNodeType_e::EnumDecl:
			genericDecl = reference->referencedNode->to<ast::EnumDecl>()->genericDecl.get();
			break;

		default:
			break;
		}

		if (genericDecl != nullptr)
		{
			// Valida o numero de argumentos.
			if (type->genericDefinitionList.size() != genericDecl->genericDeclItemList.size())
			{
				throw exceptions::custom_exception(
					"%s error: Generic '%s' requires %d type arguments",
					type->line, type->column,
					mScopeManager->getCodeUnitName().str(),
					reference->referencedNode->identifier.str(),
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
								referenceA->referencedNode,
								referenceB->referencedNode))
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

	void
	GeneralRules::validateClassRequiredInterfaceFunctions(ast::ClassDecl* const classDecl)
	{
		// Valida se a classe implementa todas as funcoes das interfaces.
		for (auto& classInterfaceDecl : classDecl->interfaceList)
		{
			auto interfaceRef = classInterfaceDecl->getAttribute<attributes::Reference>();
			auto interfaceDecl = interfaceRef->referencedNode->to<ast::InterfaceDecl>();

			for (auto& interfaceFunction : interfaceDecl->functionDeclList)
			{
				for (auto& functionDecl : classDecl->functionList)
				{
					// Valida o modificador de acesso de funcoes abstratas.
					if (functionDecl->isAbstract && functionDecl->accessModifier != ClassMemberAccessModifier_e::Public)
					{
						throw exceptions::custom_exception(
							"%s error: The '%s' function, must be public",
							functionDecl->line, functionDecl->column,
							mScopeManager->getCodeUnitName().str(),
							functionDecl->identifier.str()
						);
					}

					if (utils::AstUtils::equals(interfaceFunction.get(), functionDecl.get()))
					{
						// Valida se a funcao e publica.
						if (functionDecl->accessModifier != ClassMemberAccessModifier_e::Public)
						{
							throw exceptions::custom_exception(
								"%s error: The '%s' function implemented for '%s' interface, must be public",
								functionDecl->line, functionDecl->column,
								mScopeManager->getCodeUnitName().str(),
								interfaceFunction->identifier.str(),
								classInterfaceDecl->identifier.str()
							);
						}
						break;
					}
				}
			}
		}

		NodeList functionsOfInterfaces;
		std::function<void(ast::ClassDecl* const)> extractInterfaceFunctions;

		extractInterfaceFunctions = [this, &functionsOfInterfaces, &extractInterfaceFunctions](ast::ClassDecl* const classDecl) {
			// Percorre todas as herancas possivel.
			if (classDecl->baseClass)
			{
				auto baseClassRefAttrib = classDecl->baseClass->getAttribute<attributes::Reference>();
				extractInterfaceFunctions(baseClassRefAttrib->referencedNode->to<ast::ClassDecl>());
			}

			// Extrai as funcoes das interfaces.
			for (auto& interfaceDeclType : classDecl->interfaceList)
			{
				auto interfaceDeclRef = interfaceDeclType->getAttribute<attributes::Reference>();
				auto interfaceDecl = interfaceDeclRef->referencedNode->to<ast::InterfaceDecl>();

				for (auto& interfaceFunc : interfaceDecl->functionDeclList)
				{
					functionsOfInterfaces.emplace_back(interfaceFunc.get());
				}
			}

			// Incluir na lista funcoes abstratas.
			for (auto& functionDecl : classDecl->functionList)
			{
				if (functionDecl->isAbstract)
				{
					functionsOfInterfaces.emplace_back(functionDecl.get());
				}
			}

			// Remove funcoes ja implemetadas.
			for (auto interfaceFunctionIt = functionsOfInterfaces.begin();
				interfaceFunctionIt != functionsOfInterfaces.end();
			)
			{
				Bool needUpdate = true;
				auto interfaceFunction = *interfaceFunctionIt;
				for (auto& functionDecl : classDecl->functionList)
				{
					if (utils::AstUtils::equals(interfaceFunction, functionDecl.get()))
					{
						// Valida se a funcao e publica.
						if (interfaceFunction->nodeType == AstNodeType_e::InterfaceFunctionDecl &&
							functionDecl->accessModifier != ClassMemberAccessModifier_e::Public
						)
						{
							throw exceptions::custom_exception(
								"%s error: The '%s' function implemented from interface, must be public",
								classDecl->line, classDecl->column,
								mScopeManager->getCodeUnitName().str(),
								interfaceFunction->identifier.str(),
								classDecl->identifier.str()
							);
						}

						interfaceFunctionIt = functionsOfInterfaces.erase(
							interfaceFunctionIt
						);
						needUpdate = false;
						break;
					}
				}

				// Verifica se e necessario atualizar o iterator,
				// se alguem elemento e deletado nao e necessario atualizar.
				if (needUpdate)
				{
					interfaceFunctionIt++;
				}
			}
		};

		extractInterfaceFunctions(classDecl);

		// Remove funcoes ja implemetadas.
		for (auto& functionDecl : classDecl->functionList)
		{
			for (auto interfaceFunctionIt = functionsOfInterfaces.begin();
				interfaceFunctionIt != functionsOfInterfaces.end();
			)
			{
				if (utils::AstUtils::equals(*interfaceFunctionIt, functionDecl.get()))
				{
					interfaceFunctionIt = functionsOfInterfaces.erase(
						interfaceFunctionIt
					);
				}
				else
				{
					interfaceFunctionIt++;
				}
			}
		}

		// TODO: Para deixar a mensagem de erro mais clara informar o escopo da funcao.
		// Verifica se ha alguma funcao pendente.
		if (functionsOfInterfaces.size() && !classDecl->isAbstract)
		{
			for (auto requiredFunction : functionsOfInterfaces)
			{
				throw exceptions::custom_exception(
					"%s error: The '%s' class must implement '%s', it may have been declared in an interface or it may be abstract",
					classDecl->line, classDecl->column,
					mScopeManager->getCodeUnitName().str(),
					classDecl->identifier.str(),
					requiredFunction->identifier.str()
				);
			}
		}
	}
} }
