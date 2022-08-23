#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <exeQuery/exe_save_astronomy_array.h>

namespace msdb
{
namespace experiments
{

TEST(saecow_06_save_load, data_mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_mercury20480x10240::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_mercury20480x10240::arrName,
								  msdb::dummy::data_mercury20480x10240::filePath,
								  msdb::core::compressionType::SEACOW);

	dummy::executeLoadArray(msdb::dummy::data_mercury20480x10240::arrName,
							msdb::core::compressionType::SEACOW);
}
TEST(saecow_06_save_load, data_lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	dummy::data_lunar102400x40960::executeBuildIndex(
		core::compressionType::SEACOW, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_lunar102400x40960::arrName,
								  msdb::dummy::data_lunar102400x40960::filePath,
								  msdb::core::compressionType::SEACOW);

	dummy::executeLoadArray(msdb::dummy::data_lunar102400x40960::arrName,
							msdb::core::compressionType::SEACOW);
}
}		// experiments
}		// msdb