#pragma once
#ifndef _MSDB_OP_SPIHT_ENCODE_ACTION_H_
#define _MSDB_OP_SPIHT_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <util/math.h>
#include <system/storageMgr.h>
#include "spihtChunk.h"
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
class spiht_encode_action : public opAction
{
public:
	spiht_encode_action();
	virtual ~spiht_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);


	template <typename Ty_>
	void compressAttribute(const concreteTy<Ty_>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc, pQuery qry, size_t level)
	{
		size_t mSizeTotal = 0;
		arrayId arrId = inArr->getId();
		attributeId attrId = attrDesc->getId();

		//========================================//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//

		size_t currentThreadId = 0;
		this->threadCreate();

		auto cit = inArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto cDesc = std::make_shared<chunkDesc>(*(*cit)->getDesc());
				auto outChunk = outArr->makeChunk(cDesc);
				outChunk->bufferCopy(**cit);
				outChunk->makeAllBlocks();

				// Set Param
				auto sChunk = std::static_pointer_cast<spihtChunk<Ty_>>(outChunk);
				sChunk->setLevel(level);

				//========================================//
				//qry->getTimer()->nextWork(0, workType::IO);
				//----------------------------------------//
				//pSerializable serialChunk
				//    = std::static_pointer_cast<serializable>(outChunk);
				//storageMgr::instance()->saveChunk(arrId, attrId, (outChunk)->getId(),
				//                                  serialChunk);

				io_service_->post(boost::bind(&spiht_encode_action::compressChunk<Ty_>, this,
											  arrId, attrId, outChunk, currentThreadId));

				//========================================//
				//qry->getTimer()->nextWork(0, workType::COMPUTING);
				//----------------------------------------//
			}

			++(*cit);
		}

		this->threadStop();
		this->threadJoin();

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//========================================//

		auto ocit = outArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		while (!ocit->isEnd())
		{
			if (ocit->isExist())
			{
				auto outChunk = (**ocit);
				mSizeTotal += outChunk->getSerializedSize();
			}
			++(*ocit);
		}

		BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	}

	template <typename Ty_>
	void compressChunk(arrayId arrId, attributeId attrId, pChunk outChunk, const size_t parentThreadId)
	{
		pSerializable serialChunk
			= std::static_pointer_cast<serializable>(outChunk);
		storageMgr::instance()->saveChunk(arrId, attrId, (outChunk)->getId(),
										  serialChunk);
	}
};
}		// core
}		// msdb
#endif // _MSDB_OP_SPIHT_ENCODE_ACTION_H_