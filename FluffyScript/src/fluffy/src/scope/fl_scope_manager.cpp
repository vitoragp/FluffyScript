#include "ast\fl_ast_decl.h"
#include "scope\fl_scope.h"
#include "scope\fl_scope_manager.h"
#include "utils\fl_scope_utils.h"
#include "fl_exceptions.h"
namespace fluffy { namespace scope {
	/**
	 * ScopeManager
	 */

	ScopeManager::ScopeManager()
	{}

	ScopeManager::~ScopeManager()
	{}

	void
	ScopeManager::insertCodeUnit(ast::CodeUnit* const codeUnit)
	{
		mReferenceTree.emplace(codeUnit->identifier, codeUnit);
	}

	void
	ScopeManager::applyOnTree(ast::CodeUnit* const codeUnit, ProcessNode* const processNode)
	{
		applyOnNode(codeUnit, processNode);
	}

	Scope
	ScopeManager::getParentScope()
	{
		auto itNode = mScopeStack.rbegin();
		if ((*itNode)->nodeType == AstNodeType_e::CodeUnit) {
			return Scope(nullptr, (*itNode));
		}
		auto itScope = mScopeStack.rbegin() + 1;
		return Scope(*itScope, *itNode);
	}

	Scope
	ScopeManager::getRootScope()
	{
		return Scope(nullptr, mScopeStack.front());
	}

	void
	ScopeManager::applyOnNode(ast::AstNode* const node, ProcessNode* const processNode)
	{
		// Faz a validação no nodo.
		processNode->onBeginProcess(this, node);

		// Faz a validação nos filhos.
		switch (node->nodeType)
		{
		case AstNodeType_e::CodeUnit:
			if (auto n = reinterpret_cast<ast::CodeUnit*>(node))
			{
				pushScope(n);

				ast::for_each(this, &ScopeManager::applyOnNode, n->includeDeclList, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->namespaceDeclList, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::IncludeDecl:
			if (auto n = reinterpret_cast<ast::IncludeDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->includedItemList, processNode);
			}
			break;

		case AstNodeType_e::IncludeItemDecl:
			if (auto n = reinterpret_cast<ast::IncludeItemDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->referencedPath, processNode);
			}
			break;

		case AstNodeType_e::NamespaceDecl:
			if (auto n = reinterpret_cast<ast::NamespaceDecl*>(node))
			{
				pushScope(n);

				ast::for_each(this, &ScopeManager::applyOnNode, n->namespaceDeclList, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->generalDeclList, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassDecl:
			if (auto n = reinterpret_cast<ast::ClassDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);

				ast::for_single(this, &ScopeManager::applyOnNode, n->baseClass, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->interfaceList, processNode);

				ast::for_each(this, &ScopeManager::applyOnNode, n->constructorList, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->functionList, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->variableList, processNode);

				ast::for_single(this, &ScopeManager::applyOnNode, n->destructorDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassConstructorDecl:
			if (auto n = reinterpret_cast<ast::ClassConstructorDecl*>(node))
			{
				pushScope(n);

				ast::for_each(this, &ScopeManager::applyOnNode, n->parameterList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->superInitExpr, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->variableInitDeclList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassDestructorDecl:
			if (auto n = reinterpret_cast<ast::ClassDestructorDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassFunctionDecl:
			if (auto n = reinterpret_cast<ast::ClassFunctionDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->sourceTypeDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->parameterList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->returnType, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::ClassVariableDecl:
			if (auto n = reinterpret_cast<ast::ClassVariableDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->initExpr, processNode);
			}
			break;

		case AstNodeType_e::ClassVariableInitDecl:
			if (auto n = reinterpret_cast<ast::ClassVariableInitDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->initExpr, processNode);
			}
			break;

		case AstNodeType_e::InterfaceDecl:
			if (auto n = reinterpret_cast<ast::InterfaceDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->functionDeclList, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::InterfaceFunctionDecl:
			if (auto n = reinterpret_cast<ast::InterfaceFunctionDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->parameterList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->returnType, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::StructDecl:
			if (auto n = reinterpret_cast<ast::StructDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->variableList, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::StructVariableDecl:
			if (auto n = reinterpret_cast<ast::StructVariableDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->initExpr, processNode);
			}
			break;

		case AstNodeType_e::EnumDecl:
			if (auto n = reinterpret_cast<ast::EnumDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->enumItemDeclList, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::EnumItemDecl:
			if (auto n = reinterpret_cast<ast::EnumItemDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->dataTypeDeclList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->valueExpression, processNode);
			}
			break;

		case AstNodeType_e::TraitDecl:
			if (auto n = reinterpret_cast<ast::TraitDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->functionDeclList, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::TraitForDecl:
			if (auto n = reinterpret_cast<ast::TraitForDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDefinitionDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->functionDeclList, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::TraitFunctionDecl:
			if (auto n = reinterpret_cast<ast::TraitFunctionDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->parameterList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->returnType, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::FunctionDecl:
			if (auto n = reinterpret_cast<ast::FunctionDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->genericDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->parameterList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->returnType, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::VariableDecl:
			if (auto n = reinterpret_cast<ast::VariableDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->initExpr, processNode);
			}
			break;

		case AstNodeType_e::FunctionParameterDecl:
			if (auto n = reinterpret_cast<ast::FunctionParameterDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->patternDecl, processNode);
			}
			break;

		case AstNodeType_e::GenericDecl:
			if (auto n = reinterpret_cast<ast::GenericDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->genericDeclItemList, processNode);
			}
			break;

		case AstNodeType_e::GenericItemDecl:
			if (auto n = reinterpret_cast<ast::GenericItemDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->whereTypeList, processNode);
			}
			break;

		case AstNodeType_e::ScopedIdentifierDecl:
			if (auto n = reinterpret_cast<ast::ScopedIdentifierDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->referencedIdentifier, processNode);
			}
			break;

		case AstNodeType_e::Block:
			if (auto n = reinterpret_cast<ast::BlockDecl*>(node))
			{
				pushScope(n);

				ast::for_each(this, &ScopeManager::applyOnNode, n->stmtList, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::StmtIf:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->conditionExprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->ifBlockDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->elseBlockDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtIfLet:
			if (auto n = reinterpret_cast<ast::stmt::StmtIfLetDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->patternDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->ifBlockDecl, processNode);				
				ast::for_single(this, &ScopeManager::applyOnNode, n->elseBlockDecl, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::StmtFor:
			if (auto n = reinterpret_cast<ast::stmt::StmtForDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->initStmtDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->initExprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->conditionExprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->updateExprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtWhileDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->conditionExprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtDoWhile:
			if (auto n = reinterpret_cast<ast::stmt::StmtDoWhileDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->conditionExprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::StmtMatch:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->conditionExprDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->whenDeclList, processNode);
			}
			break;

		case AstNodeType_e::StmtReturn:
			if (auto n = reinterpret_cast<ast::stmt::StmtReturnDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
			}
			break;

		case AstNodeType_e::StmtTry:
			if (auto n = reinterpret_cast<ast::stmt::StmtTryDecl*>(node))
			{
				pushScope(n);
				
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				
				popScope();

				ast::for_each(this, &ScopeManager::applyOnNode, n->catchDeclList, processNode);
			}
			break;

		case AstNodeType_e::StmtPanic:
			if (auto n = reinterpret_cast<ast::stmt::StmtPanicDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
			}
			break;

		case AstNodeType_e::StmtVariable:
			if (auto n = reinterpret_cast<ast::stmt::StmtVariableDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->initExpr, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->patternDecl, processNode);
			}
			break;

		case AstNodeType_e::StmtExpr:
			if (auto n = reinterpret_cast<ast::stmt::StmtExprDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
			}
			break;

		case AstNodeType_e::StmtForInitDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtForInitDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->initExpr, processNode);
			}
			break;

		case AstNodeType_e::StmtMatchWhenDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtMatchWhenDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->patternDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::StmtCatchBlockDecl:
			if (auto n = reinterpret_cast<ast::stmt::StmtCatchBlockDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->patternDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::TernaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionTernaryDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->conditionDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->leftDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->rightDecl, processNode);
			}
			break;

		case AstNodeType_e::BinaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionBinaryDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->leftDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->rightDecl, processNode);
			}
			break;

		case AstNodeType_e::UnaryExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionUnaryDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
			}
			break;

		case AstNodeType_e::AsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAsDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
			}
			break;

		case AstNodeType_e::IsExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIsDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
			}
			break;

		case AstNodeType_e::MatchExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->whenDeclList, processNode);
			}
			break;

		case AstNodeType_e::FunctionDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionDecl*>(node))
			{
				pushScope(n);

				ast::for_each(this, &ScopeManager::applyOnNode, n->parametersDeclList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->returnTypeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->blockDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::FunctionCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionCall*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->lhsDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->rhsDecl, processNode);
			}
			break;

		case AstNodeType_e::GenericCallExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionGenericCallDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->genericTypeList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->lhsDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->rhsDecl, processNode);
			}
			break;

		case AstNodeType_e::IndexExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionIndexDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->lhsDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->rhsDecl, processNode);
			}
			break;

		case AstNodeType_e::NewExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->objTypeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->objInitBlockDecl, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::AnomClassDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionAnomClassDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->baseReferencedDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->functionList, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->variableList, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->constructorList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->destructorDecl, processNode);
				
				popScope();
			}
			break;

		case AstNodeType_e::PrimitiveTypeExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionPrimitiveTypeDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
			}
			break;

		case AstNodeType_e::ArrayInitExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionArrayInitDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->arrayElementDeclList, processNode);
			}
			break;

		case AstNodeType_e::MatchWhenExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionMatchWhenDecl*>(node))
			{
				pushScope(n);

				ast::for_single(this, &ScopeManager::applyOnNode, n->patternDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);

				popScope();
			}
			break;

		case AstNodeType_e::FunctionParameterDeclExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionFunctionParameterDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->typeDecl, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->patternDecl, processNode);
			}
			break;

		case AstNodeType_e::NewBlockExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewBlockDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->itemDeclList, processNode);
			}
			break;

		case AstNodeType_e::NewItemExpr:
			if (auto n = reinterpret_cast<ast::expr::ExpressionNewItemDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->exprDecl, processNode);
			}
			break;

		case AstNodeType_e::LiteralPattern:
			if (auto n = reinterpret_cast<ast::pattern::LiteralPatternDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->literalExpr, processNode);
			}
			break;

		case AstNodeType_e::TuplePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::TuplePatternDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->patternItemDeclList, processNode);
			}
			break;

		case AstNodeType_e::StructurePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructurePatternDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->structureItemDeclList, processNode);
			}
			break;

		case AstNodeType_e::EnumerablePatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::EnumerablePatternDecl*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->patternDeclItemList, processNode);
			}
			break;

		case AstNodeType_e::StructureItemPatternDecl:
			if (auto n = reinterpret_cast<ast::pattern::StructureItemPatternDecl*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->referencedPattern, processNode);
			}
			break;

		case AstNodeType_e::ArrayType:
			if (auto n = reinterpret_cast<ast::TypeDeclArray*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->arrayDeclList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->valueType, processNode);
			}
			break;

		case AstNodeType_e::FunctionType:
			if (auto n = reinterpret_cast<ast::TypeDeclFunction*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->objectOwnerDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->parameterTypeList, processNode);
				ast::for_single(this, &ScopeManager::applyOnNode, n->returnType, processNode);
			}
			break;

		case AstNodeType_e::TupleType:
			if (auto n = reinterpret_cast<ast::TypeDeclTuple*>(node))
			{
				ast::for_each(this, &ScopeManager::applyOnNode, n->tupleItemList, processNode);
			}
			break;

		case AstNodeType_e::NamedType:
			if (auto n = reinterpret_cast<ast::TypeDeclNamed*>(node))
			{
				ast::for_single(this, &ScopeManager::applyOnNode, n->scopedReferenceDecl, processNode);
				ast::for_each(this, &ScopeManager::applyOnNode, n->genericDefinitionList, processNode);
			}
			break;

		default:
			break;
		}

		// Faz a validação no nodo.
		processNode->onEndProcess(this, node);
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