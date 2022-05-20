#include <pch.h>
#include <system/exceptions.h>
#include <util/logger.h>
#include <util/threadUtil.h>
#include <system/storageMgr.h>
#include <op/tthresh_encode/tthresh_decode_action.h>
#include <op/tthresh_encode/tthreshArray.h>

namespace msdb
{
namespace core
{
tthresh_decode_action::tthresh_decode_action()
	: base_type()
{
}
tthresh_decode_action::~tthresh_decode_action()
{
}
const char* tthresh_decode_action::name()
{
	return "tthresh_decode_action";
}
pArray tthresh_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	pArray sourceArr = inputArrays[0];

	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
	pArray outArr = std::make_shared<tthreshArray>(outArrDesc);
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

void tthresh_decode_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
{
	size_t currentThreadId = 0;

	//========================================//
	qry->getTimer()->nextWork(0, workType::PARALLEL);
	//----------------------------------------//

	this->threadCreate();

	auto cit = outArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cid = cit->seqPos();
			//auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
			auto outChunk = outArr->makeChunk(attrDesc->id_, cid);
			outChunk->bufferAlloc();
			outChunk->makeAllBlocks();

			io_service_->post(boost::bind(&tthresh_decode_action::loadChunk, this,
										  qry, currentThreadId, outArr, attrDesc->id_, outChunk));
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

void tthresh_decode_action::loadChunk(pQuery qry, const size_t parentThreadId, pArray outArr, attributeId attrId, pChunk outChunk)
{
	auto threadId = getThreadId();

	//========================================//
	qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
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