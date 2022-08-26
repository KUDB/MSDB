#include <pch.h>
#include <op/lzw_encode/lzw_encode_action.h>
#include <system/storageMgr.h>
#include <op/lzw_encode/lzwChunk.h>
#include "lzwArray.h"
#include <util/logger.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
lzw_encode_action::lzw_encode_action()
{
}
lzw_encode_action::~lzw_encode_action()
{
}
const char* lzw_encode_action::name()
{
	return "lzw_encode_action";
}
pArray lzw_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	size_t currentThreadId = 0;
	//========================================//
	qry->getTimer()->nextJob(currentThreadId, this->name(), workType::PARALLEL);
	//----------------------------------------//
	this->threadCreate();

	size_t mSizeTotal = 0;
	pArray inArr = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArr->getDesc());
	pArray outArr = std::make_shared<lzwArray>(outArrDesc);

	arrayId arrId = inArr->getId();

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		if (attr->getCompType() != compressionType::LZW)
		{
			continue;
		}

		auto cit = inArr->getChunkIterator(attr->id_, iterateMode::EXIST);
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto cDesc = std::make_shared<chunkDesc>(*(*cit)->getDesc());
				auto outChunk = outArr->makeChunk(cDesc);
				outChunk->bufferCopy(**cit);
				outChunk->makeAllBlocks();

				////////////////////////////////////////
				// 1. Serialize::encodeChunk
				////////////////////////////////////////
				//pSerializable serialChunk
				//	= std::static_pointer_cast<serializable>(outChunk);
				//storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
				//								  serialChunk);
				////////////////////////////////////////

				////////////////////////////////////////
				// 2. Parallel::encodeChunk
				////////////////////////////////////////
				io_service_->post(boost::bind(&lzw_encode_action::encodeChunk, this,
											  arrId, attr->id_, outChunk, qry, currentThreadId));
				////////////////////////////////////////
			}

			++(*cit);
		}
	}

	this->threadStop();
	this->threadJoin();

	//----------------------------------------//
	qry->getTimer()->nextWork(currentThreadId, workType::COMPUTING);
	//----------------------------------------//

	for (auto attrDesc : *outArr->getDesc()->attrDescs_)
	{
		auto ocit = outArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		while (!ocit->isEnd())
		{
			auto outChunk = (**ocit);
			mSizeTotal += outChunk->getSerializedSize();

			++(*ocit);
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return inArr;
}

void lzw_encode_action::encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId)
{
	auto threadId = getThreadId() + 1;
	//========================================//
	qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::IO, std::string("chunk::") + std::to_string(outChunk->getId()));
	//----------------------------------------//
	storageMgr::instance()->saveChunk(arrId, attrId, outChunk->getId(), outChunk);
	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}
}		// core
}		// msdb