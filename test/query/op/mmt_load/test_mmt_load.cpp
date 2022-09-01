#include <pch_test.h>

// for dummy data
#include <dummy_astronomy_array.h>
#include <dummy_astronomy_array_3d.h>
#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>

namespace msdb
{
namespace test
{
TEST(mmt_save_load, data_star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_star1024x1024::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_star1024x1024::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_star1024x1024::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}

TEST(mmt_load, data_star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_star1024x1024::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

}

TEST(mmt_save_load, data_saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_saturn1024x1024::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_saturn1024x1024::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_saturn1024x1024::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}

TEST(mmt_load, data_saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_saturn1024x1024::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

}

TEST(mmt_save_load, data_solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_solar_sdo_1024x1024x512::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_solar_sdo_1024x1024x512::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_solar_sdo_1024x1024x512::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}

TEST(mmt_load, data_solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_solar_sdo_1024x1024x512::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

}

TEST(mmt_save_load, data_nexrad_201711_64x24x1024x2048)
{
	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_nexrad_201711_64x24x1024x2048::executeSaveIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

	dummy::data_nexrad_201711_64x24x1024x2048::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);
}

TEST(mmt_load, data_nexrad_201711_64x24x1024x2048)
{
	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW);

	dummy::data_nexrad_201711_64x24x1024x2048::executeLoadIndex(
		core::encodingType::SEACOW, core::attrIndexType::MMT);

}
}		// test
}		// msdb