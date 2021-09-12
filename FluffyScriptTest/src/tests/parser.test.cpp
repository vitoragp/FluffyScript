#include <memory>
#include "gtest/gtest.h"
#include "test.h"

#include "parser\fl_parser.h"
#include "scope\fl_scope_manager.h"
#include "utils\fl_info_util.h"
#include "fl_buffer.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * ParserTest
	 */

	struct ParserTest : public ::testing::Test
	{
		std::unique_ptr<Parser> parser;
		fluffy::parser::ParserContext_s ctx{ false };

		// Sets up the test fixture.
		virtual void SetUp()
		{
			parser = std::make_unique<Parser>(
				new DirectBuffer()
			);
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserTest, TestParseEmpty)
	{
		parser->loadSource("");
		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeOnlyOneIdentifier)
	{
		parser->loadSource("include { io::print } in \"\\system\"; ");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includedItemList[0]->identifier, "print");
		EXPECT_EQ(treeAst->includedItemList[0]->referencedPath->identifier, "io");

		EXPECT_EQ(treeAst->inFile, "\\system");
	}

	TEST_F(ParserTest, TestParseIncludeTwoIdentifier)
	{
		parser->loadSource("include { io::print, io::scan } in \"\\system\";");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);
		
		EXPECT_EQ(treeAst->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includedItemList[0]->identifier, "print");
		EXPECT_EQ(treeAst->includedItemList[0]->referencedPath->identifier, "io");
		EXPECT_EQ(treeAst->includedItemList[1]->identifier, "scan");
		EXPECT_EQ(treeAst->includedItemList[1]->referencedPath->identifier, "io");

		EXPECT_EQ(treeAst->inFile, "\\system");
	}

	TEST_F(ParserTest, TestParseIncludeAll)
	{
		parser->loadSource("include { math::* } in \"\\system\";");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includedItemList.size(), 1);

		EXPECT_EQ(treeAst->includedItemList[0]->includeAll, true);
		EXPECT_EQ(treeAst->includedItemList[0]->referencedPath->identifier, "math");
	
		EXPECT_EQ(treeAst->inFile, "\\system");
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeTwoIdentifierWithGeneric)
	{
		parser->loadSource("include { io::print, io::scan } in \"\\system\";\ninclude { ui::Window } in \"\\system\";");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);
		
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->identifier, "print");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->referencedPath->identifier, "io");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1]->identifier, "scan");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1]->referencedPath->identifier, "io");

		EXPECT_EQ(treeAst->includeDeclList[0]->inFile, "\\system");

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0]->identifier, "Window");
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0]->referencedPath->identifier, "ui");

		EXPECT_EQ(treeAst->includeDeclList[1]->inFile, "\\system");
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeAll)
	{
		parser->loadSource("include { io::* } in \"\\system\";\ninclude { ui::* } in \"\\system\";");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);

		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->includeAll, true);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->referencedPath->identifier, "io");

		EXPECT_EQ(treeAst->includeDeclList[0]->inFile, "\\system");

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0]->includeAll, true);
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0]->referencedPath->identifier, "ui");

		EXPECT_EQ(treeAst->includeDeclList[1]->inFile, "\\system");
	}

	TEST_F(ParserTest, TestParseMultiplesInclude)
	{
		parser->loadSource("include { io::print, io::scan } in \"\\system\";\ninclude { ui::* } in \"\\system\";");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);
		
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->identifier, "print");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->referencedPath->identifier, "io");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1]->identifier, "scan");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1]->referencedPath->identifier, "io");

		EXPECT_EQ(treeAst->includeDeclList[0]->inFile, "\\system");

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0]->includeAll, true);
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0]->referencedPath->identifier, "ui");

		EXPECT_EQ(treeAst->includeDeclList[1]->inFile, "\\system");
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceEmpty)
	{
		parser->loadSource("namespace application { }");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceEmpty)
	{
		parser->loadSource("namespace application { }\nnamespace testing {}");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);

		EXPECT_EQ(treeAst->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->namespaceDeclList[1]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");

		EXPECT_EQ(treeAst->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMixedIncludeAndNamespace)
	{
		parser->loadSource("include { io::print } in \"\\std\";\nnamespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 2);

		// Include
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0]->identifier, "print");

		// Namespace
		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");

		EXPECT_EQ(treeAst->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestDestructuringGetChildren)
	{
		parser->loadSource(
			"Test;\n"
			"123;\n"
			"{ Test };\n"
			"(Test, Test, 0);\n"
			"{ Test: Test2 };\n"
			"{ Test: { Test2 } };\n"
			"{ Test: { Test: Test2 } };\n"
			"({ Test: { Test: Test1 } }, Test2, Test3)"
		);

		auto pattern1 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern2 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern3 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern4 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern5 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern6 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern7 = parser->parsePattern(ctx); parser->skipToken();
		auto pattern8 = parser->parsePattern(ctx);
	}

} }