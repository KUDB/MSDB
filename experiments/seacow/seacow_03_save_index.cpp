#include <pch_experiments.h>

#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <api_cpp/msdb.h>
#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>

namespace msdb
{
namespace experiments
{
TEST(seacow_03_save_index, data_star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_star1024x1024::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_star1024x1024::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_03_save_index, data_saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_saturn1024x1024::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_saturn1024x1024::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_03_save_index, data_solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_solar1024x1024::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_solar1024x1024::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_03_save_index, data_mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_mars4096x2048::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_mars4096x2048::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_03_save_index, data_mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_mercury20480x10240::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_mercury20480x10240::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_03_save_index, data_lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_lunar102400x40960::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_lunar102400x40960::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
//TEST(seacow_03_save_index, data_star64x64x64)
//{
//	dummy::data_star64x64x64::executeBuildArray(
//		core::materializedType::FLATTEN, core::encodingType::SEACOW);
//
//	dummy::data_star64x64x64::executeBuildIndex(
//		core::encodingType::SEACOW, core::attrIndexType::MMT);
//
//	dummy::data_star64x64x64::executeSaveIndex(
//		core::encodingType::SEACOW, core::attrIndexType::MMT);
//}
TEST(seacow_03_save_index, data_nexrad_16x1024x2048)
{
	dummy::data_nexrad_16x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_nexrad_16x1024x2048::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_nexrad_16x1024x2048::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_03_save_index, data_solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_solar_sdo_1024x1024x512::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_solar_sdo_1024x1024x512::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
TEST(seacow_03_save_index, data_nexrad_201711_64x24x1024x2048)
{
	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_nexrad_201711_64x24x1024x2048::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}
}		// experiments
}		// msdb