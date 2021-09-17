#include <functional>
#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "utils\fl_polymorphic_utils.h"
#include "attributes\fl_reference.h"
#include "attributes\fl_scope.h"
#include "attributes\fl_included_scope.h"
#include "attributes\fl_implemented_trait_list.h"
#include "validate\fl_validate_class_rules.h"
#include "generics\fl_clone.h"
namespace fluffy { namespace validations {
	/**
	 * ClassRules
	 */

	ClassRules::ClassRules()
		: mScopeManager(nullptr)
		, mNeedCleanGenerics(false)
	{}

	ClassRules::~ClassRules()
	{}

	void
	ClassRules::onProcess(scope::ScopeManager* const scopeManager, const scope::NodeProcessorEvent_e event, ast::AstNode* const node)
	{
		if (mScopeManager == nullptr)
		{
			mScopeManager = scopeManager;
		}

		if (node->nodeType == AstNodeType_e::ClassDecl && event == scope::NodeProcessorEvent_e::onBegin)
		{
			validateClassDecl(node->to<ast::ClassDecl>());
		}
	}

	void
	ClassRules::validateClassDecl(ast::ClassDecl* const classDecl)
	{
		// Valida classe pai.
		if (classDecl->baseClass)
		{
			if (auto reference = classDecl->baseClass->getAttribute<attributes::Reference>())
			{
				if (reference->get()->nodeType == AstNodeType_e::ClassDecl)
				{
					auto baseClassRef = reference->to<ast::ClassDecl>();

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
							baseClassRef = baseClassRefAttrib->to<ast::ClassDecl>();
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
						reference->get()->identifier.str()
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
				if (reference->get()->nodeType != AstNodeType_e::InterfaceDecl)
				{
					throw exceptions::custom_exception(
						"%s error: The '%s' class must implement only interfaces: '%s' is not a interface",
						implement->line, implement->column,
						mScopeManager->getCodeUnitName().str(),
						classDecl->identifier.str(),
						reference->get()->identifier.str()
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
	ClassRules::validateClassRequiredInterfaceFunctions(ast::ClassDecl* const classDecl)
	{
		// Valida se a classe implementa todas as funcoes das interfaces.
		for (auto& classInterfaceDecl : classDecl->interfaceList)
		{
			auto interfaceRef = classInterfaceDecl->getAttribute<attributes::Reference>();
			auto interfaceDecl = interfaceRef->to<ast::InterfaceDecl>();

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
								"%s error: The '%s' function in '%s' class must be public to satisfy '%s' interface",
								functionDecl->line, functionDecl->column,
								mScopeManager->getCodeUnitName().str(),
								interfaceFunction->identifier.str(),
								classDecl->identifier.str(),
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
				auto baseClassRef = baseClassRefAttrib->to<ast::ClassDecl>();
				extractInterfaceFunctions(baseClassRef);
			}

			// Extrai as funcoes das interfaces.
			for (auto& classInterfaceDecl : classDecl->interfaceList)
			{
				auto interfaceDeclRef = classInterfaceDecl->getAttribute<attributes::Reference>();
				auto interfaceDecl = interfaceDeclRef->to<ast::InterfaceDecl>();

				for (auto& interfaceFunc : interfaceDecl->functionDeclList)
				{
					functionsOfInterfaces.emplace_back(interfaceFunc.get());
					interfaceFunc->insertAttribute(new attributes::Scope(interfaceDecl));
				}
			}

			// Incluir na lista funcoes abstratas.
			for (auto& functionDecl : classDecl->functionList)
			{
				if (functionDecl->isAbstract)
				{
					functionsOfInterfaces.emplace_back(functionDecl.get());
					functionDecl->insertAttribute(new attributes::Scope(classDecl));
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
					if (utils::AstUtils::equals(interfaceFunction, functionDecl.get()) && interfaceFunction != functionDecl.get())
					{
						auto scope = interfaceFunction->getAttribute<attributes::Scope>();

						// Valida se a funcao e publica.
						if (interfaceFunction->nodeType == AstNodeType_e::InterfaceFunctionDecl &&
							functionDecl->accessModifier != ClassMemberAccessModifier_e::Public
						)
						{
							throw exceptions::custom_exception(
								"%s error: The '%s' function in '%s' class must be public to satisfy '%s' interface",
								classDecl->line, classDecl->column,
								mScopeManager->getCodeUnitName().str(),
								interfaceFunction->identifier.str(),
								classDecl->identifier.str(),
								scope->get()->identifier.str()
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
				auto scope = requiredFunction->getAttribute<attributes::Scope>();

				if (requiredFunction->nodeType == AstNodeType_e::ClassFunctionDecl)
				{
					throw exceptions::custom_exception(
						"%s error: The '%s' class must implement '%s' abstract function from '%s' class",
						classDecl->line, classDecl->column,
						mScopeManager->getCodeUnitName().str(),
						classDecl->identifier.str(),
						requiredFunction->identifier.str(),
						scope->get()->identifier.str()
					);
				}
				else
				{
					throw exceptions::custom_exception(
						"%s error: The '%s' class must implement '%s' function from '%s' interface",
						classDecl->line, classDecl->column,
						mScopeManager->getCodeUnitName().str(),
						classDecl->identifier.str(),
						requiredFunction->identifier.str(),
						scope->get()->identifier.str()
					);
				}
			}
		}
	}
} }
