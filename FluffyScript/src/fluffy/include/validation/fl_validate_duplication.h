#pragma once
#include <unordered_map>
#include "fl_defs.h"
#include "validation\fl_validate.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace validations {
	typedef std::unordered_multimap<const I8*, ast::AstNode* const> ScopeHashMap;
	typedef std::pair<const I8*, ast::AstNode* const> ScopedPair;
	typedef std::vector<ScopeHashMap> ScopeList;

	/**
	 * DuplicatedValidation
	 */

	class DuplicatedValidation final : public Validation
	{
	public:
		DuplicatedValidation();
		~DuplicatedValidation();

	protected:
		virtual void
		beginValidation(ast::AstNode* const decl) override;

		virtual void
		endValidation(ast::AstNode* const decl) override;

	private:
		void
		checkDuplicationAndPutInScope(ast::AstNode* const node);

	private:
		ScopeList							m_scopeList;
		String								m_sourceFile;
	};
} }