#include <pch.h>
#include <system/exceptions.h>
#include <util/logger.h>
#include <util/threadUtil.h>
#include <system/storageMgr.h>
#include <op/sz3_encode/sz3_decode_action.h>
#include <op/sz3_encode/sz3Array.h>

namespace msdb
{
namespace core
{
sz3_decode_action::sz3_decode_action()
	: base_type()
{
}
sz3_decode_action::~sz3_decode_action()
{
}
const char* sz3_decode_action::name()
{
	return "sz3_decode_action";
}
pArray sz3_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	pArray sourceArr = inputArrays[0];

	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
	pArray outArr = std::make_shared<sz3Array>(outArrDesc);
	outArr->copyChunkBitmap(this->getPlanInChunkBitmap());

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		this->loadAttribute(outArr, attr, qry);
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

void sz3_decode_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
{
	size_t currentThreadId = 0;

	//========================================//
	qry->getTimer()->nextWork(0, workType::PARALLEL);
	//----------------------------------------//

	this->threadCreate();

	auto cit = outArr->getChunkIterator(attrDesc->id_, iterateMode::ALL);
	while (!cit->isEnd())
	{
		if (cit->needToMake())
		{
			chunkId cid = cit->seqPos();

			//auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
			auto outChunk = outArr->makeChunk(attrDesc->id_, cid);
			outChunk->bufferAlloc();
			outChunk->makeAllBlocks();

			////////////////////////////////////////
			// 1. Serialize::decompressChunk
			////////////////////////////////////////
			//this->loadChunk(qry, currentThreadId, outArr, attrDesc->id_, outChunk);
			////////////////////////////////////////

			////////////////////////////////////////
			// 2. Parallel::decompressChunk
			////////////////////////////////////////
			io_service_->post(boost::bind(&sz3_decode_action::loadChunk, this,
										  qry, currentThreadId, outArr, attrDesc->id_, outChunk));
			////////////////////////////////////////
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

void sz3_decode_action::loadChunk(pQuery qry, const size_t parentThreadId, pArray outArr, attributeId attrId, pChunk outChunk)
{
	auto threadId = getThreadId() + 1;

	//========================================//
	qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::IO, std::string("chunk::") + std::to_string(outChunk->getId()));
	//----------------------------------------//

	pSerializable serialChunk
		= std::static_pointer_cast<serializable>(outChunk);
	storageMgr::instance()->loadChunk(outArr->getId(), attrId, outChunk->getId(),
									  serialChunk);

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}
}		// core
}		// msdb