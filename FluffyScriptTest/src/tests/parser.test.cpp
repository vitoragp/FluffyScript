#include <memory>
#include "gtest/gtest.h"

#include "fl_parser.h"
#include "fl_exceptions.h"

namespace fluffy { namespace testing {
	using parser::Parser;
	using lexer::Lexer;

	/**
	 * ParserTest
	 */

	struct ParserTest : public ::testing::Test
	{
		std::unique_ptr<Lexer> lexer;
		std::unique_ptr<Parser> parser;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			parser = std::make_unique<Parser>(
				new Lexer(
					new DirectBuffer()
				)
			);
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserTest, TestParseEmpty)
	{
		parser->loadSource("");
		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);
	}

	TEST_F(ParserTest, TestParseConstants)
	{
		parser->loadSource("10i8 10u8 10i16 10u16 10i32 10u32 10i64 10u64 10.0f 10.0 10 \"10\" '1' true false");

		// Le o primeiro token.
		parser->nextToken();

		EXPECT_EQ(parser->expectConstantI8(), 10);
		EXPECT_EQ(parser->expectConstantU8(), 10);

		EXPECT_EQ(parser->expectConstantI16(), 10);
		EXPECT_EQ(parser->expectConstantU16(), 10);

		EXPECT_EQ(parser->expectConstantI32(), 10);
		EXPECT_EQ(parser->expectConstantU32(), 10);

		EXPECT_EQ(parser->expectConstantI64(), 10);
		EXPECT_EQ(parser->expectConstantU64(), 10);

		EXPECT_EQ(parser->expectConstantFp32(), 10.0f);
		EXPECT_EQ(parser->expectConstantFp64(), 10.0);

		EXPECT_EQ(parser->expectConstantI32(), 10);

		EXPECT_EQ(parser->expectConstantString(), "10");

		EXPECT_EQ(parser->expectConstantChar(), '1');

		EXPECT_EQ(parser->expectConstantBool(), true);
		EXPECT_EQ(parser->expectConstantBool(), false);
	}

	TEST_F(ParserTest, TestParseIncludeOnlyOneIdentifier)
	{
		parser->loadSource("include { print } from std;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->codeUnitList.size(), 0);

		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0], "print");

		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeTwoIdentifier)
	{
		parser->loadSource("include { print, scan } from std;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->codeUnitList.size(), 0);

		
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 2);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0], "print");
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[1], "scan");

		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseIncludeAll)
	{
		parser->loadSource("include { * } from std::math;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->codeUnitList.size(), 0);

		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 0);

		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->identifier, "std");
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->startFromRoot, false);
	
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier->identifier, "math");
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier->startFromRoot, false);
		EXPECT_EQ(treeAst->includeDeclList[0]->fromNamespace->tailIdentifier->tailIdentifier, nullptr);
	}

	TEST_F(ParserTest, TestParseMultiplesIncludeTwoIdentifierWithGeneric)
	{
		parser->loadSource("include { print, scan } from std;\ninclude { Window } from std::UI;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);
		EXPECT_EQ(treeAst->codeUnitList.size(), 0);

		
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
		parser->loadSource("include { * } from std;\ninclude { * } from UI;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);
		EXPECT_EQ(treeAst->codeUnitList.size(), 0);

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
		parser->loadSource("include { print2, scan } from std;\ninclude { * } from UI;");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 2);
		EXPECT_EQ(treeAst->codeUnitList.size(), 0);

		
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
		parser->loadSource("namespace application { }");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->name, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceEmpty)
	{
		parser->loadSource("namespace application { }\nnamespace testing {}");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->name, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->name, "testing");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->name, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList[0]->name, "detail");
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceOnlyOneChildNamespace)
	{
		parser->loadSource("namespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->name, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList[0]->name, "detail");

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->name, "testing");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMixedIncludeAndNamespace)
	{
		parser->loadSource("include { print } from std;\nnamespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 2);

		// Include
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0], "print");

		// Namespace
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->name, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList[0]->name, "detail");

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->name, "testing");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}
} }