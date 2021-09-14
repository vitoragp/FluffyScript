#include "attributes\fl_reference.h"
namespace fluffy { namespace attributes {
	/**
	 * Reference
	 */

	Reference::Reference(ast::AstNode* const referencedScope, ast::AstNode* const referencedNode)
		: referencedScope(referencedScope)
		, referencedNode(referencedNode)
	{}

	Reference::~Reference()
	{}
} }