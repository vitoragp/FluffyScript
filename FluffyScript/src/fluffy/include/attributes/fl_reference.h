#pragma once
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
		referencedScope;

		ast::AstNode* const
		referencedNode;
	};
} }