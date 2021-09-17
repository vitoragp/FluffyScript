#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "attributes\fl_reference.h"
#include "attributes\fl_included_scope.h"
#include "transformation\fl_transformation_resolve_types.h"
namespace fluffy { namespace transformations {
	/**
	 * ResolveTypes
	 */

	ResolveTypes::ResolveTypes()
		: mScopeManager(nullptr)
	{}

	ResolveTypes::~ResolveTypes()
	{}

	void
	ResolveTypes::onProcess(scope::ScopeManager* const scopeManager, scope::NodeProcessorEvent_e event, ast::AstNode* const node)
	{
		scope::FindResult_t findResult = { nullptr, NodeList(), false };

		// Salva o estado do scopeManager.
		if (mScopeManager == nullptr) {
			mScopeManager = scopeManager;
		}

		if (node->nodeType == AstNodeType_e::NamedType && event == scope::NodeProcessorEvent_e::onBegin)
		{
			auto namedType = ast::safe_cast<ast::TypeDeclNamed>(node);

			// Verifica se e necessario resolver o caminho do no.
			if (namedType->scopePath != nullptr)
			{
				auto nextId = namedType->scopePath.get();
				
				while (nextId)
				{
					if (!findResult.foundResult)
					{
						findResult = mScopeManager->findNodeById(nextId->identifier, namedType->startFromRoot);
					}
					else
					{
						auto scope = scope::Scope(mScopeManager, findResult.scope, findResult.nodeList[0]);
						validateScope(scope.getNode());

						findResult = scope.findNodeById(nextId->identifier);
					}

					if (!findResult.foundResult)
					{
						throw exceptions::custom_exception(
							"%s error: Identifier '%s' not found in scope",
							namedType->line, namedType->column,
							mScopeManager->getCodeUnitName().str(),
							namedType->identifier.str()
						);
					}

					if (findResult.nodeList.size() > 1)
					{
						throw exceptions::custom_exception(
							"%s error: Ambiguous search result",
							namedType->line, namedType->column,
							mScopeManager->getCodeUnitName().str()
						);
					}

					if (nextId->scopedChildPath)
					{
						nextId = nextId->scopedChildPath.get();
					}
					else
					{
						break;
					}
				}

				auto scope = scope::Scope(mScopeManager, findResult.scope, findResult.nodeList[0]);
				validateScope(scope.getNode());

				findResult = scope.findNodeById(namedType->identifier);
			}
			else
			{
				findResult = mScopeManager->findNodeById(namedType->identifier, namedType->startFromRoot);
			}

			if (!findResult.foundResult)
			{
				throw exceptions::custom_exception(
					"%s error: Identifier '%s' not found in scope",
					namedType->line, namedType->column,
					mScopeManager->getCodeUnitName().str(),
					namedType->identifier.str()
				);
			}

			// Se houver ambiguidade, remover os elementos fracos: aqueles elementos
			// que vem de includes com coringa.
			validateResult(findResult, namedType);

			// Inclui o atributo de referencia no no de tipo.
			if (findResult.scope->nodeType == AstNodeType_e::CodeUnit)
			{
				auto includeScope = findResult.scope->getAttribute<attributes::IncludedScope>();
				namedType->insertAttribute(new attributes::Reference(
					includeScope->findScopeFromItem(findResult.nodeList[0]),
					findResult.nodeList[0]
				));
			}
			else
			{
				namedType->insertAttribute(new attributes::Reference(findResult.scope, findResult.nodeList[0]));
			}
		}
	}

	void
	ResolveTypes::validateResult(fluffy::scope::FindResult_t& findResult, ast::AstNode* const namedType)
	{
		if (findResult.nodeList.size())
		{
			// Valida o escopo, removendo elementos que nao podem ser usados como tipos.
			for (auto it = findResult.nodeList.begin(); it != findResult.nodeList.end();)
			{
				if (!canBeType(*it))
				{
					it = findResult.nodeList.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		if (findResult.nodeList.size() > 1)
		{
			throw exceptions::custom_exception(
				"%s error: Ambiguous search result '%s'",
				namedType->line, namedType->column,
				mScopeManager->getCodeUnitName().str(),
				namedType->identifier.str()
			);
		}
		else if (!findResult.nodeList.size())
		{
			throw exceptions::custom_exception(
				"%s error: The '%s' could not be a valid type or could not be found in scope",
				namedType->line, namedType->column,
				mScopeManager->getCodeUnitName().str(),
				namedType->identifier.str()
			);
		}
	}

	Bool
	ResolveTypes::canBeType(fluffy::ast::AstNode* const node)
	{
		switch (node->nodeType)
		{
		case AstNodeType_e::ClassDecl:
		case AstNodeType_e::StructDecl:
		case AstNodeType_e::InterfaceDecl:
		case AstNodeType_e::EnumDecl:
		case AstNodeType_e::TraitDecl:
		case AstNodeType_e::GenericItemDecl:
			return true;

		default:
			break;
		}
		return false;
	}

	void
	ResolveTypes::validateScope(ast::AstNode* const node)
	{
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
		case AstNodeType_e::NamespaceDecl:
		case AstNodeType_e::ClassDecl:
		case AstNodeType_e::EnumDecl:
			break;
		default:
			throw exceptions::custom_exception(
				"%s error: '%s' is not a valid scope",
				node->line, node->column,
				mScopeManager->getCodeUnitName().str(),
				node->identifier.str()
			);
		}
	}
} }