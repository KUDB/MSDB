#include <pch.h>
#include <op/lzw_encode/lzw_encode_action.h>
#include <system/storageMgr.h>
#include <op/lzw_encode/lzwChunk.h>
#include "lzwArray.h"
#include <util/logger.h>

namespace msdb
{
namespace core
{
lzw_encode_action::lzw_encode_action()
{
}
lzw_encode_action::~lzw_encode_action()
{
}
const char* lzw_encode_action::name()
{
	return "lzw_encode_action";
}
pArray lzw_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	size_t mSizeTotal = 0;
	pArray inArr = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArr->getDesc());
	pArray outArr = std::make_shared<lzwArray>(outArrDesc);

	arrayId arrId = inArr->getId();

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		if (attr->getCompType() != compressionType::LZW)
		{
			continue;
		}

		auto cit = inArr->getChunkIterator(attr->id_, iterateMode::EXIST);
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

		//auto cit = inArr->getChunkIterator(attr->id_, iterateMode::EXIST);
		//while (!cit->isEnd())
		//{
		//	auto outChunk = this->makeOutChunk((**cit));

		//	//========================================//
		//	qry->getTimer()->nextWork(0, workType::IO);
		//	//----------------------------------------//
		//	pSerializable serialChunk
		//		= std::static_pointer_cast<serializable>(outChunk);
		//	storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
		//									  serialChunk);

		//								  //========================================//
		//	qry->getTimer()->nextWork(0, workType::COMPUTING);
		//	//----------------------------------------//
		//	mSizeTotal += serialChunk->getSerializedSize();
		//	++(*cit);
		//}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	qry->getTimer()->pause(0);
	//========================================//

	return inArr;
}
//pLzwChunk lzw_encode_action::makeOutChunk(pChunk inChunk)
//{
//	auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
//	pLzwChunk outChunk = std::make_shared<lzwChunk>(outChunkDesc);
//	outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
//	outChunk->makeBlocks();
//	outChunk->bufferRef(inChunk);
//
//	return outChunk;
//}
}		// core
}		// msdb