#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <experiments/experiments_filter.h>

namespace msdb
{
namespace experiments
{
TEST(seacow_15_range_filter, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_star1024x1024::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	experimentIndexFilterQry<uint8_t>(msdb::dummy::data_star1024x1024::arrName,
									  msdb::core::compressionType::SEACOW);
}
}		// experiments
}		// msdb