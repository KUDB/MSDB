#include <pch_test.h>
#include <util/coordinate.h>

namespace msdb
{
namespace test
{
TEST(coor, coordinateBasic)
{
	//////////////////////////////
	// Initalized into 0
	// coordinates(const size_type dSize);
	{
		core::coordinates myCoor(2);
		EXPECT_EQ(myCoor.size(), 2);

		EXPECT_EQ(myCoor[0], 0);
		EXPECT_EQ(myCoor[1], 0);
	}

	//////////////////////////////
	// Constructor 02
	// coordinates(const size_type dSize, dim_const_pointer coor);
	{
		int64_t c[3] = { 1, 2, 3 };
		core::coordinates myCoor(3, c);
		EXPECT_EQ(myCoor[0], 1);
		EXPECT_EQ(myCoor[1], 2);
		EXPECT_EQ(myCoor[2], 3);

		EXPECT_EQ(myCoor.at(0), 1);
		EXPECT_EQ(myCoor.at(1), 2);
		EXPECT_EQ(myCoor.at(2), 3);
	}

	//////////////////////////////
	// Constructor 03
	// coordinates(const size_type dSize, const dim_type coor);
	{
		int64_t value = 1;
		size_t size = 4;
		core::coordinates myCoor(size, value);
		EXPECT_EQ(myCoor.size(), size);

		EXPECT_EQ(myCoor[0], value);
		EXPECT_EQ(myCoor[1], value);
		EXPECT_EQ(myCoor[2], value);
	}

	//////////////////////////////
	// Constructor 04
	// coordinates(const std::vector<dim_type>& coorVec);
	{
		std::vector<int64_t> value = {1, 2, 3};
		core::coordinates myCoor(value);
		EXPECT_EQ(myCoor.size(), value.size());

		EXPECT_EQ(myCoor[0], value[0]);
		EXPECT_EQ(myCoor[1], value[1]);
		EXPECT_EQ(myCoor[2], value[2]);
	}

	//////////////////////////////
	// Constructor 05
	// coordinates(std::initializer_list<dim_type> lst);
	{
		core::coordinates myCoor({1, 2, 3});

		EXPECT_EQ(myCoor.size(), 3);

		EXPECT_EQ(myCoor[0], 1);
		EXPECT_EQ(myCoor[1], 2);
		EXPECT_EQ(myCoor[2], 3);
	}
}

TEST(multiDimIterator, move_in_4d_space)
{
	core::multiDimIterator it({8, 3, 16, 32});
	core::coordinates target({ 7, 2, 12, 0 });

	it.moveTo(target);
}
}		// test
}		// msdb