#include "gtest/gtest.h"
#include <array/bufferMgr.h>
#include <array/chunkBuffer.h>

namespace msdb
{
namespace test
{
TEST(bufferMgr, defaultLimit)
{
	auto mgr = core::bufferMgr::instance();

	auto bl = mgr->getBufferLimit();
	mgr->setBufferLimit(0);
	EXPECT_EQ(bl, 0);
}

TEST(bufferMgr, cache_flush)
{
	auto mgr = core::bufferMgr::instance();

	mgr->setBufferLimit(0);
	mgr->flushAll();
	EXPECT_EQ(mgr->getCurrentBufferSize(), 0);

	int arrIdMax = 5;
	int attrIdMax = 5;
	int cidMax = 20;

	int sum = 0;
	std::vector<std::vector<std::vector<core::bufferSize>>> size(arrIdMax);
	// Use a fixed seed
	srand(0);

	//////////////////////////////////////////////////
	// Cache Chunk Buffer Test
	for (core::arrayId arrId = 0; arrId < arrIdMax; ++arrId)
	{
		for (core::attributeId attrId = 0; attrId < attrIdMax; ++attrId)
		{
			size[arrId].push_back(std::vector<core::bufferSize>());
			for (core::chunkId cid = 0; cid < cidMax; ++cid)
			{
				int curSize = rand() % 1024;
				size[arrId][attrId].push_back(curSize);
				auto buf = std::make_shared<core::chunkBuffer>();
				buf->bufferAlloc(size[arrId][attrId][cid]);
				mgr->cacheChunkBuffer(arrId, attrId, cid, buf);

				EXPECT_TRUE(mgr->isExist({ arrId, attrId, cid }));
				sum += curSize;
				EXPECT_EQ(mgr->getCurrentBufferSize(), sum);
			}
		}
	}
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	// Get Existing Chunk Buffer Test
	for (core::arrayId arrId = 1; arrId < arrIdMax; ++arrId)
	{
		for (core::attributeId attrId = 0; attrId < attrIdMax; ++attrId)
		{
			for (int i = 0; i < cidMax * 5; ++i)
			{
				core::chunkId cid = rand() % cidMax;
				auto buf = mgr->getChunkBuffer(arrId, attrId, cid);
				EXPECT_NE(buf, nullptr);
				EXPECT_EQ(buf->size(), size[arrId][attrId][cid]);
			}
		}
	}
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	// Get Random Chunk Buffer Test
	int testCase = 0;
	while (testCase++ < 100)
	{
		core::arrayId arrId = rand() % (arrIdMax * 2);
		core::attributeId attrId = rand() % (attrIdMax * 2);
		core::chunkId cid = rand() % (cidMax * 2);

		auto buf = mgr->getChunkBuffer(arrId, attrId, cid);

		if (arrId < arrIdMax && attrId < attrIdMax && cid < cidMax)
		{
			EXPECT_TRUE(mgr->isExist({ arrId, attrId, cid }));
			EXPECT_NE(buf, nullptr);
			EXPECT_EQ(buf->size(), size[arrId][attrId][cid]);
		}
		else
		{
			EXPECT_FALSE(mgr->isExist({ arrId, attrId, cid }));
			EXPECT_EQ(buf, nullptr);
		}
	}
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	// Chunk Buffer Size Test
	EXPECT_EQ(mgr->getCurrentBufferSize(), sum);

	//////////////////////////////////////////////////
	// Flush test
	{
		core::arrayId arrId = rand() % arrIdMax;
		core::attributeId attrId = rand() % attrIdMax;
		core::chunkId cid = rand() % cidMax;

		EXPECT_TRUE(mgr->isExist(arrId, attrId, cid));
		auto beforeSize = mgr->getCurrentBufferSize();

		auto buf = mgr->getChunkBuffer(arrId, attrId, cid);
		EXPECT_NE(buf, nullptr);
		{
			auto size = buf->size();

			mgr->flushChunkBuffer(arrId, attrId, cid);
			EXPECT_FALSE(mgr->isExist(arrId, attrId, cid));
			EXPECT_EQ(mgr->getCurrentBufferSize(), beforeSize - size);
		}
	}
	//////////////////////////////////////////////////
	
	//////////////////////////////////////////////////
	// Flush ALl Test
	mgr->flushAll();
	EXPECT_EQ(mgr->getCurrentBufferSize(), 0);

	//////////////////////////////////////////////////
	mgr->setBufferLimit(0);
}
}		// test
}		// msdb