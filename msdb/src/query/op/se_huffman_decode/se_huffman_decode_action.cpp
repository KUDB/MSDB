#include <pch.h>
#include <op/se_huffman_decode/se_huffman_decode_action.h>
#include <array/memBlockArray.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>

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

	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);

	auto planBitmap = this->getPlanInChunkBitmap();
	auto arrDesc = this->getArrayDesc();
	dimension originalChunkDims = arrDesc->getDimDescs()->getChunkDims();
	for (dimensionId d = 0; d < arrDesc->getDSize(); ++d)
	{
		arrDesc->getDimDescs()->at(d)->chunkSize_ = arrDesc->getDimDescs()->at(d)->blockSize_;
	}

	auto outArr = std::make_shared<wavelet_encode_array>(arrDesc);
	outArr->setLevel(maxLevel);
	outArr->setOrigianlChunkDims(originalChunkDims);
	outArr->copyChunkBitmap(planBitmap);

	for (auto attrDesc : *outArr->getDesc()->attrDescs_)
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			decompressAttribute<char>(outArr, attrDesc, qry);
			break;
		case eleType::INT8:
			decompressAttribute<int8_t>(outArr, attrDesc, qry);
			break;
		case eleType::INT16:
			decompressAttribute<int16_t>(outArr, attrDesc, qry);
			break;
		case eleType::INT32:
			decompressAttribute<int32_t>(outArr, attrDesc, qry);
			break;
		case eleType::INT64:
			decompressAttribute<int64_t>(outArr, attrDesc, qry);
			break;
		case eleType::UINT8:
			decompressAttribute<int8_t>(outArr, attrDesc, qry);
			break;
		case eleType::UINT16:
			decompressAttribute<int16_t>(outArr, attrDesc, qry);
			break;
		case eleType::UINT32:
			decompressAttribute<int32_t>(outArr, attrDesc, qry);
			break;
		case eleType::UINT64:
			decompressAttribute<int64_t>(outArr, attrDesc, qry);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return std::static_pointer_cast<arrayBase>(outArr);;
}

pSeHuffmanChunk se_huffman_decode_action::makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc,
													  chunkId cid, coor chunkCoor)
{
	dimension chunkDims = arr->getDesc()->getDimDescs()->getChunkDims();
	dimension blockDims = arr->getDesc()->getDimDescs()->getBlockDims();

	coor sp = chunkDims * chunkCoor;
	coor ep = sp + chunkDims;

	chunkSize mSize = chunkDims.area() * attrDesc->typeSize_;

	dimension originalSourceDims = arr->getOrigianlChunkDims();

	auto outDesc = std::make_shared<chunkDesc>(cid, std::make_shared<attributeDesc>(*attrDesc),
											   chunkDims, blockDims,
											   sp, ep, mSize);
	pSeHuffmanChunk inChunk = std::make_shared<seHuffmanChunk>(outDesc);
	auto blockBitmap = this->getPlanBlockBitmap(cid);
	if (blockBitmap)
	{
		inChunk->copyBlockBitmap(blockBitmap);
	}
	else
	{
		// If there were no bitmap, set all blocks as true.
		inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
	}

	inChunk->setLevel(arr->getMaxLevel());
	inChunk->makeBlocks();

	return inChunk;
}
}		// core
}		// msdb