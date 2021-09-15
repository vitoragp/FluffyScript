#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_buffer.h"
#include "parser\fl_parser.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "transformation\fl_transformation_resolve_types.h"
#include "validation\fl_validation_duplicated_nodes.h"
#include "validation\fl_validation_duplicated_nodes.h"
#include "fl_compiler.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {

	/**
	 * ValidationDuplicationTest
	 */

	struct ValidationDuplicationTest : public ::testing::Test
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

	TEST_F(ValidationDuplicationTest, TestDuplicatedNamespace)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app {}\n"
				"namespace app {}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'app' at: line 2, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameter)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"fn main(a: u32, a: u32) {} \n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 2, column 17");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo {} \n"
					"fn main(a: u32, { a }: Foo) {} \n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 3, column 19");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternTwo)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo {} \n"
					"fn main(a: u32, { a: a }: Foo) {} \n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 3, column 22");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternThree)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo {} \n"
					"fn main(a: u32, { a: (a) }: Foo) {} \n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 3, column 23");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternFour)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo {} \n"
					"fn main(a: u32, { a: ({ a }) }: Foo) {} \n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 3, column 25");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternFive)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"fn main(a: u32, { a: ({ b: a }) }: Foo) {} \n"
					"class Foo {}\n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 2, column 28");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternSix)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"fn main(a: u32, { a: ({ a: b }) }: Foo) { \n"
						"let a = 0; \n"
					"} \n"
					"class Foo {} \n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 3, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternSeven)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"fn main(a: u32, { a: ({ a: b }) }: Foo) { \n"
						"let { a } = foo;"
					"} \n"
					"class Foo {}\n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 3, column 7");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedParameterPatternEight)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo {}\n"
					"fn main(a: u32, { a: ({ a: b }) }: Foo) { \n"
						"let { a: { a: (a)} } = foo;"
					"} \n"
				"}"
			);
			
			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 4, column 16");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedFunctionOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo {} \n"
					"class Tonho {} \n"
					"fn main(a: u32, { a: ({ a: b }) }: Foo) {}\n"
					"fn main(a: u32, b: bool) {}\n"
					"fn main(a: u32, b: i8) {}\n"
					"fn main(a: u32, b: u8) {}\n"
					"fn main(a: u32, b: Tonho) {}\n"
					"fn main(a: u32, b: app::Foo) {}\n"
				"}"
			);
			
			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'main' at: line 9, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestLambdaOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"fn main() { \n"
						"let a = |a, a| {}; \n"
					"} \n"
				"}"
			);

			compiler->build();

			ASSERT_ANY_THROW("Unexpected result");
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Duplicated identifier 'a' at: line 3, column 13");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationDuplicationTest, TestDuplicatedFunctionTwo)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::ValidationDuplicatedNodes());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo {} \n"
					"class Tonho { \n"
						"fn main(a: u32, { a: ({ a: b }) }: Foo) {}\n"
						"fn main(a: u32, b: bool) {}\n"
						"fn main(a: u32, b: i8) {}\n"
						"fn main(a: u32, b: u8) {}\n"
						"fn main(a: u32, b: Tonho) {}\n"
						"fn Foo.main(a: u32, b: app::Foo) {}\n"
					"} \n"
				"}"
			);
			
			compiler->build();
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}
} }