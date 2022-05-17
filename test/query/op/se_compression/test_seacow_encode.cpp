#include <pch_test.h>
#include <dummy_astronomy_array.h>

#include <array/arrayMgr.h>

#include <index/mmt.h>
#include <index/compass.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/se_compression/seChunk.h>

namespace msdb
{
namespace test
{
TEST(seacow_encode, seacoe_afl)
{
	msdb::Context ctx;
	auto afl = msdb::Between(
		msdb::Decomp(
			msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
			msdb::compressionType::SEACOW,
			msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
		),
		msdb::Domain(msdb::Coordinates({ 0, 0 }), msdb::Coordinates({ 4, 4 }))
	);

	std::cout << afl->toString(0);

}
}		// test
}		// msdb