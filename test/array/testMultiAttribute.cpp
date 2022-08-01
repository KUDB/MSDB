#include <pch_test.h>
#include <dummy/dummy_test_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <util/datatype.h>

namespace msdb
{
namespace test
{
TEST(multiAttribute, twoAttr2D)
{
	//auto arrDesc = getDummyArrayDesc_MultiAttr_2D();
	//auto expectFilePath = std::filesystem::path("../storage") / std::filesystem::path(std::to_string(arrDesc->id_) + ".msdbarray");

	//std::filesystem::remove(expectFilePath);

	//core::arrayMgr::instance()->registArray(arrDesc);

	////////////////////////////////////////////////////
	//{
	//	auto buildAfl = msdb::dummy::array_mem_char_4x4::getArrayBuildAFL();
	//	auto qry = msdb::Query(buildAfl);
	//	auto ra = qry.execute();
	//	std::cout << qry.strStatus() << std::endl;
	//	//std::cout << buildQry.getTimer()->getDetailResult() << std::endl;

	//	EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	//}
	////////////////////////////////////////////////////


}		// TEST
}		// test
}		// msdb