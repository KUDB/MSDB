#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_save_astronomy_array.h>

namespace msdb
{
namespace experiments
{
TEST(tthresh_04_save, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeInsertSaveArray(msdb::dummy::data_star1024x1024::arrName,
						   msdb::dummy::data_star1024x1024::filePath,
						   core::encodingType::TTHRESH);
}
TEST(tthresh_04_save, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeInsertSaveArray(msdb::dummy::data_saturn1024x1024::arrName,
								  msdb::dummy::data_saturn1024x1024::filePath,
								  core::encodingType::TTHRESH);
}
TEST(tthresh_04_save, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeInsertSaveArray(msdb::dummy::data_solar1024x1024::arrName,
								  msdb::dummy::data_solar1024x1024::filePath,
								  core::encodingType::TTHRESH);
}
TEST(tthresh_04_save, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeInsertSaveArray(msdb::dummy::data_mars4096x2048::arrName,
								  msdb::dummy::data_mars4096x2048::filePath,
								  core::encodingType::TTHRESH);
}
TEST(tthresh_04_save, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeInsertSaveArray(msdb::dummy::data_mercury20480x10240::arrName,
								  msdb::dummy::data_mercury20480x10240::filePath,
								  core::encodingType::TTHRESH);
}
TEST(tthresh_04_save, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeInsertSaveArray(msdb::dummy::data_lunar102400x40960::arrName,
								  msdb::dummy::data_lunar102400x40960::filePath,
								  core::encodingType::TTHRESH);
}
}
}