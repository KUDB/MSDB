#include <pch_experiments.h>

#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <dummy_astronomy_array_3d.h>
#include <api_cpp/msdb.h>
#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>

namespace msdb
{
namespace experiments
{
TEST(raw_01_build, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
TEST(raw_01_build, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
TEST(raw_01_build, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
TEST(raw_01_build, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
TEST(raw_01_build, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
TEST(raw_01_build, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(raw_01_build, data_star64x64x64)
{
	dummy::data_star64x64x64::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
TEST(raw_01_build, data_nexrad_16x1024x2048)
{
	dummy::data_nexrad_16x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::NONE);
}
}		// experiments
}		// msdb