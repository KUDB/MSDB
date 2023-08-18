#include <pch_test.h>
#include <util/enumType.h>

using namespace msdb::core;

ENUM_CLASS_MACRO(enTest, TEST_A, TEST_B);

TEST(enum_test, to_string)
{
	EXPECT_EQ(enTestToString(enTest::TEST_A), std::string("TEST_A"));

	EXPECT_EQ(ENUM_TO_STRING(enTest, enTest::TEST_A), std::string("TEST_A"));
}