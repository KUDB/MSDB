#include <pch.h>
#include <op/encode_raw/decode_raw_action.h>
#include <array/memBlockArray.h>
#include <array/blockChunk.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
decode_raw_action::decode_raw_action()
{
}
decode_raw_action::~decode_raw_action()
{
}
const char* decode_raw_action::name()
{
	return "decode_raw";
}
pArray decode_raw_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	assert(inputArrays.size() == 1);
	auto planChunkBitmap = this->getPlanInChunkBitmap();

	//pArray outArr = arrayMgr::instance()->makeArray<memBlockArray>(this->getArrayDesc());
	pArray outArr = std::make_shared<memBlockArray>(this->getArrayDesc());
	outArr->copyChunkBitmap(planChunkBitmap);
	arrayId arrId = outArr->getId();

	for (auto attr : *outArr->getDesc()->attrDescs_)
	{
		this->decode_rawAttribute(outArr, attr, qry);
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

void decode_raw_action::decode_rawAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
{
	size_t currentThreadId = 0;

	//========================================//
	qry->getTimer()->nextWork(0, workType::PARALLEL);
	//----------------------------------------//

	this->threadCreate(_MSDB_ACTION_THREAD_NUM_);

	auto cit = outArr->getChunkIterator(iterateMode::ALL);

	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cId = cit->seqPos();
			auto outChunk = outArr->makeChunk(attrDesc->id_, cId);

			auto blockBitmap = this->getPlanBlockBitmap(cId);
			if (blockBitmap)
			{
				outChunk->copyBlockBitmap(blockBitmap);
			} else
			{
				// If there were no bitmap, set all blocks as true.
				outChunk->replaceBlockBitmap(std::make_shared<bitmap>(outChunk->getBlockCapacity(), true));
			}
			outChunk->makeBlocks();

			io_service_->post(boost::bind(&decode_raw_action::decode_rawChunk, this,
							  outArr, outChunk, attrDesc->id_, qry, currentThreadId));
		}

		++(*cit);
	}

	this->threadStop();
	this->threadJoin();

	//----------------------------------------//
	qry->getTimer()->nextWork(0, workType::COMPUTING);
	//========================================//

	this->getArrayStatus(outArr);
}

void decode_raw_action::decode_rawChunk(pArray outArr, pChunk outChunk, attributeId attrId, pQuery qry, const size_t parentThreadId)
{
	auto threadId = getThreadId();

	//========================================//
	qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
	//----------------------------------------//

	pSerializable serialChunk
		= std::static_pointer_cast<serializable>(outChunk);
	storageMgr::instance()->decode_rawChunk(outArr->getId(), attrId, outChunk->getId(),
									  serialChunk);

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}
}		// core
}		// msdb