#pragma once
#include "scope\fl_scope.h"
#include "fl_collections.h"

namespace fluffy { namespace ast {
	class AstNode;
	class CodeUnit;
} }

namespace fluffy { namespace scope {
	class ScopeManager;
	class Scope;

	/**
	 * NodeProcessor
	 */

	class NodeProcessor
	{
	public:
		NodeProcessor() {}
		virtual ~NodeProcessor() {}

		virtual void
		onProcess(ScopeManager* const scopeManager, ast::AstNode* const node) = 0;

	};

	/**
	 * ExternalRequestAccess_s
	 */

	struct ExternalRequestAccess_s
	{
		// Indica o escopo e o code unit que fizeram a solicitacao.
		ast::AstNode* const requestorCodeUnit;
		ast::AstNode* const requesteeCodeUnit;
	};

	/**
	 * InternalRequestAccess_s
	 */

	struct InternalRequestAccess_s
	{
		// Indica o escopo e o code unit que fizeram a solicitacao.
		ast::AstNode* const requestorScope;
		ast::AstNode* const requesteeScope;
	};

	/**
	 * ScopeRelationship_e
	 */

	enum class ScopeRelationship_e
	{
		Irrelevant,
		Itself,
		Child,
		None
	};

	/**
	 * ScopeManager
	 */

	class ScopeManager
	{
	public:
		ScopeManager();
		virtual ~ScopeManager();

		void
		insertCodeUnit(ast::CodeUnit* const codeUnit);

		void
		copyReferenceTree(ScopeManager* const scopeManager);

		void
		processCodeUnit(ast::CodeUnit* const codeUnit, NodeProcessor* const nodeProcessor);

		void
		setCodeUnit(ast::CodeUnit* const codeUnit);

		ast::CodeUnit* const
		findCodeUnitByName(const TString& identifier);

		FindResult_t
		findNodeById(const TString& identifier, Bool findInRoot);

		FindResult_t
		findNodeById(const TString& identifier, ast::AstNode* const ignoredNode, Bool findInRoot);

		FindResult_t
		findNodeById(const TString& identifier, const AstNodeType_e ignoredType, Bool findInRoot);

		FindResult_t
		findAllNodeById(const TString& identifier);

		void
		interrupt();

		Bool
		getInterruptFlag();

		Scope
		getParentScope();

		Scope
		getRootScope();

		NodeMultiMap&
		getReferenceTree();

		const TString&
		getCodeUnitName();

	private:
		void
		processNode(ast::AstNode* const node, NodeProcessor* const nodeProcessor);

		void
		pushScope(ast::AstNode* const node);

		void
		popScope();

	private:
		NodeMultiMap
		mReferenceTree;

		NodeList
		mScopeStack;

		ast::CodeUnit*
		mCodeUnit;

		Bool
		mInterruptFlag;

	};

} }