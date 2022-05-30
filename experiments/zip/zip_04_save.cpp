#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <exeQuery/exe_save_astronomy_array.h>

namespace msdb
{
namespace experiments
{
TEST(zip_04_save, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_star1024x1024::arrName,
								  dummy::data_star1024x1024::filePath,
								  core::compressionType::ZIP);
}
TEST(zip_04_save, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_saturn1024x1024::arrName,
								  dummy::data_saturn1024x1024::filePath,
								  core::compressionType::ZIP);
}
TEST(zip_04_save, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_solar1024x1024::arrName,
								  dummy::data_solar1024x1024::filePath,
								  core::compressionType::ZIP);
}
TEST(zip_04_save, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_mars4096x2048::arrName,
								  dummy::data_mars4096x2048::filePath,
								  core::compressionType::ZIP);
}
TEST(zip_04_save, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_mercury20480x10240::arrName,
								  dummy::data_mercury20480x10240::filePath,
								  core::compressionType::ZIP);
}
TEST(zip_04_save, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_lunar102400x40960::arrName,
								  dummy::data_lunar102400x40960::filePath,
								  core::compressionType::ZIP);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(zip_04_save, data_nexrad_16x1024x2048)
{
	dummy::data_nexrad_16x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_nexrad_16x1024x2048::arrName,
								  dummy::data_nexrad_16x1024x2048::filePath,
								  core::compressionType::ZIP);
}
TEST(zip_04_save, solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::ZIP);

	dummy::executeInsertSaveArray(dummy::data_solar_sdo_1024x1024x512::arrName,
								  dummy::data_solar_sdo_1024x1024x512::filePath,
								  core::compressionType::ZIP);
}
}		// experiments
}		// msdb