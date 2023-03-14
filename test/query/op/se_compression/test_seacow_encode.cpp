#include "gtest/gtest.h"
#include <dummy_astronomy_array.h>
#include <dummy_query_util.h>
#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <exeQuery/exe_save_astronomy_array.h>

#include <array/arrayMgr.h>

#include <index/mmt.h>
#include <index/compass.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/seacow/seChunk.h>

#include <api_cpp_operator/cpp_io_operators.h>

namespace msdb
{
namespace test
{
TEST(seacow_encode, seacow_afl)
{
	msdb::Context ctx;
	auto afl = msdb::Between(
		msdb::Decomp(
			msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
			msdb::encodingType::SEACOW,
			msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
		),
		msdb::Domain(msdb::Coordinates({ 0, 0 }), msdb::Coordinates({ 4, 4 }))
	);

	std::cout << afl->toString(0);
}

TEST(seacow_encode, seacow_data_star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_star1024x1024::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_star1024x1024::arrName,
								  msdb::dummy::data_star1024x1024::filePath,
								  msdb::core::encodingType::SEACOW);

	dummy::executeLoadArray(msdb::dummy::data_star1024x1024::arrName,
							msdb::core::encodingType::SEACOW);
}

TEST(seacow_range_filter, data_nexrad_201711_64x24x1024x2048)
{
	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_nexrad_201711_64x24x1024x2048::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);


	Domain r = Domain(Coordinates({462, 268, 8, 2}), Coordinates({1613, 843, 21, 37}));
	int64_t v = 6;

	dummy::exeRangeIndexFilterQry(dummy::data_nexrad_201711_64x24x1024x2048::arrName, core::encodingType::SEACOW, r, v);
}
}		// test
}		// msdb