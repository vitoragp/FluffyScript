#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_compiler.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	using Compiler = fluffy::Compiler;

	/**
	 * CompilerTest
	 */

	struct CompilerTest : public ::testing::Test
	{
		std::unique_ptr<Compiler> compiler;

		// Antes de cada test
		virtual void SetUp() override {
			compiler = std::make_unique<Compiler>();
		}
	};

	/**
	 * Testing
	 */

	TEST_F(CompilerTest, TestOne)
	{
		compiler->initialize();
		compiler->buildSingleThread({
			getProjectFilePath("files\\compiler\\main.txt"),
			getProjectFilePath("files\\compiler\\subfile.txt"),
		});
	}

} }