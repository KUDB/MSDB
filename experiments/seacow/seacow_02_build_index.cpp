#include <pch_experiments.h>

#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <api_cpp/msdb.h>
#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>

namespace msdb
{
namespace experiments
{
TEST(seacow_02_build_index, data_star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_star1024x1024::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_02_build_index, data_saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_saturn1024x1024::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_02_build_index, data_solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_solar1024x1024::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_02_build_index, data_mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_mars4096x2048::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_02_build_index, data_mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_mercury20480x10240::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_02_build_index, data_lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_lunar102400x40960::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(seacow_02_build_index, data_star64x64x64)
{
	dummy::data_star64x64x64::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_star64x64x64::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_02_build_index, data_nexrad_16x1024x2048)
{
	dummy::data_nexrad_16x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_nexrad_16x1024x2048::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);
}
}		// experiments
}		// msdb