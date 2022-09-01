#include <pch_experiments.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <experiments/experiments_range.h>

namespace msdb
{
namespace experiments
{
TEST(seacow_huffman_15_range_filter, data_star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_star1024x1024::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_star1024x1024::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_star1024x1024::dims,
										   dummy::data_star1024x1024::valueRange[0],
										   dummy::data_star1024x1024::valueRange[1]);
}
TEST(seacow_huffman_15_range_filter, data_saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_saturn1024x1024::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_saturn1024x1024::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_saturn1024x1024::dims,
										   dummy::data_saturn1024x1024::valueRange[0],
										   dummy::data_saturn1024x1024::valueRange[1]);
}
TEST(seacow_huffman_15_range_filter, data_solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_solar1024x1024::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_solar1024x1024::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_solar1024x1024::dims,
										   dummy::data_solar1024x1024::valueRange[0],
										   dummy::data_solar1024x1024::valueRange[1]);
}
TEST(seacow_huffman_15_range_filter, data_mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_mars4096x2048::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_mars4096x2048::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_mars4096x2048::dims,
										   dummy::data_mars4096x2048::valueRange[0],
										   dummy::data_mars4096x2048::valueRange[1]);
}
TEST(seacow_huffman_15_range_filter, data_mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_mercury20480x10240::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_mercury20480x10240::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_mercury20480x10240::dims,
										   dummy::data_mercury20480x10240::valueRange[0],
										   dummy::data_mercury20480x10240::valueRange[1]);
}
TEST(seacow_huffman_15_range_filter, data_lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_lunar102400x40960::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_lunar102400x40960::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_lunar102400x40960::dims,
										   dummy::data_lunar102400x40960::valueRange[0],
										   dummy::data_lunar102400x40960::valueRange[1]);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(seacow_huffman_15_range_filter, data_solar_sdo_1024x1024x512)
{
	dummy::data_solar_sdo_1024x1024x512::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_solar_sdo_1024x1024x512::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_solar_sdo_1024x1024x512::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_solar_sdo_1024x1024x512::dims,
										   dummy::data_solar_sdo_1024x1024x512::valueRange[0],
										   dummy::data_solar_sdo_1024x1024x512::valueRange[1]);
}
TEST(seacow_huffman_15_range_filter, data_nexrad_201711_64x24x1024x2048)
{
	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_nexrad_201711_64x24x1024x2048::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	experimentRangeIndexFilterQry<uint8_t>(msdb::dummy::data_nexrad_201711_64x24x1024x2048::arrName,
										   msdb::core::encodingType::SEACOW_HUFFMAN,
										   dummy::data_nexrad_201711_64x24x1024x2048::dims,
										   dummy::data_nexrad_201711_64x24x1024x2048::valueRange[0],
										   dummy::data_nexrad_201711_64x24x1024x2048::valueRange[1]);
}
}		// experiments
}		// msdb