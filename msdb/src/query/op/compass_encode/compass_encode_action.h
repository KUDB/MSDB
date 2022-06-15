#pragma once
#ifndef _MSDB_OP_COMPASS_ENCODE_ACTION_H_
#define _MSDB_OP_COMPASS_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <util/math.h>
#include <system/storageMgr.h>
#include "compassChunk.h"

namespace msdb
{
namespace core
{
class compass_encode_action : public opAction
{
public:
	compass_encode_action();
	virtual ~compass_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

	template <typename Ty_>
	void compressAttribute(const concreteTy<Ty_>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc, pQuery qry, size_t numBins)
	{
		size_t mSizeTotal = 0;
		arrayId arrId = inArr->getId();

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
				auto cChunk = std::static_pointer_cast<compassChunk<Ty_>>(outChunk);
				cChunk->setNumBins(numBins);

				//pChunk inChunk = (**cit);
				//auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
				//pCompassChunk outChunk = std::make_shared<compassChunk>(outChunkDesc);
				//outChunk->setNumBins(numBins);
				//outChunk->makeAllBlocks();
				//outChunk->bufferRef(inChunk);

				//========================================//
				qry->getTimer()->nextWork(0, workType::IO);
				//----------------------------------------//
				pSerializable serialChunk
					= std::static_pointer_cast<serializable>(outChunk);
				storageMgr::instance()->saveChunk(arrId, attrDesc->id_, (outChunk)->getId(),
												  serialChunk);

				//========================================//
				qry->getTimer()->nextWork(0, workType::COMPUTING);
				//----------------------------------------//
				mSizeTotal += serialChunk->getSerializedSize();
			}

			++(*cit);
		}

		BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	}
};
}		// core
}		// msdb
#endif	// _MSDB_OP_COMPASS_ENCODE_ACTION_H_