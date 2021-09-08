#include <pch_test.h>
#include <dummy/dummy.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>

namespace msdb
{
namespace test
{
TEST(arrayDesc, xml_save)
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
}		// test
}		// msdb