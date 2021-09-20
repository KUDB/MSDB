#include <pch.h>
#include <op/compass_encode/compass_encode_action.h>
#include <system/storageMgr.h>
#include <compression/compassChunk.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
compass_encode_action::compass_encode_action()
{
}
compass_encode_action::~compass_encode_action()
{
}
const char* compass_encode_action::name()
{
	return "compass_encode_action";
}
pArray compass_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	size_t mSizeTotal = 0;
	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	// Get Parameter - NumBin
	eleDefault numBins;
	std::static_pointer_cast<stableElement>(this->params_[1]->getParam())->getData(&numBins);

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(iterateMode::EXIST);
		while (!cit->isEnd())
		{
			pChunk inChunk = (**cit);
			auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
			pCompassChunk outChunk = std::make_shared<compassChunk>(outChunkDesc);
			outChunk->setNumBins(numBins);
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
			++(*cit);
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return sourceArr;
}
}		// core
}		// msdb