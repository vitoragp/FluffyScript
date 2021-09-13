#pragma once
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class AstNode;

	namespace pattern {
		class PatternDecl;
	}
} }

namespace fluffy { namespace utils {
	/**
	 * AstUtils
	 */

	class AstUtils
	{
	public:
		static Bool
		equals(ast::AstNode* nodeA, ast::AstNode* nodeB);

		static std::vector<std::tuple<TString, U32, U32>>
		extractFunctionIdentifiers(ast::AstNode* node);

		static std::vector<std::tuple<TString, U32, U32>>
		extractPatternIdentifiers(ast::pattern::PatternDecl* pattern);

		static String
		printScopedPath(ast::AstNode* const scopedIdentifier);

		static String
		printIncludeItem(ast::AstNode* const includeItem);

		static Bool
		getExtendsFromClass(ast::AstNode* const decl, ast::AstNode** outDecl);
	};
} }