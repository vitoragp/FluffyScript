#include <memory>
#include "test.h"
#include "gtest/gtest.h"

#include "fl_buffer.h"
#include "parser\fl_parser.h"
#include "transformation\fl_transformation_resolve_include.h"
#include "transformation\fl_transformation_resolve_types.h"
#include "validation\fl_validation_duplicated_nodes.h"
#include "validation\fl_validation_general_rules.h"
#include "attributes\fl_reference.h"
#include "fl_compiler.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	/**
		* ValidationGeneralRulesTest
		*/

	struct ValidationGeneralRulesTest : public ::testing::Test
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

	TEST_F(ValidationGeneralRulesTest, TestClassExtendsOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
				"class Foo extends i32 {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Foo' class extends must be a class element at: line 2, column 19");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassExtendsTwo)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface Loo {}\n"
					"class Foo extends Loo {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Foo' class must be extended by another class: 'Loo' is not a class at: line 3, column 19");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassImplementsOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
				"class Foo implements i32 {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Foo' class implements must be a interface element at: line 2, column 22");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassImplementsTwo)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
				"class Loo {} \n"
				"class Foo implements Loo {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Foo' class must implement only interfaces: 'Loo' is not a interface at: line 3, column 22");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestTraitForOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
				"trait Loo for Foo {} \n"
				"class Foo {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Failed to implement 'Loo' trait, 'Loo' trait doesn't exists at: line 2, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestTraitForTwo)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult()
			{}

			virtual ~CheckResult()
			{}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::TraitForDecl)
				{
					auto reference = node->getAttribute<attributes::Reference>();

					ASSERT_TRUE(reference != nullptr);

					ASSERT_TRUE(reference->referencedNode != nullptr);
					ASSERT_EQ(reference->referencedNode->nodeType, AstNodeType_e::TraitDecl);
					ASSERT_EQ(reference->referencedNode->identifier, node->identifier);

					passes++;
				}
			}

			U32 passes = false;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());
		compiler->applyValidation(new validations::DuplicatedNodes());
		compiler->applyValidation(new validations::GeneralRules());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"trait Loo {} \n"
				"trait Loo for Foo {} \n"
				"class Foo {} \n"
			"}"
		);

		compiler->build();

		ASSERT_EQ(checkResult->passes, 1);
	}

	TEST_F(ValidationGeneralRulesTest, TestTraitForThree)
	{
		class CheckResult : public scope::NodeProcessor
		{
		public:
			CheckResult()
			{}

			virtual ~CheckResult()
			{}

			virtual void
			onProcess(scope::ScopeManager* const scopeManager, ast::AstNode* const node)
			{
				if (node->nodeType == AstNodeType_e::TraitForDecl)
				{
					auto reference = node->getAttribute<attributes::Reference>();

					ASSERT_TRUE(reference != nullptr);

					ASSERT_TRUE(reference->referencedNode != nullptr);
					ASSERT_EQ(reference->referencedNode->nodeType, AstNodeType_e::TraitDecl);
					ASSERT_EQ(reference->referencedNode->identifier, node->identifier);
					ASSERT_EQ(reference->referencedScope->nodeType, AstNodeType_e::NamespaceDecl);
					ASSERT_EQ(reference->referencedScope->identifier, "foo");

					passes++;
				}
			}

			U32 passes = false;
		};

		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());
		compiler->applyValidation(new validations::DuplicatedNodes());
		compiler->applyValidation(new validations::GeneralRules());

		auto checkResult = new CheckResult();
		compiler->applyValidation(checkResult);

		compiler->addBlockToBuild("source1",
			"include {foo::Loo} in \"source2\"; \n"
			"namespace app { \n"
				"trait Loo for Foo {} \n"
				"class Foo {} \n"
			"}"
		);

		compiler->addBlockToBuild("source2",
			"namespace foo { \n"
				"export trait Loo {} \n"
			"}"
		);

		compiler->build();

		ASSERT_EQ(checkResult->passes, 1);
	}

	TEST_F(ValidationGeneralRulesTest, TestTraitForFour)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"trait Loo for i32 {  } \n"
					"trait Loo { fn test(); } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Trait definition 'Loo' trait, must implement all functions: 'test' was not implemented at: line 3, column 13");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestTraitForFive)
	{
		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());
		compiler->applyValidation(new validations::DuplicatedNodes());
		compiler->applyValidation(new validations::GeneralRules());

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
			"trait Loo for i32 { fn test() {} } \n"
			"trait Loo { fn test(); } \n"
			"}"
		);

		compiler->build();
	}

	TEST_F(ValidationGeneralRulesTest, TestTraitForSix)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"trait Loo for i32 { fn test() {} } \n"
					"trait Loo { static fn test(); } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Trait definition 'Loo' trait, must implement all functions: 'test' was not implemented at: line 3, column 20");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestTraitForSeven)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"trait Loo for i32 { static fn test() {} } \n"
					"trait Loo { fn test(); } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Trait definition 'Loo' trait, must implement all functions: 'test' was not implemented at: line 3, column 13");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestGenericOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo<T, W> {} \n"
					"let p: Foo<i32>; \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Generic 'Foo' requires 2 type arguments at: line 3, column 8");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestGenericWhereClauseOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo<T where T is i32[] | bool> {} \n"
					"let p: Foo<string>; \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: Type generic item doesn't match where clause at: line 3, column 8");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestGenericWhereClauseTwo)
	{
		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());
		compiler->applyValidation(new validations::DuplicatedNodes());
		compiler->applyValidation(new validations::GeneralRules());

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"trait Lone {} \n"
				"trait Lone for Xoo {} \n"
				"class Loo {} \n"
				"class Xoo extends Loo {} \n"
				"class Foo<T where T is Lone> {} \n"
				"let p: Foo<Xoo>; \n"
			"}"
		);

		compiler->build();
	}

	TEST_F(ValidationGeneralRulesTest, TestGenericWhereClauseThree)
	{
		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());
		compiler->applyValidation(new validations::DuplicatedNodes());
		compiler->applyValidation(new validations::GeneralRules());

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"trait Lone {} \n"
				"trait Lone for Xoo {} \n"
				"class Loo {} \n"
				"class Xoo extends Loo {} \n"
				"class Lim extends Xoo {} \n"
				"class Foo<T where T is Lone> {} \n"
				"let p: Foo<Lim>; \n"
			"}"
		);

		compiler->build();
	}

	TEST_F(ValidationGeneralRulesTest, TestGenericWhereClauseFour)
	{
		compiler->initialize();
		compiler->applyTransformation(new transformations::ResolveInclude());
		compiler->applyTransformation(new transformations::ResolveTypes());
		compiler->applyValidation(new validations::DuplicatedNodes());
		compiler->applyValidation(new validations::GeneralRules());

		compiler->addBlockToBuild("source1",
			"namespace app { \n"
				"interface A {} \n"
				"class Loo implements A {} \n"
				"class Xoo extends Loo {} \n"
				"class Lim extends Xoo {} \n"
				"class Foo<T where T is A> {} \n"
				"let p: Foo<Lim>; \n"
			"}"
		);

		compiler->build();
	}

	TEST_F(ValidationGeneralRulesTest, TestCircularDependency)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"class Foo extends Loo {} \n"
					"class Loo extends Zoo {} \n"
					"class Zoo extends Xoo {} \n"
					"class Xoo extends Foo {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Foo' class has circular extends dependency at: line 2, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsOne)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface Foo { fn zoo(); } \n"
					"class Xoo implements Foo {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Xoo' class must implement 'zoo', it may have been declared in an interface or it may be abstract at: line 3, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsTwo)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface Foo { fn zoo(); } \n"
					"class Xoo implements Foo { private fn zoo() {} } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'zoo' function implemented for 'Foo' interface, must be public at: line 3, column 36");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsThree)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface Foo { fn zoo(); fn xoo(); } \n"
					"abstract class Xoo implements Foo { private fn xoo() {} } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'xoo' function implemented for 'Foo' interface, must be public at: line 3, column 45");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsFour)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface Foo { fn zoo(); } \n"
					"abstract class Xoo implements Foo {} \n"
					"class Boo extends Xoo {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Boo' class must implement 'zoo', it may have been declared in an interface or it may be abstract at: line 4, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsFive)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface FooOne { fn xoo(); }"
					"interface FooTwo { fn zoo(); } \n"
					"abstract class Xoo implements FooTwo {} \n"
					"class Boo extends Xoo implements FooOne {} \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Boo' class must implement 'zoo', it may have been declared in an interface or it may be abstract at: line 4, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsSix)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface FooOne { fn xoo(); }"
					"interface FooTwo { fn zoo(); } \n"
					"abstract class Xoo implements FooTwo {} \n"
					"class Boo extends Xoo implements FooOne { fn zoo() {} public fn xoo() {} } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'zoo' function implemented from interface, must be public at: line 4, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsSeven)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface FooOne { fn xoo(); } \n"
					"interface FooTwo { fn zoo(); } \n"
					"abstract class Xoo implements FooTwo { abstract fn test(); } \n"
					"class Boo extends Xoo implements FooOne { public fn zoo() {} public fn xoo() {} } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'test' function, must be public at: line 4, column 40");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsEight)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface FooOne { fn xoo(); } \n"
					"interface FooTwo { fn zoo(); } \n"
					"abstract class Xoo implements FooTwo { public abstract fn test(); } \n"
					"class Boo extends Xoo implements FooOne { public fn zoo() {} public fn xoo() {} } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The 'Boo' class must implement 'test', it may have been declared in an interface or it may be abstract at: line 5, column 1");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}

	TEST_F(ValidationGeneralRulesTest, TestClassRequiredInterfaceFunctionsNine)
	{
		try
		{
			compiler->initialize();
			compiler->applyTransformation(new transformations::ResolveInclude());
			compiler->applyTransformation(new transformations::ResolveTypes());
			compiler->applyValidation(new validations::DuplicatedNodes());
			compiler->applyValidation(new validations::GeneralRules());

			compiler->addBlockToBuild("source1",
				"namespace app { \n"
					"interface FooOne { } \n"
					"class Boo implements FooOne { abstract fn foo(); } \n"
				"}"
			);

			compiler->build();

			FAIL() << "Unexpected result";
		}
		catch (exceptions::custom_exception& e)
		{
			ASSERT_STREQ(e.what(), "source1 error: The class 'Boo' declare 'foo' function as abstract, only abstract class can have abstract functions at: line 3, column 31");
		}
		catch (std::exception& e)
		{
			FAIL() << e.what();
		}
	}
} }