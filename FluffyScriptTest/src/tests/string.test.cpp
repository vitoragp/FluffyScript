#include <memory>
#include <unordered_map>
#include "gtest/gtest.h"

#include "fl_string.h"

namespace fluffy { namespace testing {
	/**
	 * StringBufferTest
	 */

	struct StringBufferTest : public ::testing::Test
	{};

	/**
	 * Tests
	 */

	TEST_F(StringBufferTest, TestCreateString)
	{
		auto str = TString("Test");

		EXPECT_STREQ(str.str(), "Test");
		EXPECT_EQ(str.hash(), std::hash<std::string>{} ("Test"));
	}

	TEST_F(StringBufferTest, TestAssignString)
	{
		auto str = TString();

		str = "Test";

		EXPECT_STREQ(str.str(), "Test");
		EXPECT_EQ(str.hash(), std::hash<std::string>{} ("Test"));

		str = std::string("Test");

		EXPECT_STREQ(str.str(), "Test");
		EXPECT_EQ(str.hash(), std::hash<std::string>{} ("Test"));
	}

	TEST_F(StringBufferTest, TestCompareString)
	{
		auto str = TString("Test");
		auto str2 = TString("Test");
				
		EXPECT_TRUE(str == "Test");
		EXPECT_TRUE(str == std::string("Test"));
		EXPECT_TRUE(str == str2);
	}
} }