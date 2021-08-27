#include <memory>
#include "gtest/gtest.h"

#include "parser\fl_parser.h"
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
		fluffy::parser::ParserContext_s ctx { true, false, false };
		parser->loadSource("");
		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeOnlyOneIdentifier)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("include { print } from std;");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includedItemList[0], "print");

		EXPECT_EQ(treeAst->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeTwoIdentifier)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("include { print, scan } from std;");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);
		
		EXPECT_EQ(treeAst->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includedItemList[0], "print");
		EXPECT_EQ(treeAst->includedItemList[1], "scan");

		EXPECT_EQ(treeAst->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeAll)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("include { * } from std::math;");

		auto treeAst = parser->parseInclude(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->fromNamespace->startFromRoot, false);
	
		EXPECT_EQ(treeAst->fromNamespace->tailIdentifier->identifier, "math");
		EXPECT_EQ(treeAst->fromNamespace->tailIdentifier->startFromRoot, false);
		EXPECT_EQ(treeAst->fromNamespace->tailIdentifier->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeTwoIdentifierWithGeneric)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("include { print, scan } from std;\ninclude { Window } from std::UI;");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);
		
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0], "print");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1], "scan");

		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier, nullptr);

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList[0], "Window");

		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->startFromRoot, false);

		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->tailIdentifier->identifier, "UI");
		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->tailIdentifier->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->tailIdentifier->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeAll)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("include { * } from std;\ninclude { * } from UI;");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);

		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 0);
				
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier, nullptr);

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->identifier, "UI");
		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseMultiplesInclude)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("include { print2, scan } from std;\ninclude { * } from UI;");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);

		
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0], "print2");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1], "scan");

		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier, nullptr);

		EXPECT_EQ(treeAst->includeDeclList[1]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->identifier, "UI");
		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[1]->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceEmpty)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("namespace application { }");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceEmpty)
	{
		fluffy::parser::ParserContext_s ctx{ true, false, false };

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
		fluffy::parser::ParserContext_s ctx{ true, false, false };

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
		fluffy::parser::ParserContext_s ctx{ true, false, false };

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
		fluffy::parser::ParserContext_s ctx{ true, false, false };

		parser->loadSource("include { print } from std;\nnamespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parseCodeUnit(ctx);
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->namespaceDeclList.size(), 2);

		// Include
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0], "print");

		// Namespace
		EXPECT_EQ(treeAst->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");

		EXPECT_EQ(treeAst->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}
} }