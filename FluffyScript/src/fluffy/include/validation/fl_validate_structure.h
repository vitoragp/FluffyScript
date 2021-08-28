#pragma once
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class AstNode;
} }

namespace fluffy { namespace parser {
	class Parser;
} }

namespace fluffy { namespace validations {
	/**
	 * StructureValidation
	 */

	class StructureValidation
	{
	public:
		StructureValidation(parser::Parser* const parser);
		~StructureValidation();



		void
		validate(ast::AstNode* const rootNode);

	private:
		parser::Parser* const
		m_parser;
	};
} }