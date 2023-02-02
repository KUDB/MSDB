#pragma once
#ifndef _MSDB_OP_SPIHT_DECODE_ACTION_H
#define _MSDB_OP_SPIHT_DECODE_ACTION_H

#include <pch_op.h>
#include <query/opAction.h>
#include <util/math.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <op/spiht_encode/spihtChunk.h>
#include <op/wavelet_encode/wtChunk.h>
#include <array/attributeId.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>
#include <encoding/encodingParam.h>

namespace msdb
{
namespace core
{
class spiht_decode_action : public opAction
{
public:
	spiht_decode_action();
	virtual ~spiht_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& outputArrays, pQuery qry);

	//pSpihtChunk makeInChunk(std::shared_ptr<wavelet_encode_array>arr, pAttributeDesc attrDesc, 
	//						chunkId cid);

	template <typename Ty_>
	void decodeAttribute(const concreteTy<Ty_>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery qry)
	{
		auto cit = inArr->getChunkIterator(attrDesc->id_, iterateMode::ALL);
		auto cBitmap = cit->getChunkBitmap();

		auto attrId = attrDesc->id_;
		size_t wtLevel = std::stoi(attrDesc->getParam(_STR_PARAM_WAVELET_LEVEL_));

		//========================================//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//

		size_t currentThreadId = 0;
		this->threadCreate();

		while (!cit->isEnd())
		{
			chunkId cid = cit->seqPos();
			if (cBitmap->isExist(cid))
			{
				chunkId cid = cit->seqPos();

				auto inChunk = std::static_pointer_cast<spihtChunk<Ty_>>(inArr->makeChunk(attrDesc->id_, cid));
				inChunk->makeAllBlocks();
				inChunk->setLevel(wtLevel);

				auto cDesc = inChunk->getDesc();
				auto outChunk = std::static_pointer_cast<wtChunk<Ty_>>(outArr->makeChunk(attrDesc->id_, cid));
				//outChunk->makeAllBlocks();
				outChunk->setLevel(wtLevel);
				// make block in decompressChunk()

				//auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
				//auto outChunk = outArr->makeChunk(*inChunk->getDesc());
				//auto wtOutChunk = std::static_pointer_cast<wtChunk>(outChunk);
				//wtOutChunk->setLevel(outArr->getMaxLevel());

				io_service_->post(boost::bind(&spiht_decode_action::decompressChunk<Ty_>, this,
											  inChunk, outChunk, qry, outArr->getId(), attrId, currentThreadId));
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

	template <typename Ty_>
	void decompressChunk(std::shared_ptr<spihtChunk<Ty_>> inChunk, std::shared_ptr<wtChunk<Ty_>> outChunk, pQuery qry,
						 const arrayId arrId, const attributeId attrId, const size_t parentThreadId)
	{
		auto threadId = getThreadId();

		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
		//----------------------------------------//

		pSerializable serialChunk
			= std::static_pointer_cast<serializable>(inChunk);

		storageMgr::instance()->loadChunk(arrId, attrId, inChunk->getId(), serialChunk);

		//----------------------------------------//
		qry->getTimer()->nextWork(threadId, workType::COMPUTING);
		//----------------------------------------//

		//outChunk->setLevel(inChunk->getLevel());
		outChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
		outChunk->makeBlocks();
		outChunk->bufferCopy(inChunk);	// If ref bitmap from inChunk, it might be deleted in 'inChunk' when it destroied.
		outChunk->setSerializedSize(inChunk->getSerializedSize());

		//----------------------------------------//
		qry->getTimer()->pause(threadId);
		//========================================//
	}
};
}		// core
}		// msdb
#endif // _MSDB_OP_SPIHT_DECODE_ACTION_H
