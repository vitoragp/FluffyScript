#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <set>
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class AstNode;
	class NamespaceDecl;
	class CodeUnit;
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
		buildMultiThread(std::vector<String> sourceFileList);

		void
		buildSingleThread(std::vector<String> sourceFileList);

		void
		setNumberOfJobs(U32 jobCount);

	private:
		std::unique_ptr<parser::Parser>
		m_parser;

		std::unordered_map<const I8*, std::unique_ptr<ast::CodeUnit>>
		m_rootScope;

		std::vector<ast::AstNode*>
		m_scopeReference;

		U32
		m_jobCount;

		Bool
		m_initialized;
	};
}