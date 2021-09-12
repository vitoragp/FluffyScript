#pragma once
#include "fl_defs.h"

namespace fluffy { namespace ast {
	class IncludeDecl;
} }

namespace fluffy { namespace utils {
	/**
	 * IncludeUtils
	 */

	class IncludeUtils
	{
	public:
		static Bool
		isIncludeFromSystem(ast::IncludeDecl* const include);
	};
} }
