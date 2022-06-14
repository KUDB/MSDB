#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <exeQuery/exe_save_astronomy_array.h>

namespace msdb
{
namespace experiments
{
TEST(zip_05_load, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_star1024x1024::arrName,
							core::compressionType::ZIP);
}
TEST(zip_05_load, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_saturn1024x1024::arrName,
							core::compressionType::ZIP);
}
TEST(zip_05_load, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_solar1024x1024::arrName,
							core::compressionType::ZIP);
}
TEST(zip_05_load, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_mars4096x2048::arrName,
							core::compressionType::ZIP);
}
TEST(zip_05_load, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_mercury20480x10240::arrName,
							core::compressionType::ZIP);
}
TEST(zip_05_load, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_lunar102400x40960::arrName,
							core::compressionType::ZIP);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(zip_05_load, data_nexrad_16x1024x2048)
{
	dummy::data_nexrad_16x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_nexrad_16x1024x2048::arrName,
							core::compressionType::ZIP);
}
TEST(zip_05_load, data_solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_solar_sdo_1024x1024x512::arrName,
							core::compressionType::ZIP);
}
TEST(zip_05_load, data_nexrad_201711_64x24x1024x2048)
{
	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeLoadArray(dummy::data_nexrad_201711_64x24x1024x2048::arrName,
							core::compressionType::ZIP);
}
}		// experiments
}		// msdb