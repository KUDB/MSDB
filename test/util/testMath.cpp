#include "../pch.h"
#include <util/math.h>

using namespace msdb::core;

TEST(util_math, msb_char)
{
	// 0111 1000
	EXPECT_EQ(msb<char>(120, 1), 7);
	EXPECT_EQ(msb<char>(120, 2), 6);
	EXPECT_EQ(msb<char>(120, 3), 5);
	EXPECT_EQ(msb<char>(120, 4), 4);
	EXPECT_EQ(msb<char>(120, 5), 0);

	// 0101 0101
	EXPECT_EQ(msb<char>(85, 1), 7);
	EXPECT_EQ(msb<char>(85, 2), 5);
	EXPECT_EQ(msb<char>(85, 3), 3);
	EXPECT_EQ(msb<char>(85, 4), 1);
	EXPECT_EQ(msb<char>(85, 5), 0);

	// 0011 0100
	EXPECT_EQ(msb<char>(52, 1), 6);
	EXPECT_EQ(msb<char>(52, 2), 5);
	EXPECT_EQ(msb<char>(52, 3), 3);
	EXPECT_EQ(msb<char>(52, 4), 0);

	// 1000 1000
	EXPECT_EQ(msb<char>(-120, 1), 8);
	EXPECT_EQ(msb<char>(-120, 2), 4);
	EXPECT_EQ(msb<char>(-120, 3), 0);
	EXPECT_EQ(msb<char>(-120, 4), 0);
	EXPECT_EQ(msb<char>(-120, 5), 0);

	// 1010 1011
	EXPECT_EQ(msb<char>(-85, 1), 8);
	EXPECT_EQ(msb<char>(-85, 2), 6);
	EXPECT_EQ(msb<char>(-85, 3), 4);
	EXPECT_EQ(msb<char>(-85, 4), 2);
	EXPECT_EQ(msb<char>(-85, 5), 1);
	EXPECT_EQ(msb<char>(-85, 6), 0);

	// 1100 1100
	EXPECT_EQ(msb<char>(-52, 1), 8);
	EXPECT_EQ(msb<char>(-52, 2), 7);
	EXPECT_EQ(msb<char>(-52, 3), 4);
	EXPECT_EQ(msb<char>(-52, 4), 3);
	EXPECT_EQ(msb<char>(-52, 5), 0);

	// 1000 0001
	EXPECT_EQ(msb<char>(-127, 1), 8);
	EXPECT_EQ(msb<char>(-127, 2), 1);
	EXPECT_EQ(msb<char>(-127, 3), 0);

	// Special case: Min Limit
	// 1000 0000
	EXPECT_EQ(msb<char>(-128, 1), 7);
	EXPECT_EQ(msb<char>(-128, 2), 0);
	EXPECT_EQ(msb<char>(-128, 2), 0);
}

TEST(util_math, getMinBoundary)
{
	// 28 (0001 1100)
	EXPECT_EQ(getMinBoundary<char>(28, 1, 7), 64);
	EXPECT_EQ(getMinBoundary<char>(28, 1, 6), 32);
	EXPECT_EQ(getMinBoundary<char>(28, 1, 5), 28);

	EXPECT_EQ(getMinBoundary<char>(28, 2, 4), 28);

	EXPECT_EQ(getMinBoundary<char>(28, 3, 3), 28);

	// -28
	EXPECT_EQ(getMinBoundary<char>(-28, 1, -5), -28);
	EXPECT_EQ(getMinBoundary<char>(-28, 1, -4), -15);
	EXPECT_EQ(getMinBoundary<char>(-28, 1, -3), -7);
	EXPECT_EQ(getMinBoundary<char>(-28, 1, -2), -3);
	EXPECT_EQ(getMinBoundary<char>(-28, 1, -1), -1);

	EXPECT_EQ(getMinBoundary<char>(-28, 2, -4), -28);
	EXPECT_EQ(getMinBoundary<char>(-28, 2, -3), -23);
	EXPECT_EQ(getMinBoundary<char>(-28, 2, -2), -19);
	EXPECT_EQ(getMinBoundary<char>(-28, 2, -1), -17);

	EXPECT_EQ(getMinBoundary<char>(-28, 3, -3), -28);
	EXPECT_EQ(getMinBoundary<char>(-28, 3, -2), -27);
	EXPECT_EQ(getMinBoundary<char>(-28, 3, -1), -25);

	// 81 (0101 0001)
	EXPECT_EQ(getMinBoundary<char>(81, 1, 7), 81);

	EXPECT_EQ(getMinBoundary<char>(81, 2, 6), 96);
	EXPECT_EQ(getMinBoundary<char>(81, 2, 5), 81);

	EXPECT_EQ(getMinBoundary<char>(81, 3, 4), 88);
	EXPECT_EQ(getMinBoundary<char>(81, 3, 3), 84);
	EXPECT_EQ(getMinBoundary<char>(81, 3, 2), 82);
	EXPECT_EQ(getMinBoundary<char>(81, 3, 1), 81);

	// -126 (0111 1110)
	EXPECT_EQ(getMinBoundary<char>(-126, 1, -7), -126);

	// -91 (0101 1011)
	EXPECT_EQ(getMinBoundary<char>(-91, 2, -5), -91);
}

TEST(util_math, getMaxBoundary)
{
	// 81 (0101 0001)
	EXPECT_EQ(getMaxBoundary<char>(81, 1, 7), 81);
	EXPECT_EQ(getMaxBoundary<char>(81, 1, 6), 63);
	EXPECT_EQ(getMaxBoundary<char>(81, 1, 5), 31);
	EXPECT_EQ(getMaxBoundary<char>(81, 1, 4), 15);
	EXPECT_EQ(getMaxBoundary<char>(81, 1, 3), 7);
	EXPECT_EQ(getMaxBoundary<char>(81, 1, 2), 3);
	EXPECT_EQ(getMaxBoundary<char>(81, 1, 1), 1);

	// Cannot be large than prevLimit '81'
	// 81, 2, (7, 6) >= 96
	EXPECT_EQ(getMaxBoundary<char>(81, 2, 5), 81);
	EXPECT_EQ(getMaxBoundary<char>(81, 2, 4), 79);
	EXPECT_EQ(getMaxBoundary<char>(81, 2, 3), 71);
	EXPECT_EQ(getMaxBoundary<char>(81, 2, 2), 67);
	EXPECT_EQ(getMaxBoundary<char>(81, 2, 1), 65);

	// Cannot be large than prevLimit '81'
	// 81, 3, (4, 3, 2) >= 82
	EXPECT_EQ(getMaxBoundary<char>(81, 3, 1), 81);

	//////////////////////////////
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, -7), -64);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, -6), -32);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, -5), -16);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, -4), -8);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, -3), -4);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, -2), -2);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, -1), -1);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 0), 0);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 1), 1);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 2), 3);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 3), 7);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 4), 15);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 5), 31);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 6), 63);
	EXPECT_EQ((char)getMaxBoundary<char>(127, 1, 7), 127);

	//////////////////////////////
	EXPECT_EQ((char)getMaxBoundary<char>(-78, 2, -4), -78);
}

TEST(util_math, getMaxValue)
{
	EXPECT_EQ(getMaxValue<char>(), 127);
	EXPECT_EQ(getMaxValue<uint8_t>(), 255);

	EXPECT_EQ(getMaxValue<int16_t>(), 32767);
	EXPECT_EQ(getMaxValue<uint16_t>(), 65535);
}

TEST(util_math, getMinValue)
{
	EXPECT_EQ(getMinValue<char>(), -128);
	EXPECT_EQ(getMinValue<uint8_t>(), 0);

	EXPECT_EQ(getMinValue<int16_t>(), -32768);
	EXPECT_EQ(getMinValue<uint16_t>(), 0);
}

TEST(util_math, getPrefixPosForPrevLimit)
{
	EXPECT_EQ(getPrefixPosForPrevLimit(64, 2), 7);
}