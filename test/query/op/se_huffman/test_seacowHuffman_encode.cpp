#include <pch_test.h>
#include <dummy_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_save_astronomy_array.h>

#include <array/arrayMgr.h>

#include <index/mmt.h>
#include <index/compass.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/seacow/seHuffmanChunk.h>

namespace msdb
{
namespace test
{
TEST(seacow_huffman_encode, seacow_data_star1024x1024)
{
	dummy::data_star1024x1024::executeBuildArray(
		core::materializedType::FLATTEN, core::encodingType::SEACOW_HUFFMAN);

	dummy::data_star1024x1024::executeBuildIndex(
		core::encodingType::SEACOW_HUFFMAN, core::attrIndexType::MMT);

	dummy::executeInsertSaveArray(msdb::dummy::data_star1024x1024::arrName,
								  msdb::dummy::data_star1024x1024::filePath,
								  msdb::core::encodingType::SEACOW_HUFFMAN);

	dummy::executeLoadArray(msdb::dummy::data_star1024x1024::arrName,
							msdb::core::encodingType::SEACOW_HUFFMAN);
}
}		// test
}		// msdb