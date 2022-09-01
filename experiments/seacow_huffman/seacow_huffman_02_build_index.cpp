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
TEST(seacow_huffman_02_build_index, star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_star1024x1024::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
TEST(seacow_huffman_02_build_index, saturn1024x1024)
{
	dummy::data_saturn1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_saturn1024x1024::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
TEST(seacow_huffman_02_build_index, solar1024x1024)
{
	dummy::data_solar1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_solar1024x1024::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
TEST(seacow_huffman_02_build_index, mars4096x2048)
{
	dummy::data_mars4096x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_mars4096x2048::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
TEST(seacow_huffman_02_build_index, mercury20480x10240)
{
	dummy::data_mercury20480x10240::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_mercury20480x10240::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
TEST(seacow_huffman_02_build_index, lunar102400x40960)
{
	dummy::data_lunar102400x40960::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_lunar102400x40960::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
////////////////////////////////////////
// 3D Data
////////////////////////////////////////
TEST(seacow_huffman_02_build_index, star64x64x64)
{
	dummy::data_star64x64x64::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_star64x64x64::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
TEST(seacow_huffman_02_build_index, data_nexrad_16x1024x2048)
{
	dummy::data_nexrad_16x1024x2048::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_nexrad_16x1024x2048::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);
}
}		// experiments
}		// msdb