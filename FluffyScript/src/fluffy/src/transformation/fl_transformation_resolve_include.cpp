#include "ast\fl_ast_decl.h"
#include "utils\fl_ast_utils.h"
#include "transformation\fl_transformation_resolve_include.h"
namespace fluffy { namespace transformations {
	template <
		typename TList,
		typename TFunc,
		typename ...TArgs
	> void
	for_each_ptr(TList& list, TFunc func, TArgs...args)
	{
		auto itBegin = list.begin();
		auto itEnd = list.end();

		while (itBegin != itEnd)
		{
			func(itBegin->get(), args...);			
			itBegin++;
		}
	}

	template <
		typename TList,
		typename TFunc,
		typename ...TArgs
	> void
	for_each(TList& list, TFunc func, TArgs...args)
	{
		auto itBegin = list.begin();
		auto itEnd = list.end();

		while (itBegin != itEnd)
		{
			func(itBegin->second, args...);			
			itBegin++;
		}
	}

	/**
	 * ResolveInclude
	 */

	ResolveInclude::ResolveInclude()
	{}

	ResolveInclude::~ResolveInclude()
	{}

	void
	ResolveInclude::onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
	{
		if (node->nodeType == AstNodeType_e::IncludeDecl)
		{
			// Prepara o escopo raiz do code unit da include.
			auto includeDecl = ast::safe_cast<ast::IncludeDecl>(node);

			auto includeScopeManager = scope::ScopeManager();
			includeScopeManager.copyReferenceTree(scopeManager);
			includeScopeManager.setCodeUnit(includeScopeManager.findCodeUnitByName(includeDecl->inFile));

			auto includeRootScope = includeScopeManager.getRootScope();

			ast::for_each(this, &ResolveInclude::processIncludeItem, includeDecl->includedItemList, scopeManager, includeDecl, includeRootScope);

			for (auto& weakIncludeItem : mWeakIncludeItemList)
			{
				includeDecl->includedItemList.emplace_back(weakIncludeItem);
			}
			mWeakIncludeItemList.clear();
		}
	}

	void
	ResolveInclude::processIncludeItem(ast::BaseIncludeItemDecl* const baseIncludeItemDecl, scope::ScopeManager* const scopeManager, ast::IncludeDecl* const includeDecl, scope::Scope& includeRootScope)
	{
		auto includeItemDecl = ast::safe_cast<ast::IncludeItemDecl>(baseIncludeItemDecl);

		scope::Scope scope = includeRootScope;
		auto nextId = includeItemDecl->scopePath.get();

		while (nextId)
		{
			auto findResult = scope.findNodeById(nextId->identifier);
			
			if (!findResult.foundResult)
			{
				throw exceptions::custom_exception(
					"%s error: Failed to resolve include item '%s'",
					includeItemDecl->line, includeItemDecl->column,
					scopeManager->getCodeUnitName().str(),
					nextId->identifier.str()
				);
			}

			if (!checkNodeVisibility(scopeManager, findResult.nodeList[0]))
			{
				throw exceptions::custom_exception(
					"%s error: Trying to attempt a unexportable element '%s'",
					includeItemDecl->line, includeItemDecl->column,
					scopeManager->getCodeUnitName().str(),
					nextId->identifier.str()
				);
			}
			scope.assign(findResult.scope, findResult.nodeList[0]);

			if (nextId->scopedChildPath)
			{
				nextId = nextId->scopedChildPath.get();
			}
			else
			{
				if (!includeItemDecl->includeAll)
				{
					auto findResult = scope.findNodeById(includeItemDecl->identifier);

					if (!findResult.foundResult)
					{
						throw exceptions::custom_exception(
							"%s error: Failed to resolve include item '%s'",
							includeItemDecl->line, includeItemDecl->column,
							scopeManager->getCodeUnitName().str(),
							includeItemDecl->identifier.str()
						);
					}

					Bool hasImportableNode = false;
					for (auto includedNodeIt = findResult.nodeList.begin(); includedNodeIt != findResult.nodeList.end();)
					{
						auto includedNode = *includedNodeIt;

						if (includedNode->nodeType != AstNodeType_e::TraitForDecl)
						{
							if (!checkNodeVisibility(scopeManager, includedNode))
							{
								throw exceptions::custom_exception(
									"%s error: Trying to attempt a unexportable element '%s'",
									includeItemDecl->line, includeItemDecl->column,
									scopeManager->getCodeUnitName().str(),
									includeItemDecl->identifier.str()
								);
							}
							hasImportableNode = true;
						}
						else
						{
							if (!checkNodeVisibility(scopeManager, includedNode))
							{
								includedNodeIt = findResult.nodeList.erase(includedNodeIt);
								continue;
							}
						}
						includedNodeIt++;
					}

					if (!hasImportableNode)
					{
						throw exceptions::custom_exception(
							"%s error: Failed to resolve include item '%s'",
							includeItemDecl->line, includeItemDecl->column,
							scopeManager->getCodeUnitName().str(),
							utils::AstUtils::printIncludeItem(includeItemDecl).c_str()
						);
					}

					includeItemDecl->hasBeenResolved = true;
					includeItemDecl->referencedScope = findResult.scope;
					includeItemDecl->referencedNodeList = std::move(findResult.nodeList);

					break;
				}
				else
				{
					transformations::for_each(scope.toMap(), [scopeManager, includeItemDecl, includeDecl, &scope, this](ast::AstNode* const node) {
						if (checkNodeVisibility(scopeManager, node))
						{
							auto weakInclude = new ast::WeakIncludeItemDecl(
								includeItemDecl->line, includeItemDecl->column
							);

							weakInclude->identifier = node->identifier;
							
							weakInclude->scopePath = includeItemDecl->scopePath.get();
							weakInclude->referencedScope = scope.getNode();
							weakInclude->referencedNode = node;

							mWeakIncludeItemList.push_back(weakInclude);
						}
					});

					break;
				}
			}
		}
	}

	Bool
	ResolveInclude::checkNodeVisibility(scope::ScopeManager* const scopeManager, ast::AstNode* node)
	{
		switch (node->nodeType)
		{
		case AstNodeType_e::NamespaceDecl:
			break;
		case AstNodeType_e::ClassDecl:
		case AstNodeType_e::InterfaceDecl:
		case AstNodeType_e::StructDecl:
		case AstNodeType_e::TraitDecl:
		case AstNodeType_e::TraitForDecl:
		case AstNodeType_e::EnumDecl:
		case AstNodeType_e::FunctionDecl:
		case AstNodeType_e::VariableDecl:
			{
				if (auto generalDecl = dynamic_cast<ast::GeneralStmtDecl*>(node))
				{
					return generalDecl->isExported;
				}
				else
				{
					throw exceptions::custom_exception("checkNodeVisibility: badcast");
				}
			}
			break;
		default:
			break;
		}
		return true;
	}
} }