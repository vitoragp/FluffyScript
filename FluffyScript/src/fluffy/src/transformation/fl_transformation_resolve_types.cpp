#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
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
	ResolveTypes::onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
	{
		scope::FindResult_t findResult = { nullptr, NodeList(), false };

		// Salva o estado do scopeManager.
		mScopeManager = scopeManager;

		if (node->nodeType == AstNodeType_e::NamedType)
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
			if (findResult.nodeList.size())
			{
				validateResult(findResult, namedType);
			}
			updateType(findResult, namedType);
		}
	}

	void
	ResolveTypes::updateType(fluffy::scope::FindResult_t& findResult, fluffy::ast::TypeDeclNamed* namedType)
	{
		if (findResult.nodeList[0]->nodeType == AstNodeType_e::IncludeItemDecl)
		{
			auto includedItem = ast::safe_cast<ast::IncludeItemDecl>(findResult.nodeList[0]);

			namedType->hasBeenResolved = true;
			namedType->referencedScope = includedItem->referencedScope;
			namedType->referencedNode = includedItem->referencedNodeList[0];
		}
		else if (findResult.nodeList[0]->nodeType == AstNodeType_e::WeakIncludeItemDecl)
		{
			auto weakIncludedItem = ast::safe_cast<ast::WeakIncludeItemDecl>(findResult.nodeList[0]);

			namedType->hasBeenResolved = true;
			namedType->referencedScope = weakIncludedItem->referencedScope;
			namedType->referencedNode = weakIncludedItem->referencedNode;
		}
		else
		{
			namedType->hasBeenResolved = true;
			namedType->referencedScope = findResult.scope;
			namedType->referencedNode = findResult.nodeList[0];
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

		// Remove elementos fracos, se necessario.
		if (findResult.nodeList.size() > 1)
		{
			for (auto it = findResult.nodeList.begin(); it != findResult.nodeList.end();)
			{
				auto node = *it;

				if (node->nodeType == AstNodeType_e::WeakIncludeItemDecl)
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
				"%s error: Ambiguous search result",
				namedType->line, namedType->column,
				mScopeManager->getCodeUnitName().str()
			);
		}
		else if (!findResult.nodeList.size())
		{
			throw exceptions::custom_exception(
				"%s error: The '%s' type could not be found in scope",
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

		case AstNodeType_e::IncludeItemDecl:
		{
			auto includeDecl = ast::safe_cast<ast::IncludeItemDecl>(node);

			if (!includeDecl->hasBeenResolved)
			{
				throw exceptions::custom_exception("Unresolved include");
			}

			if (includeDecl->referencedNodeList.size() > 1)
			{
				auto referencedNode = extractTraitForFromList(includeDecl->referencedNodeList);
				if (referencedNode == nullptr)
				{
					throw exceptions::custom_exception("Unresolved include");
				}
				return canBeType(referencedNode);
			}
			return canBeType(includeDecl->referencedNodeList[0]);
		}
		break;
		case AstNodeType_e::WeakIncludeItemDecl:
			{
				auto weakIncludeDecl = ast::safe_cast<ast::WeakIncludeItemDecl>(node);
				return canBeType(weakIncludeDecl->referencedNode);
			}
			break;

		default:
			break;
		}
		return false;
	}

	ast::AstNode* const
	ResolveTypes::extractTraitForFromList(NodeList& list)
	{
		ast::AstNode* foundNode = nullptr;
		for (auto node : list)
		{
			if (node->nodeType != AstNodeType_e::TraitForDecl)
			{
				foundNode = node;
				break;
			}
		}
		return foundNode;
	}
} }