#pragma once
#include "scope\fl_scope.h"
#include "fl_collection.h"

namespace fluffy { namespace ast {
	class AstNode;
	class CodeUnit;
} }

namespace fluffy { namespace scope {
	class ScopeManager;
	class Scope;

	/**
	 * ProcessNode
	 */

	class ProcessNode
	{
	public:
		ProcessNode() {}
		virtual ~ProcessNode() {}

		virtual void
		onBeginProcess(ScopeManager* const scopeManager, ast::AstNode* const node) = 0;

		virtual void
		onEndProcess(ScopeManager* const scopeManager, ast::AstNode* const node) = 0;

	};

	/**
	 * ScopeManager
	 */

	class ScopeManager
	{
	public:
		ScopeManager();
		~ScopeManager();

		void
		insertCodeUnit(ast::CodeUnit* const codeUnit);

		void
		applyOnTree(ast::CodeUnit* const codeUnit, ProcessNode* const processNode);

		Scope
		getParentScope();

		Scope
		getRootScope();

	private:
		void
		applyOnNode(ast::AstNode* const node, ProcessNode* const processNode);

		void
		pushScope(ast::AstNode* const node);

		void
		popScope();

	private:
		NodeMultiMap
		mReferenceTree;

		NodeList
		mScopeStack;

	};
} }