#include <pch.h>
#include <op/se_huffman_encode/se_huffman_decode_action.h>
#include <array/flattenArray.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>
#include "seHuffmanArray.h"

namespace msdb
{
namespace core
{
se_huffman_decode_action::se_huffman_decode_action()
{
}
se_huffman_decode_action::~se_huffman_decode_action()
{
}
const char* se_huffman_decode_action::name()
{
	return "se_huffman_decode_action";
}
pArray se_huffman_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::ARRAY_CONSTRUCTING);
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

	pArray inArr = std::make_shared<seHuffmanArray>(std::make_shared<arrayDesc>(*arrDesc));
	inArr->copyChunkBitmap(planBitmap);

	auto outArr = std::make_shared<wavelet_encode_array>(std::make_shared<arrayDesc>(*arrDesc));
	//outArr->setLevel(maxLevel);
	outArr->setOrigianlChunkDims(originalChunkDims);
	outArr->copyChunkBitmap(planBitmap);

	auto attrDescs = inArr->getDesc()->getAttrDescs();
	for (auto attr : *attrDescs)
	{
		if (attr->getCompType() != compressionType::SEACOW_HUFFMAN)
		{
			continue;
		}

		inArr->makeChunks(attr->getId(), *planBitmap);

		std::visit(
			visitHelper
			{
				[this, &outArr, &inArr, &attr, &qry](const auto& vType)
				{
					decompressAttribute(vType, outArr, inArr, attr, qry);
				}
			},
			attr->getDataType());
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return std::static_pointer_cast<array>(outArr);;
}
}		// core
}		// msdb