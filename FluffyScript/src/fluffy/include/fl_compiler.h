#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <set>
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class AstNode;
	class NamespaceDecl;
} }

namespace fluffy { namespace parser {
	class Parser;
} }

namespace fluffy { namespace jobs {
	class JobPool;
} }

namespace fluffy {
	/**
	 * Compiler
	 */

	class Compiler
	{
	public:
		Compiler();
		virtual ~Compiler();

		void
		initialize();

		void
		build(std::vector<String> sourceFileList);

		void
		setNumberOfJobs(U32 jobCount);

	private:
		void
		mergeOrInsertNamespaceToRoot(const I8* sourceFilename, std::unique_ptr<ast::NamespaceDecl> nsDecl);
		
		void
		mergeOrInsertChildNamespace(const I8* sourceFilename, ast::NamespaceDecl* destinationNS, std::unique_ptr<ast::NamespaceDecl> sourceNs);

	private:
		std::unique_ptr<parser::Parser>
		m_parser;

		std::unordered_map<const I8*, std::unique_ptr<ast::NamespaceDecl>>
		m_rootScope;

		std::vector<ast::AstNode*>
		m_scopeReference;

		U32
		m_jobCount;

		Bool
		m_initialized;
	};
}