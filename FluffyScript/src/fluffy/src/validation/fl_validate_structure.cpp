#include <typeinfo>
#include "ast\fl_ast.h"
#include "validation\fl_validate_structure.h"
namespace fluffy { namespace validations {
	/**
	 * StructureValidation
	 */

	StructureValidation::StructureValidation(parser::Parser* const parser)
		: m_parser(parser)
	{}

	StructureValidation::~StructureValidation()
	{}

	void
	StructureValidation::validate(ast::AstNode* const rootNode)
	{
		if (rootNode == nullptr)
		{
			throw std::bad_alloc();
		}
	}
} }