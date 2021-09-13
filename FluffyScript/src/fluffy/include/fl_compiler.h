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
	class NodeProcessor;
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
		initialize(String basePath);

		void
		build(String sourceFile);

		void
		build();

		void
		addBlockToBuild(String sourceFile, String sourceCode);

		void
		setNumberOfJobs(U32 jobCount);

		void
		applyTransformation(scope::NodeProcessor* const transformationProcessor);

		void
		applyValidation(scope::NodeProcessor* const validationProcessor);

	private:
		void
		buildInternal(String sourceFile);

	private:
		std::unordered_map<const TString, std::unique_ptr<ast::CodeUnit>, TStringHash, TStringEqual>
		mApplicationTree;

		std::vector<ast::CodeUnit*>
		mExecutionTree;

		std::unique_ptr<scope::ScopeManager>
		mScopeManager;

		std::vector<std::unique_ptr<scope::NodeProcessor>>
		mNodeProcessorList;

		String
		mBasePath;

		U32
		mJobCount;

		Bool
		mInitialized;

		Bool
		mBuildBlock;
	};
}