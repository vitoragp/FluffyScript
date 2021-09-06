#pragma once
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace validations {
	/**
	 * Validation
	 */

	class Validation
	{
	public:
		Validation();
		virtual ~Validation();

		void
		validate(ast::AstNode* const decl);

	protected:
		virtual void
		beginValidation(ast::AstNode* const decl) = 0;

		virtual void
		endValidation(ast::AstNode* const decl) = 0;

	private:
		template <typename TList> __inline void
		validate_each(TList& list)
		{
			auto itS = list.begin();
			auto itE = list.end();
			for (; itS != itE; itS++) {
				validate(itS->get());
			}
		}

		template <typename TUnique> __inline void
		validate_single(TUnique& only_one)
		{
			if (only_one != nullptr)
			{
				validate(only_one.get());
			}
		}
	};
} }
