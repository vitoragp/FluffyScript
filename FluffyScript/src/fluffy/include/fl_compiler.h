#pragma once
#include "fl_defs.h"
#include "fl_string.h"

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

namespace fluffy { namespace transformation {
	class Transformation;
} }

namespace fluffy { namespace scope {
	class ScopeManager;
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
		initialize(String basePath);

		void
		build(String sourceFile);

		void
		setNumberOfJobs(U32 jobCount);

	private:
		void
		buildInternal(String sourceFile);

	private:
		std::unordered_map<String, std::unique_ptr<ast::CodeUnit>>
		mApplicationTree;

		std::vector<ast::CodeUnit*>
		mExecutionTree;

		std::unique_ptr<scope::ScopeManager>
		mScopeManager;

		String
		mBasePath;

		U32
		mJobCount;

		Bool
		mInitialized;
	};
}