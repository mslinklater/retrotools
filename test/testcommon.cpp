// [CopyrightNotice]
// Copyright (c) 2019-2022, Martin Linklater
// All rights reserved.
//
// See file 'LICENSE' for license details

#include <gtest/gtest.h>

TEST(SimpleTests, SimpleTest_False)
{
	EXPECT_FALSE(false);
}
TEST(SimpleTests, SimpleTest_True)
{
	EXPECT_TRUE(true);
}
TEST(SimpleTests, SimpleTest_Equals)
{
	EXPECT_EQ(1, 1);
}
