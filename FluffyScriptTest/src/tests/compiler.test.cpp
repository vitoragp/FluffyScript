#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_compiler.h"
#include "fl_exceptions.h"

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
		compiler->build("main.txt");
	}

} }