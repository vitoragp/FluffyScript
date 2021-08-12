/*
 * buffer.test.cpp
 *
 *  Created on: 12 de ago. de 2021
 *      Author: NPShinigami
 */

#include <memory>
#include <gtest/gtest.h>

#include "fl_buffer.h"
#include "fl_exceptions.h"


namespace fluffy { namespace testing {
	/**
	 * DirectBuffer
	 */

	struct DirectBufferTest : public ::testing::Test
	{
		std::unique_ptr<BufferBase> buffer;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			buffer = std::make_unique<DirectBuffer>();
		}
	};

	/**
	 * Testing
	 */

	TEST_F(DirectBufferTest, TestLoadAndReadData)
	{
		String src = "test";

		buffer->load(src.c_str(), src.length());

		String str;
		while (true)
		{
			const I8 ch = buffer->readByte();

			if (ch == '\0') {
				break;
			}
			str.push_back(ch);

			buffer->nextByte();
		}

		EXPECT_EQ(src, str);
	}

	TEST_F(DirectBufferTest, TestLoadAndReadDataFromFile)
	{
		String src = "u64 id\r\nstring name";

		buffer->loadFromFile(".\\files\\source_1.txt");

		String str;
		while (true)
		{
			const I8 ch = buffer->readByte();

			if (ch == '\0') {
				break;
			}
			str.push_back(ch);

			buffer->nextByte();
		}

		EXPECT_EQ(src, str);
	}
} }

namespace fluffy { namespace testing {
	/**
	 * LazyBuffer
	 */

	struct LazyBufferTest : public ::testing::Test
	{
		std::unique_ptr<BufferBase> buffer;

		// Sets up the test fixture.
		virtual void SetUp()
		{
			buffer = std::make_unique<LazyBuffer>(3);
		}
	};

	/**
	 * Testing
	 */

	TEST_F(LazyBufferTest, TestLoadAndReadData)
	{
		String src = "test";

		buffer->load(src.c_str(), src.length());

		String str;
		while (true)
		{
			const I8 ch = buffer->readByte();

			if (ch == '\0') {
				break;
			}
			str.push_back(ch);

			buffer->nextByte();
		}

		EXPECT_EQ(src, str);
	}

	TEST_F(LazyBufferTest, TestLoadAndReadDataFromFile)
	{
		String src = "u64 id\r\nstring name";

		buffer->loadFromFile(".\\files\\source_1.txt");

		String str;
		while (true)
		{
			const I8 ch = buffer->readByte();

			if (ch == '\0') {
				break;
			}
			str.push_back(ch);

			buffer->nextByte();
		}

		EXPECT_EQ(src, str);
	}
} }
