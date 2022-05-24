#include <pch_experiments.h>

#include <exeQuery/arrayBuild.h>
#include <exeQuery/arraySaveLoad.h>

namespace msdb
{
namespace experiments
{
TEST(seacow_05_load, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_star1024x1024::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeLoadArray(msdb::dummy::data_star1024x1024::arrName,
							msdb::core::compressionType::SEACOW);
}
TEST(seacow_05_load, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_saturn1024x1024::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeLoadArray(msdb::dummy::data_saturn1024x1024::arrName,
							msdb::core::compressionType::SEACOW);
}
TEST(seacow_05_load, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_solar1024x1024::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeLoadArray(msdb::dummy::data_solar1024x1024::arrName,
							msdb::core::compressionType::SEACOW);
}
TEST(seacow_05_load, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_mars4096x2048::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeLoadArray(msdb::dummy::data_mars4096x2048::arrName,
							msdb::core::compressionType::SEACOW);
}
TEST(seacow_05_load, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_mercury20480x10240::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeLoadArray(msdb::dummy::data_mercury20480x10240::arrName,
							msdb::core::compressionType::SEACOW);
}
TEST(seacow_05_load, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_lunar102400x40960::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeLoadArray(msdb::dummy::data_lunar102400x40960::arrName,
							msdb::core::compressionType::SEACOW);
}
}
}