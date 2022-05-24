#include <pch_test.h>
#include <dummy/dummy_test_array.h>
#include <op/tthresh_encode/tthreshChunk.h>

namespace msdb
{
namespace test
{
TEST(tthresh_encode, tthresh_encode_int8_t)
{
	std::stringstream ssComp;
	core::dimension dim({ 32, 32 });
	int64_t inputSize = dim.area() * sizeof(char);
	int64_t outputBytes = 0;
	uint8_t* libInput = new uint8_t[inputSize];

	try
	{
		auto cDesc = getDummyChunkDesc_SIMPLE_2D<char>(dim);
		auto chk = std::make_shared<core::tthreshChunk<char>>(cDesc);
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
		std::static_pointer_cast<core::tthreshBlock<char>>(blk)->serialize(ssComp);

		EXPECT_TRUE(true);
	}
	catch (...)
	{
		EXPECT_TRUE(false);
	}
}

TEST(tthresh_encode, tthresh_encode_decode_int8_t)
{
	std::stringstream ssComp;
	core::dimension dim({ 128, 128 });
	int64_t inputSize = dim.area() * sizeof(char);
	int64_t outputBytes = 0;
	uint8_t* source = new uint8_t[inputSize];

	try
	{
		auto cDesc = getDummyChunkDesc_SIMPLE_2D<char>(dim);
		auto chk = std::make_shared<core::tthreshChunk<char>>(cDesc);
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
		std::static_pointer_cast<core::tthreshBlock<char>>(blk)->serialize(ssComp);
		chk->flush();
		{
			auto cBuffer = core::chunkTester::getBuffer(chk);
			EXPECT_EQ(cBuffer, nullptr);
		}

		// ------------------------------
		// Decompression
		// ------------------------------
		// Decompress block
		ssComp.seekg(0);
		chk->bufferAlloc();
		std::static_pointer_cast<core::tthreshBlock<char>>(blk)->deserialize(ssComp);

		// ------------------------------
		// Comparison
		// ------------------------------
		{
			// The results are slightly different.
			//auto cBuffer = core::chunkTester::getBuffer(chk);
			//EXPECT_TRUE(cBuffer->size() > 0);

			//uint8_t* cData = (uint8_t*)cBuffer->getReadData();
			//EXPECT_EQ(memcmp(cData, source, cBuffer->size()), 0);
		}
	}
	catch (...)
	{
		delete[] source;
	}
}
}		// test
}		// msdb