#include <pch_experiments.h>

#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <api_cpp/msdb.h>
#include <exeQuery/exe_build_astronomy_array.h>

namespace msdb
{
namespace experiments
{
TEST(zfp_01_build, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZFP);
}
TEST(zfp_01_build, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZFP);
}
TEST(zfp_01_build, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZFP);
}
TEST(zfp_01_build, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZFP);
}
TEST(zfp_01_build, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZFP);
}
TEST(zfp_01_build, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZFP);
}
}		// experiments
}		// msdb