#include "fl_ast.h"
namespace fluffy { namespace ast {
	/**
	 * Program
	 */

	Program::Program()
	{}

	Program::~Program()
	{}

	/**
	 * Include
	 */

	Include::Include()
		: allFlag(false)
	{}

	Include::~Include()
	{}
} }