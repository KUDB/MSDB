#include <pch_test.h>
#include <dummy/dummy.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>

namespace msdb
{
namespace test
{
TEST(attributeDesc, copy_assign)
{
	using namespace msdb::core;

	auto myAttrDescs = attributeDescBuilder({ "ATTR_1", "ATTR_2" }, { core::eleType::CHAR, core::eleType::DOUBLE });
	auto myAttrDescsStable = attributeDescBuilder({ "ATTR_1", "ATTR_2" }, { core::eleType::CHAR, core::eleType::DOUBLE });
	EXPECT_EQ(*myAttrDescs, *myAttrDescsStable);

	auto myAttrDesc = myAttrDescs->at(0);
	auto myAttrDescStable = myAttrDescsStable->at(0);
	EXPECT_EQ(*myAttrDesc, *myAttrDescStable);

	//////////////////////////////
	// Copy
	attributeDesc copyDesc(*myAttrDesc);
	EXPECT_EQ(*myAttrDesc, *myAttrDescStable);
	EXPECT_EQ(copyDesc, *myAttrDescStable);

	//////////////////////////////
	// Swap
	attributeDesc rhsCopyDesc(std::move(*myAttrDesc));
	EXPECT_EQ(rhsCopyDesc, *myAttrDescStable);

	// arrDesc2D is now swapped with empty arrayDesc() obj
	// Expected not equal with original version of arrayDesc
	EXPECT_NE(*myAttrDesc, *myAttrDescStable);
	// Instead, expected equal with empty arrayDesc obj
	auto empty = core::attributeDesc();
	EXPECT_EQ(*myAttrDesc, empty);
}

TEST(dimensionDesc, copy_assign)
{
	using namespace msdb::core;

	dimension dims(1024, 1024);
	dimension chunkDims(128, 128);
	dimension blockDims(32, 32);

	auto myDimDescs = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	auto myDimDescsStable = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	EXPECT_EQ(*myDimDescs, *myDimDescsStable);

	auto myDimDesc = myDimDescs->at(0);
	auto myDimDescStable = myDimDescsStable->at(0);
	EXPECT_EQ(*myDimDesc, *myDimDescStable);

	//////////////////////////////
	// Copy
	dimensionDesc copyDesc(*myDimDesc);
	EXPECT_EQ(*myDimDesc, *myDimDescStable);
	EXPECT_EQ(copyDesc, *myDimDescStable);

	//////////////////////////////
	// Swap
	dimensionDesc rhsCopyDesc(std::move(*myDimDesc));
	EXPECT_EQ(rhsCopyDesc, *myDimDescStable);

	// arrDesc2D is now swapped with empty arrayDesc() obj
	// Expected not equal with original version of arrayDesc
	EXPECT_NE(*myDimDesc, *myDimDescStable);
	// Instead, expected equal with empty arrayDesc obj
	auto empty = core::dimensionDesc();
	EXPECT_EQ(*myDimDesc, empty);
}
}		// test
}		// msdb