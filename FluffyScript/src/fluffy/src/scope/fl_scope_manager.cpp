#include <functional>
#include "ast\fl_ast_decl.h"
#include "scope\fl_scope.h"
#include "scope\fl_scope_manager.h"
#include "utils\fl_scope_utils.h"
#include "fl_exceptions.h"
namespace fluffy { namespace scope {
	template <typename T>
	T* safe_cast(ast::AstNode* const node)
	{
		if (node == nullptr || node->nodeType != T::getReferenceType())
		{
			throw exceptions::custom_exception("Invalid AstNode cast");
		}
		return reinterpret_cast<T*>(node);
	}

	/**
	 * Funcoes auxiliares
	 */

	template <
		typename TClass,
		typename TList,
		typename TFunc,
		typename ...TArgs
	> void
	for_each_i(TClass owner, TFunc func, TList& list, TArgs...args)
	{
		auto itBegin = list.begin();
		auto itEnd = list.end();

		while (itBegin != itEnd)
		{
			if (owner->getInterruptFlag())
			{
				return;
			}
			(owner->* func)(itBegin->get(), args...);
			itBegin++;
		}
	}

	template <
		typename TClass,
		typename TUnique,
		typename TFunc,
		typename ...TArgs
	> void
	for_single_i(TClass owner, TFunc func, TUnique& unique, TArgs...args)
	{
		if (unique != nullptr && !owner->getInterruptFlag()) {
			(owner->* func)(unique.get(), args...);
		}
	}

	/**
	 * ScopeManager
	 */

	ScopeManager::ScopeManager()
		: mCodeUnit(nullptr)
		, mInterruptFlag(false)
		
	{}

	ScopeManager::~ScopeManager()
	{}

	void
	ScopeManager::insertCodeUnit(ast::CodeUnit* const codeUnit)
	{
		mReferenceTree.emplace(codeUnit->identifier, codeUnit);
	}

	void
	ScopeManager::copyReferenceTree(ScopeManager* const scopeManager)
	{
		getReferenceTree().insert(
			scopeManager->getReferenceTree().begin(),
			scopeManager->getReferenceTree().end()
		);
	}

	void
	ScopeManager::processCodeUnit(ast::CodeUnit* const codeUnit, NodeProcessor* const nodeProcessor)
	{
		mCodeUnit = codeUnit;
		processNode(codeUnit, nodeProcessor);
	}

	/*
	ScopeRelationship_e
	ScopeManager::getScopeRelationship(ast::AstNode* const requestor, ast::AstNode* const requestee)
	{
		ScopeRelationship_e relationship = ScopeRelationship_e::Irrelevant;

		if (requestee->nodeType == AstNodeType_e::ClassDecl)
		{
			if (requestor == requestee)
			{
				relationship = ScopeRelationship_e::Itself;
			}
			else
			{
				if (requestor->nodeType == AstNodeType_e::ClassDecl)
				{
					auto requestorParentClass = utils::ScopeUtils::resolveExtendsClass(this, requestor);

					if (requestorParentClass == requestee)
					{
						relationship = ScopeRelationship_e::Child;
					}
					relationship = ScopeRelationship_e::None;
				}
				else
				{
					relationship = ScopeRelationship_e::None;
				}
			}
		}
		return relationship;
	}
	*/

	void
	ScopeManager::setCodeUnit(ast::CodeUnit* const codeUnit)
	{
		mScopeStack.push_back(codeUnit);
	}

	ast::CodeUnit* const
	ScopeManager::findCodeUnitByName(const TString& identifier)
	{
		auto it = mReferenceTree.find(identifier);
		return it != mReferenceTree.end() ? ast::safe_cast<ast::CodeUnit>(it->second) : nullptr;
	}

	FindResult_t
	ScopeManager::findNodeById(const TString& identifier, Bool findInRoot)
	{
		if (findInRoot)
		{
			return getRootScope().findNodeById(identifier);
		}
		else
		{
			auto beginScope = mScopeStack.rbegin();
			auto endScope = mScopeStack.rend();

			for (; beginScope != endScope; beginScope++)
			{
				auto scopeNodeScope = beginScope + 1;
				auto scope = scope::Scope(this, scopeNodeScope == endScope ? nullptr : *scopeNodeScope, *beginScope);

				auto findResult = scope.findNodeById(identifier);
				if (findResult.foundResult) {
					return findResult;
				}
			}
		}
		return FindResult_t { nullptr, NodeList(), false };
	}

	FindResult_t
	ScopeManager::findNodeById(const TString& identifier, ast::AstNode* const ignoredNode, Bool findInRoot)
	{
		if (findInRoot)
		{
			return getRootScope().findNodeById(identifier);
		}
		else
		{
			auto beginScope = mScopeStack.rbegin();
			auto endScope = mScopeStack.rend();

			for (; beginScope != endScope; beginScope++)
			{
				auto scopeNodeScope = beginScope + 1;
				auto scope = scope::Scope(this, scopeNodeScope == endScope ? nullptr : *scopeNodeScope, *beginScope);

				auto findResult = scope.findNodeById(identifier);
				if (findResult.foundResult) {
					if (findResult.nodeList.size() > 1 || (findResult.nodeList.size() == 1 && findResult.nodeList[0] != ignoredNode)) {
						return findResult;
					}
				}
			}
		}
		return FindResult_t { nullptr, NodeList(), false };
	}

	FindResult_t
	ScopeManager::findNodeById(const TString& identifier, const AstNodeType_e ignoredType, Bool findInRoot)
	{
		if (findInRoot)
		{
			return getRootScope().findNodeById(identifier);
		}
		else
		{
			auto beginScope = mScopeStack.rbegin();
			auto endScope = mScopeStack.rend();

			for (; beginScope != endScope; beginScope++)
			{
				auto scopeNodeScope = beginScope + 1;
				auto scope = scope::Scope(this, scopeNodeScope == endScope ? nullptr : *scopeNodeScope, *beginScope);

				auto findResult = scope.findNodeById(identifier);
				if (findResult.foundResult) {
					Bool foundNode = false;
					for (auto it = findResult.nodeList.begin(); it != findResult.nodeList.end();) {
						auto node = *it;

						if (node->nodeType == ignoredType)
						{
							it = findResult.nodeList.erase(it);
						}
						else
						{
							foundNode = true;
							it++;
						}
					}

					if (foundNode)
					{
						return findResult;
					}
				}
			}
		}
		return FindResult_t { nullptr, NodeList(), false };
	}

	FindResult_t
	ScopeManager::findAllNodeById(const TString& identifier)
	{
		// TODO: Implementar uma consulta que recebe todos os nos com o mesmo identificar do escopo atual
		// ate a raiz.
		FindResult_t finalFindResult = { nullptr, NodeList(), false };
		return finalFindResult;
	}

	void
	ScopeManager::interrupt()
	{
		mInterruptFlag = true;
	}

	Bool
	ScopeManager::getInterruptFlag()
	{
		return mInterruptFlag;
	}

	Scope
	ScopeManager::getParentScope()
	{
		auto itNode = mScopeStack.rbegin();
		if ((*itNode)->nodeType == AstNodeType_e::CodeUnit) {
			return Scope(this, nullptr, (*itNode));
		}
		auto itScope = mScopeStack.rbegin() + 1;
		return Scope(this, *itScope, *itNode);
	}

	Scope
	ScopeManager::getRootScope()
	{
		return Scope(this, nullptr, mScopeStack.front());
	}

	NodeMultiMap&
	ScopeManager::getReferenceTree()
	{
		return mReferenceTree;
	}

	const TString&
	ScopeManager::getCodeUnitName()
	{
		return mCodeUnit->identifier;
	}

	void
	ScopeManager::processNode(ast::AstNode* const node, NodeProcessor* const nodeProcessor)
	{
		if (mInterruptFlag)
		{
			return;
		}

		// Faz a validação no nodo.
		nodeProcessor->onProcess(this, node);

		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(node))
			{
				pushScope(n);

				for_each_i(this, &ScopeManager::processNode, n->includeDeclList, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->namespaceDeclList, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::IncludeDecl:
			if (auto n = reinterpret_cast<ast::IncludeDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->includedItemList, nodeProcessor);
			}
			break;

		case AstNodeType_e::IncludeItemDecl:
			if (auto n = reinterpret_cast<ast::IncludeItemDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->scopePath, nodeProcessor);
			}
			break;

		case AstNodeType_e::NamespaceDecl:
			if (auto n = reinterpret_cast<ast::NamespaceDecl*>(node))
			{
				pushScope(n);

				for_each_i(this, &ScopeManager::processNode, n->namespaceDeclList, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->generalDeclList, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassDecl:
			if (auto n = reinterpret_cast<ast::ClassDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);

				for_single_i(this, &ScopeManager::processNode, n->baseClass, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->interfaceList, nodeProcessor);

				for_each_i(this, &ScopeManager::processNode, n->constructorList, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->functionList, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->variableList, nodeProcessor);

				for_single_i(this, &ScopeManager::processNode, n->destructorDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			if (auto n = reinterpret_cast<ast::ClassConstructorDecl*>(node))
			{
				pushScope(n);

				for_each_i(this, &ScopeManager::processNode, n->parameterList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->superInitExpr, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->variableInitDeclList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassDestructorDecl:
			if (auto n = reinterpret_cast<ast::ClassDestructorDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			if (auto n = reinterpret_cast<ast::ClassFunctionDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->sourceTypeDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->parameterList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->returnType, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassVariableDecl:
			if (auto n = reinterpret_cast<ast::ClassVariableDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->initExpr, nodeProcessor);
			}
			break;

		case AstNodeType_e::ClassVariableInitDecl:
			if (auto n = reinterpret_cast<ast::ClassVariableInitDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->initExpr, nodeProcessor);
			}
			break;

		case AstNodeType_e::InterfaceDecl:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->functionDeclList, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->parameterList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->returnType, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::StructDecl:
			if (auto n = reinterpret_cast<ast::StructDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->variableList, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::StructVariableDecl:
			if (auto n = reinterpret_cast<ast::StructVariableDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->initExpr, nodeProcessor);
			}
			break;

		case AstNodeType_e::EnumDecl:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->enumItemDeclList, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::EnumItemDecl:
			if (auto n = reinterpret_cast<ast::EnumItemDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->dataTypeDeclList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->valueExpression, nodeProcessor);
			}
			break;

		case AstNodeType_e::TraitDecl:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->functionDeclList, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::TraitForDecl:
			if (auto n = reinterpret_cast<ast::TraitForDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->typeDefinitionDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->functionDeclList, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			if (auto n = reinterpret_cast<ast::TraitFunctionDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->parameterList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->returnType, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::FunctionDecl:
			if (auto n = reinterpret_cast<ast::FunctionDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->genericDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->parameterList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->returnType, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::VariableDecl:
			if (auto n = reinterpret_cast<ast::VariableDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->initExpr, nodeProcessor);
			}
			break;

		case AstNodeType_e::FunctionParameterDecl:
			if (auto n = reinterpret_cast<ast::FunctionParameterDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->patternDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::GenericDecl:
			if (auto n = reinterpret_cast<ast::GenericDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->genericDeclItemList, nodeProcessor);
			}
			break;

		case AstNodeType_e::GenericItemDecl:
			if (auto n = reinterpret_cast<ast::GenericItemDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->whereTypeList, nodeProcessor);
			}
			break;

		case AstNodeType_e::ScopedPathDecl:
			if (auto n = reinterpret_cast<ast::ScopedPathDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->scopedChildPath, nodeProcessor);
			}
			break;

		case AstNodeType_e::Block:
			if (auto n = reinterpret_cast<ast::BlockDecl*>(node))
			{
				pushScope(n);

				for_each_i(this, &ScopeManager::processNode, n->stmtList, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::StmtIf:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->conditionExprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->ifBlockDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->elseBlockDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtIfLet:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfLetDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->patternDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->ifBlockDecl, nodeProcessor);				
				for_single_i(this, &ScopeManager::processNode, n->elseBlockDecl, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::StmtFor:
			if (auto n = reinterpret_cast<ast::stmt::StmtForDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->initStmtDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->initExprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->conditionExprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->updateExprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtWhileDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->conditionExprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtDoWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtDoWhileDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->conditionExprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtMatch:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->conditionExprDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->whenDeclList, nodeProcessor);
			}
			break;

		case AstNodeType_e::StmtReturn:
			if (auto n = reinterpret_cast<ast::stmt::StmtReturnDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::StmtTry:
			if (auto n = reinterpret_cast<ast::stmt::StmtTryDecl*>(node))
			{
				pushScope(n);
				
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				
				popScope();

				for_each_i(this, &ScopeManager::processNode, n->catchDeclList, nodeProcessor);
			}
			break;

		case AstNodeType_e::StmtPanic:
			if (auto n = reinterpret_cast<ast::stmt::StmtPanicDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::StmtVariable:
			if (auto n = reinterpret_cast<ast::stmt::StmtVariableDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->initExpr, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->patternDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::StmtExpr:
			if (auto n = reinterpret_cast<ast::stmt::StmtExprDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::StmtForInitDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtForInitDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->initExpr, nodeProcessor);
			}
			break;

		case AstNodeType_e::StmtMatchWhenDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchWhenDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->patternDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::StmtCatchBlockDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtCatchBlockDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->patternDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::TernaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionTernaryDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->conditionDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->leftDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->rightDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::BinaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionBinaryDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->leftDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->rightDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::UnaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionUnaryDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::AsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAsDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::IsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIsDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::MatchExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->whenDeclList, nodeProcessor);
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionDecl*>(node))
			{
				pushScope(n);

				for_each_i(this, &ScopeManager::processNode, n->parametersDeclList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->returnTypeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->blockDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::FunctionCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionCall*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->lhsDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->rhsDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::GenericCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionGenericCallDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->genericTypeList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->lhsDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->rhsDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::IndexExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIndexDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->lhsDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->rhsDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->objTypeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->objInitBlockDecl, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->baseReferencedDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->functionList, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->variableList, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->constructorList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->destructorDecl, nodeProcessor);
				
				popScope();
			}
			break;

		case AstNodeType_e::PrimitiveTypeExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionPrimitiveTypeDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::ArrayInitExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionArrayInitDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->arrayElementDeclList, nodeProcessor);
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(node))
			{
				pushScope(n);

				for_single_i(this, &ScopeManager::processNode, n->patternDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);

				popScope();
			}
			break;

		case AstNodeType_e::FunctionParameterDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->typeDecl, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->patternDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->itemDeclList, nodeProcessor);
			}
			break;

		case AstNodeType_e::NewItemExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewItemDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->exprDecl, nodeProcessor);
			}
			break;

		case AstNodeType_e::LiteralPattern:
			if (auto n = reinterpret_cast<ast::pattern::LiteralPatternDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->literalExpr, nodeProcessor);
			}
			break;

		case AstNodeType_e::TuplePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::TuplePatternDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->patternItemDeclList, nodeProcessor);
			}
			break;

		case AstNodeType_e::StructurePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructurePatternDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->structureItemDeclList, nodeProcessor);
			}
			break;

		case AstNodeType_e::EnumerablePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::EnumerablePatternDecl*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->patternDeclItemList, nodeProcessor);
			}
			break;

		case AstNodeType_e::StructureItemPatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructureItemPatternDecl*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->referencedPattern, nodeProcessor);
			}
			break;

		case AstNodeType_e::ArrayType:
			if (auto n = reinterpret_cast<ast::TypeDeclArray*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->arrayDeclList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->valueType, nodeProcessor);
			}
			break;

		case AstNodeType_e::FunctionType:
			if (auto n = reinterpret_cast<ast::TypeDeclFunction*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->objectOwnerDecl, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->parameterTypeList, nodeProcessor);
				for_single_i(this, &ScopeManager::processNode, n->returnType, nodeProcessor);
			}
			break;

		case AstNodeType_e::TupleType:
			if (auto n = reinterpret_cast<ast::TypeDeclTuple*>(node))
			{
				for_each_i(this, &ScopeManager::processNode, n->tupleItemList, nodeProcessor);
			}
			break;

		case AstNodeType_e::NamedType:
			if (auto n = reinterpret_cast<ast::TypeDeclNamed*>(node))
			{
				for_single_i(this, &ScopeManager::processNode, n->scopePath, nodeProcessor);
				for_each_i(this, &ScopeManager::processNode, n->genericDefinitionList, nodeProcessor);
			}
			break;

		default:
			break;
		}
	}

	void
	ScopeManager::pushScope(ast::AstNode* const node)
	{
		mScopeStack.emplace_back(node);
	}

	void
	ScopeManager::popScope()
	{
		mScopeStack.pop_back();
	}
	
} }