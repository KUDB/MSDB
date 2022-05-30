#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <exeQuery/exe_save_astronomy_array.h>

namespace msdb
{
namespace experiments
{
TEST(seacow_huffman_04_save, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_star1024x1024::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_star1024x1024::arrName,
								  msdb::dummy::data_star1024x1024::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
TEST(seacow_huffman_04_save, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_saturn1024x1024::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_saturn1024x1024::arrName,
								  msdb::dummy::data_saturn1024x1024::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
TEST(seacow_huffman_04_save, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_solar1024x1024::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_solar1024x1024::arrName,
								  msdb::dummy::data_solar1024x1024::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
TEST(seacow_huffman_04_save, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_mars4096x2048::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_mars4096x2048::arrName,
								  msdb::dummy::data_mars4096x2048::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
TEST(seacow_huffman_04_save, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_mercury20480x10240::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_mercury20480x10240::arrName,
								  msdb::dummy::data_mercury20480x10240::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
TEST(seacow_huffman_04_save, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_lunar102400x40960::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_lunar102400x40960::arrName,
								  msdb::dummy::data_lunar102400x40960::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(seacow_huffman_04_save, data_star64x64x64)
{
	dummy::data_star64x64x64::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_star64x64x64::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_star64x64x64::arrName,
								  msdb::dummy::data_star64x64x64::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
TEST(seacow_huffman_04_save, data_nexrad_16x1024x2048)
{
	dummy::data_nexrad_16x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_nexrad_16x1024x2048::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_nexrad_16x1024x2048::arrName,
								  msdb::dummy::data_nexrad_16x1024x2048::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
TEST(seacow_huffman_04_save, data_solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::compressionType::SEACOW_HUFFMAN);

	dummy::data_solar_sdo_1024x1024x512::executeBuildIndex(
		core::compressionType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_solar_sdo_1024x1024x512::arrName,
								  msdb::dummy::data_solar_sdo_1024x1024x512::filePath,
								  msdb::core::compressionType::SEACOW_HUFFMAN);
}
}		// experiment
}		// msdb