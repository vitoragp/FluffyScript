#pragma once
#include "ast\fl_ast.h"
#include "attributes\fl_attribute.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace attributes {
	/**
	 * Reference
	 */

	class Reference : public AttributeTemplate<AttributeType_e::Reference>
	{
	public:
		Reference(ast::AstNode* const referencedScope, ast::AstNode* const referencedNode);
		~Reference();

		ast::AstNode* const
		getScope();

		ast::AstNode* const
		get();

		template <typename T>
		T* const
		to();

	private:
		ast::AstNode* const
		mReferencedScope;

		ast::AstNode* const
		mReferencedNode;
	};

	/**
	 * Reference Impl
	 */

	template <typename T>
	T* const
	Reference::to()
	{
		return ast::safe_cast<T>(mReferencedNode);
	}
} }