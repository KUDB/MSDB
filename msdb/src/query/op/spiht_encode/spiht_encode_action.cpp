#include <pch.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/memChunk.h>
#include <compression/spihtChunk.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
spiht_encode_action::spiht_encode_action()
{
}

spiht_encode_action::~spiht_encode_action()
{
}

const char* spiht_encode_action::name()
{
	return "spiht_encode_action";
}

pArray spiht_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	size_t mSizeTotal = 0;
	pArray sourceArr = inputArrays[0];
	auto wArray = std::static_pointer_cast<wavelet_encode_array>(sourceArr);
	arrayId arrId = sourceArr->getId();

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(iterateMode::EXIST);
		while (!cit->isEnd())
		{
			pChunk inChunk = (**cit);
			auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
			pSpihtChunk outChunk = std::make_shared<spihtChunk>(outChunkDesc);
			outChunk->setLevel(wArray->getMaxLevel());
			outChunk->makeAllBlocks();
			outChunk->bufferRef(inChunk);

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
			//std::cout << serialChunk->getSerializedSize() << std::endl;
			++(*cit);
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	qry->getTimer()->pause(0);
	//========================================//

	return sourceArr;
}
}		// core
}		// msdb