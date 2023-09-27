#include "gtest/gtest.h"
#include <api_python_ml/python_ml.h>
#include <api_python_ml/ml_dataloader.h>
#include <numeric>

namespace msdb
{
namespace test
{
TEST(api_python_ml, dataloader_batch_1)
{
	core::arrayId xArrId = 103203203;
	core::arrayId yArrId = 103203201;

	core::attributeId xAttrId = 0;
	core::attributeId yAttrId = 0;

	size_t itemBufferSize = 1;
	size_t batchSize = 1;
	size_t xBufferSize = 0;
	size_t yBufferSize = 0;

	api_python_ml::ml_dataloader loader(xArrId, yArrId, itemBufferSize, batchSize, xBufferSize, yBufferSize);

	auto numItems = loader.getNumItems();
	EXPECT_NE(0, numItems);

	{
		// Test Item Sequence
		std::vector<uint64_t> testSeq(numItems);
		std::iota(testSeq.begin(), testSeq.end(), 0);

		EXPECT_EQ(testSeq, loader.getSeq());

		loader.suffleSequence();
		EXPECT_NE(testSeq, loader.getSeq());
	}

	//{
	//	loader.activePreloaderThread();
	//	EXPECT_TRUE(loader.isPreloadActivated());
	//}

	for (int i = 0; i < 10; ++i)
	{
		auto next = loader.getNextItemBatch();
	}
}		// TEST(dataloader)
}		// test
}		// msdb