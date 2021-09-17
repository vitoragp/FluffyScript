#pragma once
#include "attributes\fl_attribute.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace attributes {
	/**
	 * Scope
	 */

	class Scope: public AttributeTemplate<AttributeType_e::Scope>
	{
	public:
		Scope(ast::AstNode* const referencedScope);
		~Scope();

		ast::AstNode* const
		get();

	private:
		ast::AstNode* const
		mReferencedScope;

	};
} }