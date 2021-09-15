#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "ast\fl_ast.h"
#include "ast\fl_ast_decl.h"
#include "ast\fl_ast_stmt.h"
#include "ast\fl_ast_expr.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "transformation\fl_transformation_resolve_types.h"
#include "validation\fl_validation_duplicated_nodes.h"
#include "validation\fl_validation_general_rules.h"
#include "fl_exceptions.h"
#include "fl_compiler.h"

namespace fluffy { namespace testing {

	/**
	 * CompilerTest
	 */

	struct CompilerTest : public ::testing::Test
	{
		std::unique_ptr<fluffy::Compiler> compiler;

		// Antes de cada test
		virtual void SetUp() override {
			compiler = std::make_unique<fluffy::Compiler>();
		}
	};

	/**
	 * Testing
	 */

	TEST_F(CompilerTest, TestOne)
	{
		String path = _BASE_PATH;
		compiler->initialize(path + "\\files\\compiler\\");
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());
		compiler->applyValidation(new validations::DuplicatedNodes());
		compiler->applyValidation(new validations::GeneralRules());
		compiler->build("main.txt");
	}

} }