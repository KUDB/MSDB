#include <pch.h>
#include <op/adapt_huffman_encode/adapt_huffman_encode_action.h>
#include <system/storageMgr.h>
#include <compression/adaptHuffmanChunk.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
adapt_huffman_encode_action::adapt_huffman_encode_action()
{
}
adapt_huffman_encode_action::~adapt_huffman_encode_action()
{
}
const char* adapt_huffman_encode_action::name()
{
	return "adapt_huffman_encode_action";
}
pArray adapt_huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	size_t mSizeTotal = 0;
	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(iterateMode::EXIST);
		while (!cit->isEnd())
		{
			auto outChunk = this->makeOutChunk((**cit));

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
			++(*cit);
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	qry->getTimer()->pause(0);
	//========================================//

	return sourceArr;
}
pAdaptHuffmanChunk adapt_huffman_encode_action::makeOutChunk(pChunk inChunk)
{
	auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
	pAdaptHuffmanChunk outChunk = std::make_shared<adaptHuffmanChunk>(outChunkDesc);
	outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
	outChunk->makeBlocks();
	outChunk->bufferRef(inChunk);

	return outChunk;
}
}		// core
}		// msdb