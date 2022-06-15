#include <pch.h>
#include <op/huffman_encode/huffman_encode_action.h>
#include <system/storageMgr.h>
#include <op/huffman_encode/huffmanChunk.h>
#include <util/logger.h>
#include "huffmanArray.h"

namespace msdb
{
namespace core
{
huffman_encode_action::huffman_encode_action()
{
}
huffman_encode_action::~huffman_encode_action()
{
}
const char* huffman_encode_action::name()
{
	return "huffman_encode_action";
}
pArray huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	size_t mSizeTotal = 0;
	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
	pArray outArr = std::make_shared<huffmanArray>(outArrDesc);

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(attr->id_, iterateMode::EXIST);
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto cDesc = std::make_shared<chunkDesc>(*(*cit)->getDesc());
				auto outChunk = outArr->makeChunk(cDesc);
				outChunk->bufferCopy(**cit);
				outChunk->makeAllBlocks();

				//========================================//
				qry->getTimer()->nextWork(0, workType::IO);
				//----------------------------------------//
				pSerializable serialChunk
					= std::static_pointer_cast<serializable>(outChunk);
				storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
												  serialChunk);

				//========================================//
				qry->getTimer()->nextWork(0, workType::COMPUTING);
				//----------------------------------------//
				mSizeTotal += serialChunk->getSerializedSize();
			}

			++(*cit);
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	qry->getTimer()->pause(0);
	//========================================//

	return sourceArr;
}
//pHuffmanChunk huffman_encode_action::makeOutChunk(pChunk inChunk)
//{
//	auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
//	pHuffmanChunk outChunk = std::make_shared<huffmanChunk>(outChunkDesc);
//	outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
//	outChunk->makeBlocks();
//	outChunk->bufferRef(inChunk);
//
//	return outChunk;
//}
}	// core
}	// msdb