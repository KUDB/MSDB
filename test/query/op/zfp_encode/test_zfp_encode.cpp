#include <pch_test.h>
#include <dummy/dummy_test_array.h>
#include <op/zfp_encode/zfp_encode_action.h>
#include <op/zfp_encode/zfpChunk.h>
#include "zfp_lib.h"

namespace msdb
{
namespace test
{
TEST(zfp_encode, zfp_encode_int8_t)
{
	core::bstream bsComp;
	core::dimension dim({ 32, 32 });
	int64_t inputSize = dim.area() * sizeof(char);
	int64_t outputBytes = 0;
	uint8_t* libInput = new uint8_t[inputSize];
	
	try
	{
		auto cDesc = getDummyChunkDesc_SIMPLE_2D<char>(dim);
		auto chk = std::make_shared<core::zfpChunk<char>>(cDesc);
		chk->makeAllBlocks();
		chk->bufferAlloc();
		
		auto blk = chk->getBlock(0);
		auto it = core::getTyIterator<char>(blk->getValueIterator());

		int i = 0;
		uint8_t tempValue = 0;

		// ------------------------------
		// Init source values
		auto seed = time(NULL);
		srand(seed);

		std::cout << "Seed: " << seed << std::endl;

		while (!it->isEnd())
		{
			tempValue = rand();
			libInput[i] = tempValue;
			(**it) = tempValue;
			++(*it);
			++i;
		}

		// ------------------------------
		// Compression
		// ------------------------------
		// Compress block
		blk->serialize(bsComp);

		// Compress raw source array
		uint8_t* libOutput = nullptr;
		zfp_lib_compress(libInput, libOutput, inputSize, outputBytes);


		// ------------------------------
		// Comparison
		// ------------------------------
		auto chunkOutput = (uint8_t*)(bsComp.data()) + sizeof(int64_t);
		EXPECT_TRUE(outputBytes > 0);

		for (int i = 0; i < outputBytes; ++i)
		{
			if (chunkOutput[i] != libOutput[i])
			{
				EXPECT_TRUE(false);
			}
		}

		// Jump over header of block, (compressed block size: int64_t)
		EXPECT_EQ(memcmp(chunkOutput, libOutput, outputBytes), 0);

	}
	catch (...)
	{
		delete[] libInput;
	}
}

TEST(zfp_encode, zfp_encode_decode)
{
	core::bstream bsComp;
	core::dimension dim({ 32, 32 });
	int64_t inputSize = dim.area() * sizeof(char);
	int64_t outputBytes = 0;
	uint8_t* source = new uint8_t[inputSize];

	try
	{
		auto cDesc = getDummyChunkDesc_SIMPLE_2D<char>(dim);
		auto chk = std::make_shared<core::zfpChunk<char>>(cDesc);
		chk->makeAllBlocks();
		chk->bufferAlloc();

		auto blk = chk->getBlock(0);
		auto it = core::getTyIterator<char>(blk->getValueIterator());

		int i = 0;
		uint8_t tempValue = 0;

		// ------------------------------
		// Init source values
		auto seed = time(NULL);
		srand(seed);

		std::cout << "Seed: " << seed << std::endl;

		while (!it->isEnd())
		{
			tempValue = rand();
			source[i] = tempValue;
			(**it) = tempValue;
			++(*it);
			++i;
		}

		// ------------------------------
		// Compression
		// ------------------------------
		// Compress block
		blk->serialize(bsComp);
		chk->flush();
		{
			auto cBuffer = core::chunkTester::getBuffer(chk);
			EXPECT_EQ(cBuffer, nullptr);
		}

		// ------------------------------
		// Decompression
		// ------------------------------
		// Decompress block
		chk->bufferAlloc();
		bsComp.moveToFront();
		blk->deserialize(bsComp);

		// ------------------------------
		// Comparison
		// ------------------------------
		{
			auto cBuffer = core::chunkTester::getBuffer(chk);
			EXPECT_TRUE(cBuffer->size() > 0);

			uint8_t* cData = (uint8_t*)cBuffer->getReadData();
			EXPECT_EQ(memcmp(cData, source, cBuffer->size()), 0);
		}
	}
	catch (...)
	{
		delete[] source;
	}
}
}		// test
}		// msdb

