#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_save_astronomy_array.h>

namespace msdb
{
namespace experiments
{
TEST(tthresh_05_load, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeLoadArray(dummy::data_star1024x1024::arrName,
							core::encodingType::TTHRESH);
}
TEST(tthresh_05_load, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeLoadArray(dummy::data_saturn1024x1024::arrName,
							core::encodingType::TTHRESH);
}
TEST(tthresh_05_load, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeLoadArray(dummy::data_solar1024x1024::arrName,
							core::encodingType::TTHRESH);
}
TEST(tthresh_05_load, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeLoadArray(dummy::data_mars4096x2048::arrName,
							core::encodingType::TTHRESH);
}
TEST(tthresh_05_load, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeLoadArray(dummy::data_mercury20480x10240::arrName,
							core::encodingType::TTHRESH);
}
TEST(tthresh_05_load, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::TTHRESH);

	dummy::executeLoadArray(dummy::data_lunar102400x40960::arrName,
							core::encodingType::TTHRESH);
}
}
}