#include "gtest/gtest.h"
#include <dummy/dummy_test_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>

namespace msdb
{
namespace test
{
TEST(arrayDesc, xml_save_simple_2D)
{
	auto arrDesc = getDummyArrayDesc_SIMPLE_2D();
	auto expectFilePath = std::filesystem::path("../storage") / std::filesystem::path(std::to_string(arrDesc->id_) + ".msdbarray");

	std::filesystem::remove(expectFilePath);

	core::arrayMgr::instance()->registArray(arrDesc);
	core::arrayMgr::instance()->saveArrayDesc(arrDesc->id_);	
	EXPECT_TRUE(std::filesystem::exists(expectFilePath));

	auto loadArrDesc = core::storageMgr::instance()->loadArrayDesc(expectFilePath);
	EXPECT_TRUE(*arrDesc == *loadArrDesc);
}

TEST(arrayDesc, xml_save_multiattr_2D)
{
	auto arrDesc = getDummyArrayDesc_MultiAttr_2D();
	auto expectFilePath = std::filesystem::path("../storage") / std::filesystem::path(std::to_string(arrDesc->id_) + ".msdbarray");

	std::filesystem::remove(expectFilePath);

	core::arrayMgr::instance()->registArray(arrDesc);
	core::arrayMgr::instance()->saveArrayDesc(arrDesc->id_);
	EXPECT_TRUE(std::filesystem::exists(expectFilePath));

	auto loadArrDesc = core::storageMgr::instance()->loadArrayDesc(expectFilePath);
	EXPECT_TRUE(*arrDesc == *loadArrDesc);
}

TEST(arrayDesc, xml_save_multiattr_3D)
{
	auto arrDesc = getDummyArrayDesc_MultiAttr_3D();
	auto expectFilePath = std::filesystem::path("../storage") / std::filesystem::path(std::to_string(arrDesc->id_) + ".msdbarray");

	std::filesystem::remove(expectFilePath);

	core::arrayMgr::instance()->registArray(arrDesc);
	core::arrayMgr::instance()->saveArrayDesc(arrDesc->id_);
	EXPECT_TRUE(std::filesystem::exists(expectFilePath));

	auto loadArrDesc = core::storageMgr::instance()->loadArrayDesc(expectFilePath);
	EXPECT_TRUE(*arrDesc == *loadArrDesc);
}

TEST(arrayDesc, copy_assign)
{
	using namespace msdb::core;

	auto arrDesc2D = getDummyArrayDesc_SIMPLE_2D();
	auto arrDesc2DStable = getDummyArrayDesc_SIMPLE_2D();

	EXPECT_EQ(*arrDesc2D, *arrDesc2DStable);

	arrayDesc copyDesc(*arrDesc2D);
	EXPECT_EQ(*arrDesc2D, *arrDesc2DStable);
	EXPECT_EQ(copyDesc, *arrDesc2DStable);

	arrayDesc rhsCopyDesc(std::move(*arrDesc2D));
	EXPECT_EQ(rhsCopyDesc, *arrDesc2DStable);

	// arrDesc2D is now swapped with empty arrayDesc() obj
	// Expected not equal with original version of arrayDesc
	EXPECT_NE(*arrDesc2D, *arrDesc2DStable);
	// Instead, expected equal with empty arrayDesc obj
	auto empty = core::arrayDesc();
	EXPECT_EQ(*arrDesc2D, empty);
}
}		// test
}		// msdb