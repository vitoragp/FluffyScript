#include <memory>
#include "gtest\gtest.h"

#include "parser\fl_parser.h"
#include "ast\fl_ast_decl.h"
#include "fl_buffer.h"

namespace fluffy { namespace testing {
	using parser::Parser;

	/**
	 * ParserNamespaceTest
	 */

	struct ParserNamespaceTest : public ::testing::Test
	{
		std::unique_ptr<Parser> parser;
		fluffy::parser::ParserContext_s ctx{ true, false, false };

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

	TEST_F(ParserNamespaceTest, TestIncludeDecl_OneNS)
	{
		parser->loadSource("namespace std {}");

		auto namespaceDecl = parser->parseNamespace(ctx);

		ASSERT_TRUE(namespaceDecl != nullptr);

		EXPECT_EQ(namespaceDecl->identifier, "std");
		EXPECT_EQ(namespaceDecl->generalDeclList.size(), 0);
		EXPECT_EQ(namespaceDecl->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserNamespaceTest, TestIncludeDecl_NestedNS)
	{
		parser->loadSource("namespace std { namespace Vec {} }");

		auto namespaceDecl = parser->parseNamespace(ctx);

		ASSERT_TRUE(namespaceDecl != nullptr);

		EXPECT_EQ(namespaceDecl->identifier, "std");
		EXPECT_EQ(namespaceDecl->generalDeclList.size(), 0);
		EXPECT_EQ(namespaceDecl->namespaceDeclList.size(), 1);

		EXPECT_EQ(namespaceDecl->namespaceDeclList[0]->identifier, "Vec");
		EXPECT_EQ(namespaceDecl->namespaceDeclList[0]->generalDeclList.size(), 0);
		EXPECT_EQ(namespaceDecl->namespaceDeclList[0]->namespaceDeclList.size(), 0);
	}

	TEST_F(ParserNamespaceTest, TestIncludeDecl_OneNS_withGeneralStmt)
	{
		parser->loadSource("namespace std { struct IO {} }");

		auto namespaceDecl = parser->parseNamespace(ctx);

		ASSERT_TRUE(namespaceDecl != nullptr);

		EXPECT_EQ(namespaceDecl->identifier, "std");
		EXPECT_EQ(namespaceDecl->generalDeclList.size(), 1);
		EXPECT_EQ(namespaceDecl->namespaceDeclList.size(), 0);

		EXPECT_EQ(namespaceDecl->generalDeclList[0]->identifier, "IO");
		EXPECT_EQ(namespaceDecl->generalDeclList[0]->type, GeneralDeclType_e::StructDecl);
	}

	TEST_F(ParserNamespaceTest, TestIncludeDecl_NestedNS_WithStmt)
	{
		parser->loadSource("namespace std { struct IO {} namespace Vec { struct Vec3 {} } }");

		auto namespaceDecl = parser->parseNamespace(ctx);

		ASSERT_TRUE(namespaceDecl != nullptr);

		EXPECT_EQ(namespaceDecl->identifier, "std");
		EXPECT_EQ(namespaceDecl->generalDeclList.size(), 1);
		EXPECT_EQ(namespaceDecl->namespaceDeclList.size(), 1);

		EXPECT_EQ(namespaceDecl->generalDeclList[0]->identifier, "IO");

		EXPECT_EQ(namespaceDecl->namespaceDeclList[0]->identifier, "Vec");
		EXPECT_EQ(namespaceDecl->namespaceDeclList[0]->generalDeclList.size(), 1);
		EXPECT_EQ(namespaceDecl->namespaceDeclList[0]->namespaceDeclList.size(), 0);

		EXPECT_EQ(namespaceDecl->namespaceDeclList[0]->generalDeclList[0]->identifier, "Vec3");
	}
} }