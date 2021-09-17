#include "attributes\fl_scope.h"
namespace fluffy { namespace attributes {
	/**
	 * Scope
	 */

	Scope::Scope(ast::AstNode* const referencedScope)
		: mReferencedScope(referencedScope)
	{}

	Scope::~Scope()
	{}

	ast::AstNode* const
	Scope::get()
	{
		return mReferencedScope;
	}
} }