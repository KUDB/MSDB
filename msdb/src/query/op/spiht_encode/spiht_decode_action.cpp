#include <pch.h>
#include <op/spiht_encode/spiht_decode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <array/monoChunk.h>
#include <util/threadUtil.h>
#include "spihtArray.h"

namespace msdb
{
namespace core
{
spiht_decode_action::spiht_decode_action()
{
}

spiht_decode_action::~spiht_decode_action()
{
}

const char* spiht_decode_action::name()
{
	return "spiht_decode_action";
}

pArray spiht_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	//pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	//eleDefault maxLevel;
	//ele->getData(&maxLevel);

	auto planBitmap = this->getPlanInChunkBitmap();
	auto arrDesc = this->getArrayDesc();
	dimension originalChunkDims = arrDesc->getDimDescs()->getChunkDims();
	for (dimensionId d = 0; d < arrDesc->getDSize(); ++d)
	{
		arrDesc->getDimDescs()->at(d)->chunkSize_ = arrDesc->getDimDescs()->at(d)->blockSize_;
	}

	pArray inArr = std::make_shared<spihtArray>(std::make_shared<arrayDesc>(*arrDesc));
	inArr->copyChunkBitmap(planBitmap);

	auto outArr = std::make_shared<wavelet_encode_array>(std::make_shared<arrayDesc>(*arrDesc));
	outArr->copyChunkBitmap(planBitmap);
	outArr->setOrigianlChunkDims(originalChunkDims);

	auto attrDescs = inArr->getDesc()->getAttrDescs();
	for (auto attr : *attrDescs)
	{
		if (attr->getCompType() != encodingType::SPIHT)
		{
			continue;
		}

		//this->decodeAttribute(outArr, inArr, attr, qry);
		std::visit(
			visitHelper
			{
				[this, &outArr, &inArr, &attr, &qry](const auto& vType)
				{
					decodeAttribute(vType, outArr, inArr, attr, qry);
				}
			},
			attr->getDataType());
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

//pSpihtChunk spiht_decode_action::makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc, chunkId cid)
//{
//	auto inChunk = std::make_shared<spihtChunk>(arr->getChunkDesc(attrDesc->id_, cid));
//	auto blockBitmap = this->getPlanBlockBitmap(cid);
//	if (blockBitmap)
//	{
//		inChunk->copyBlockBitmap(blockBitmap);
//	} else
//	{
//		// If there were no bitmap, set all blocks as true.
//		inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
//	}
//
//	inChunk->setLevel(arr->getMaxLevel());
//	inChunk->makeBlocks();
//
//	return inChunk;
//}
}		// core
}		// msdb