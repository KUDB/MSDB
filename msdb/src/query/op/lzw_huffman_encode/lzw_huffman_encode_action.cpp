#include <pch.h>
#include <op/lzw_huffman_encode/lzw_huffman_encode_action.h>
#include <system/storageMgr.h>
#include <compression/lzwHuffmanChunk.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
lzw_huffman_encode_action::lzw_huffman_encode_action()
{
}
lzw_huffman_encode_action::~lzw_huffman_encode_action()
{
}
const char* lzw_huffman_encode_action::name()
{
	return "lzw_huffman_encode_action";
}
pArray lzw_huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
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
pLzwHuffmanChunk lzw_huffman_encode_action::makeOutChunk(pChunk inChunk)
{
	auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
	pLzwHuffmanChunk outChunk = std::make_shared<lzwHuffmanChunk>(outChunkDesc);
	outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
	outChunk->makeBlocks();
	outChunk->bufferRef(inChunk);

	return outChunk;
}
}		// core
}		// msdb