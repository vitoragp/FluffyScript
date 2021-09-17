#include "attributes\fl_reference.h"
namespace fluffy { namespace attributes {
	/**
	 * Reference
	 */

	Reference::Reference(ast::AstNode* const referencedScope, ast::AstNode* const referencedNode)
		: mReferencedScope(referencedScope)
		, mReferencedNode(referencedNode)
	{}

	Reference::~Reference()
	{}

	ast::AstNode* const
	Reference::getScope()
	{
		return mReferencedScope;
	}

	ast::AstNode* const
	Reference::get()
	{
		return mReferencedNode;
	}
} }