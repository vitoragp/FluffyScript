#include <memory>
#include "gtest/gtest.h"

#include "parser\fl_parser.h"
#include "lexer\fl_lexer.h"
#include "fl_buffer.h"
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

		CompilationContext_t ctx;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			parser = std::make_unique<Parser>(
				new Lexer(
					new DirectBuffer()
				)
			);

			ctx.parser = parser.get();
		}
	};

	/**
	 * Tests
	 */

	TEST_F(ParserTest, TestParseEmpty)
	{
		ctx.parser->loadSource("");
		auto treeAst = ctx.parser->parse();
		EXPECT_FALSE(treeAst == nullptr);
	}

	TEST_F(ParserTest, TestParseConstants)
	{
		ctx.parser->loadSource("10i8 10u8 10i16 10u16 10i32 10u32 10i64 10u64 10.0f 10.0 10 \"10\" '1' true false");

		// Le o primeiro token.
		ctx.parser->nextToken();

		EXPECT_EQ(ctx.parser->expectConstantI8(), 10);
		EXPECT_EQ(ctx.parser->expectConstantU8(), 10);

		EXPECT_EQ(ctx.parser->expectConstantI16(), 10);
		EXPECT_EQ(ctx.parser->expectConstantU16(), 10);

		EXPECT_EQ(ctx.parser->expectConstantI32(), 10);
		EXPECT_EQ(ctx.parser->expectConstantU32(), 10);

		EXPECT_EQ(ctx.parser->expectConstantI64(), 10);
		EXPECT_EQ(ctx.parser->expectConstantU64(), 10);

		EXPECT_EQ(ctx.parser->expectConstantFp32(), 10.0f);
		EXPECT_EQ(ctx.parser->expectConstantFp64(), 10.0);

		EXPECT_EQ(ctx.parser->expectConstantI32(), 10);

		EXPECT_EQ(ctx.parser->expectConstantString(), "10");

		EXPECT_EQ(ctx.parser->expectConstantChar(), '1');

		EXPECT_EQ(ctx.parser->expectConstantBool(), true);
		EXPECT_EQ(ctx.parser->expectConstantBool(), false);
	}

	TEST_F(ParserTest, TestParseIncludeOnlyOneIdentifier)
	{
		ctx.parser->loadSource("include { print } from std;");

		auto treeAst = ctx.parser->parse();
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
		ctx.parser->loadSource("include { print, scan } from std;");

		auto treeAst = ctx.parser->parse();
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
		ctx.parser->loadSource("include { * } from std::math;");

		auto treeAst = ctx.parser->parse();
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
		ctx.parser->loadSource("include { print, scan } from std;\ninclude { Window } from std::UI;");

		auto treeAst = ctx.parser->parse();
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
		ctx.parser->loadSource("include { * } from std;\ninclude { * } from UI;");

		auto treeAst = ctx.parser->parse();
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
		ctx.parser->loadSource("include { print2, scan } from std;\ninclude { * } from UI;");

		auto treeAst = ctx.parser->parse();
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
		ctx.parser->loadSource("namespace application { }");

		auto treeAst = ctx.parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceEmpty)
	{
		ctx.parser->loadSource("namespace application { }\nnamespace testing {}");

		auto treeAst = ctx.parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->generalDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseOnlyOneNamespaceOnlyOneChildNamespace)
	{
		ctx.parser->loadSource("namespace application { namespace detail { } }");

		auto treeAst = ctx.parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");
	}

	TEST_F(ParserTest, TestParseMultiplesNamespaceOnlyOneChildNamespace)
	{
		ctx.parser->loadSource("namespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = ctx.parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 2);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserTest, TestParseMixedIncludeAndNamespace)
	{
		ctx.parser->loadSource("include { print } from std;\nnamespace application { namespace detail { } }\nnamespace testing {}");

		auto treeAst = ctx.parser->parse();
		EXPECT_FALSE(treeAst == nullptr);

		EXPECT_EQ(treeAst->includeDeclList.size(), 1);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList.size(), 2);

		// Include
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList.size(), 1);
		EXPECT_EQ(treeAst->includeDeclList[0]->includedItemList[0], "print");

		// Namespace
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->identifier, "application");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList.size(), 1);

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[0]->namespaceDeclList[0]->identifier, "detail");

		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->identifier, "testing");
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->generalDeclList.size(), 0);
		EXPECT_EQ(treeAst->codeUnitList[0]->namespaceDeclList[1]->namespaceDeclList.size(), 0);
	}
} }