#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <experiments/experiments_filter.h>

namespace msdb
{
namespace experiments
{
TEST(zip_10_filter, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint8_t>(msdb::dummy::data_star1024x1024::arrName,
								 msdb::core::compressionType::ZIP);
}
TEST(zip_10_filter, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint8_t>(msdb::dummy::data_saturn1024x1024::arrName,
								 msdb::core::compressionType::ZIP);
}
TEST(zip_10_filter, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint8_t>(msdb::dummy::data_solar1024x1024::arrName,
								 msdb::core::compressionType::ZIP);
}
TEST(zip_10_filter, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint8_t>(msdb::dummy::data_mars4096x2048::arrName,
								 msdb::core::compressionType::ZIP);
}
TEST(zip_10_filter, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint8_t>(msdb::dummy::data_mercury20480x10240::arrName,
								 msdb::core::compressionType::ZIP);
}
TEST(zip_10_filter, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint16_t>(msdb::dummy::data_lunar102400x40960::arrName,
								  msdb::core::compressionType::ZIP);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(zip_10_filter, data_solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint8_t>(msdb::dummy::data_solar_sdo_1024x1024x512::arrName,
								 msdb::core::compressionType::ZIP);
}
TEST(zip_10_filter, data_nexrad_201711_64x24x1024x2048)
{
	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	experimentFilterQry<uint8_t>(msdb::dummy::data_nexrad_201711_64x24x1024x2048::arrName,
								 msdb::core::compressionType::ZIP);
}
}		// experiments
}		// msdb