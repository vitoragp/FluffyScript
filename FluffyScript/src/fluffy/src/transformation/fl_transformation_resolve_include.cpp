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
		: mScopeManager(nullptr)
		, mIncludedScope(nullptr)
		, mCreatedIncludedScope(false)
	{}

	ResolveInclude::~ResolveInclude()
	{}

	void
	ResolveInclude::onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
	{
		if (mScopeManager == nullptr)
		{
			mScopeManager = scopeManager;
		}

		if (node->nodeType == AstNodeType_e::IncludeDecl)
		{
			// Busca um atributo ja
			mIncludedScope = scopeManager->getRootScope().getNode()->getOrCreateAttribute<attributes::IncludedScope>();

			// Prepara o escopo raiz do code unit da include.
			auto includeDecl = ast::safe_cast<ast::IncludeDecl>(node);

			auto includeScopeManager = scope::ScopeManager();
			includeScopeManager.copyReferenceTree(scopeManager);
			includeScopeManager.setCodeUnit(includeScopeManager.findCodeUnitByName(includeDecl->inFile));

			auto includeRootScope = includeScopeManager.getRootScope();
			validateScope(includeRootScope.getNode());

			ast::for_each(this, &ResolveInclude::processIncludeItem, includeDecl->includedItemList, includeDecl, includeRootScope);
		}
	}

	void
	ResolveInclude::processIncludeItem(ast::IncludeItemDecl* const includeItemDecl, ast::IncludeDecl* const includeDecl, scope::Scope& includeRootScope)
	{
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
					mScopeManager->getCodeUnitName().str(), nextId->identifier.str()
				);
			}

			if (!checkNodeVisibility(findResult.nodeList[0]))
			{
				throw exceptions::custom_exception(
					"%s error: Trying to attempt include a unimportable element '%s'",
					includeItemDecl->line, includeItemDecl->column,
					mScopeManager->getCodeUnitName().str(), nextId->identifier.str()
				);
			}
			scope.assign(findResult.scope, findResult.nodeList[0]);
			validateScope(scope.getNode());

			if (nextId->scopedChildPath)
			{
				nextId = nextId->scopedChildPath.get();
			}
			else
			{
				if (!includeItemDecl->includeAll)
				{
					TString includedIdentifier;

					scope::FindResult_t findResult;					
					if (includeItemDecl->referencedAlias == TString(nullptr))
					{
						findResult = scope.findNodeById(includeItemDecl->identifier);
						includedIdentifier = includeItemDecl->identifier;
					}
					else
					{
						findResult = scope.findNodeById(includeItemDecl->referencedAlias);
						includedIdentifier = includeItemDecl->referencedAlias;
					}

					if (!findResult.foundResult)
					{
						throw exceptions::custom_exception(
							"%s error: Failed to resolve include item '%s'",
							includeItemDecl->line, includeItemDecl->column,
							mScopeManager->getCodeUnitName().str(), includeItemDecl->identifier.str()
						);
					}

					if (!findResult.foundResult)
					{
						throw exceptions::custom_exception(
							"%s error: Failed to resolve include item '%s'",
							includeItemDecl->line, includeItemDecl->column,
							mScopeManager->getCodeUnitName().str(), includeItemDecl->identifier.str()
						);
					}

					for (auto includedNode : findResult.nodeList)
					{
						validateIncludedNode(includedNode, includeItemDecl);

						if (includedNode->nodeType == AstNodeType_e::TraitForDecl)
						{
							mIncludedScope->insertTraitDefinitionNode(includeItemDecl->identifier, includedNode);
						}
						else
						{
							mIncludedScope->insertIncludedNode(includeItemDecl->identifier, scope.getNode(), includedNode);
						}
					}
					break;
				}
				else
				{
					transformations::for_each(scope.toMap(), [includeItemDecl, &scope, this](ast::AstNode* const includedNode) {
						if (checkNodeVisibility(includedNode))
						{
							validateIncludedNode(includedNode, includeItemDecl);

							if (includedNode->nodeType == AstNodeType_e::TraitForDecl)
							{
								mIncludedScope->insertTraitDefinitionNode(includedNode->identifier, includedNode);
							}
							else
							{
								mIncludedScope->insertWeakIncludedNode(includedNode->identifier, scope.getNode(), includedNode);
							}
						}
					});

					break;
				}
			}
		}
	}

	void ResolveInclude::validateIncludedNode(fluffy::ast::AstNode* const includedNode, fluffy::ast::AstNode* const includedItem)
	{
		if (!checkNodeVisibility(includedNode))
		{
			throw exceptions::custom_exception(
				"%s error: Trying to attempt include a unimportable element '%s'",
				includedItem->line, includedItem->column,
				mScopeManager->getCodeUnitName().str(), includedNode->identifier.str()
			);
		}

		if (includedNode->nodeType == AstNodeType_e::ClassFunctionDecl)
		{
			validateClassMember(includedNode, includedItem);
		}
		else if (includedNode->nodeType == AstNodeType_e::ClassVariableDecl)
		{
			validateClassMember(includedNode, includedItem);
		}
	}

	void ResolveInclude::validateClassMember(fluffy::ast::AstNode* const includedNode, fluffy::ast::AstNode* const includedItem)
	{
		switch (includedNode->nodeType)
		{
		case AstNodeType_e::ClassFunctionDecl:
			{
				auto classFunction = ast::safe_cast<ast::ClassFunctionDecl>(includedNode);
				if (!classFunction->isStatic)
				{
					throw exceptions::custom_exception(
						"%s error: Failed to include '%s' function, only static functions can be included",
						includedItem->line, includedItem->column,
						mScopeManager->getCodeUnitName().str(), classFunction->identifier.str()
					);
				}
			}
			break;

		case AstNodeType_e::ClassVariableDecl:
			{
				auto classVariable = ast::safe_cast<ast::ClassVariableDecl>(includedNode);
				if (!classVariable->isStatic)
				{
					throw exceptions::custom_exception(
						"%s error: Failed to include '%s' variable, only static variables can be included",
						includedItem->line, includedItem->column,
						mScopeManager->getCodeUnitName().str(), classVariable->identifier.str()
					);
				}
			}
			break;

		default:
			break;
		}
	}

	Bool
	ResolveInclude::checkNodeVisibility(ast::AstNode* node)
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

	void
	ResolveInclude::validateScope(ast::AstNode* const node)
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