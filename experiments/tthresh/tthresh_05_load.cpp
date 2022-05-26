#include <pch_experiments.h>

#include <exeQuery/arrayBuild.h>
#include <exeQuery/arraySaveLoad.h>

namespace msdb
{
namespace experiments
{
TEST(tthresh_05_load, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::TTHRESH);

	dummy::executeLoadArray(dummy::data_star1024x1024::arrName,
							core::compressionType::TTHRESH);
}
TEST(tthresh_05_load, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::TTHRESH);

	dummy::executeLoadArray(dummy::data_saturn1024x1024::arrName,
							core::compressionType::TTHRESH);
}
TEST(tthresh_05_load, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::TTHRESH);

	dummy::executeLoadArray(dummy::data_solar1024x1024::arrName,
							core::compressionType::TTHRESH);
}
TEST(tthresh_05_load, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::TTHRESH);

	dummy::executeLoadArray(dummy::data_mars4096x2048::arrName,
							core::compressionType::TTHRESH);
}
TEST(tthresh_05_load, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::TTHRESH);

	dummy::executeLoadArray(dummy::data_mercury20480x10240::arrName,
							core::compressionType::TTHRESH);
}
TEST(tthresh_05_load, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::TTHRESH);

	dummy::executeLoadArray(dummy::data_lunar102400x40960::arrName,
							core::compressionType::TTHRESH);
}
}
}