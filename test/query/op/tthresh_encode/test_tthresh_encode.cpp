#include <pch_test.h>
#include <dummy/dummy.h>
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

		// Compress raw source array
		uint8_t* libOutput = nullptr;
		//zfp_lib_compress(libInput, libOutput, inputSize, outputBytes);


	}
	catch (...)
	{

	}
}
}		// test
}		// msdb