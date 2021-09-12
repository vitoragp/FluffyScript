#include "ast\fl_ast_decl.h"
#include "utils\fl_include_utils.h"

namespace fluffy { namespace utils {
	/**
	 * IncludeUtils
	 */

	Bool
	IncludeUtils::isIncludeFromSystem(ast::IncludeDecl* const include)
	{
		return include->inFile[0] == '/';
	}
} }