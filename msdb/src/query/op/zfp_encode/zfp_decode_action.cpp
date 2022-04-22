#include <pch.h>
#include <system/exceptions.h>
#include <util/logger.h>
#include <util/threadUtil.h>
#include <system/storageMgr.h>
#include <op/zfp_encode/zfp_decode_action.h>
#include <op/zfp_encode/zfpArray.h>

namespace msdb
{
namespace core
{
zfp_decode_action::zfp_decode_action()
    : base_type()
{
}
zfp_decode_action::~zfp_decode_action()
{
}
const char* zfp_decode_action::name()
{
    return "zfp_decode_action";
}
pArray zfp_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
    assert(inputArrays.size() == 1);

    //========================================//
    qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
    //----------------------------------------//

    pArray sourceArr = inputArrays[0];

    pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
    pArray outArr = std::make_shared<zfpArray>(outArrDesc);
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

void zfp_decode_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
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

			io_service_->post(boost::bind(&zfp_decode_action::loadChunk, this,
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

void zfp_decode_action::loadChunk(pQuery qry, const size_t parentThreadId, pArray outArr, attributeId attrId, pChunk outChunk)
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