#include <pch_op.h>
#include <op/load/load_action.h>
#include <array/flattenArray.h>
#include <array/flattenChunk.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
load_action::load_action()
{
}
load_action::~load_action()
{
}
const char* load_action::name()
{
	return "load";
}
pArray load_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	assert(inputArrays.size() == 1);
	auto planChunkBitmap = this->getPlanInChunkBitmap();

	//pArray outArr = arrayMgr::instance()->makeArray<flattenArray>(this->getArrayDesc());
	pArray outArr = std::make_shared<flattenArray>(this->getArrayDesc());
	arrayId arrId = outArr->getId();

	for (auto attr : *outArr->getDesc()->attrDescs_)
	{
		outArr->copyAttrChunkBitmap(attr->getId(), planChunkBitmap);
		this->loadAttribute(outArr, attr, qry);
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	size_t mSizeTotal = 0;
	for (auto attrDesc : *outArr->getDesc()->attrDescs_)
	{
		auto ocit = outArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		while (!ocit->isEnd())
		{
			auto outChunk = (**ocit);
			mSizeTotal += outChunk->getSerializedSize();

			++(*ocit);
		}
	}
	qry->setIOBytes(mSizeTotal);

	//========================================//
	this->getArrayStatus(outArr);

	return outArr;
}

void load_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
{
	size_t currentThreadId = 0;

	//========================================//
	qry->getTimer()->nextWork(0, workType::PARALLEL);
	//----------------------------------------//
	
	auto cit = outArr->getChunkIterator(attrDesc->id_, iterateMode::ALL);
	auto cBitmap = cit->getChunkBitmap();

	this->threadCreate();

	while (!cit->isEnd())
	{
		chunkId cid = cit->seqPos();
		if (cBitmap->isExist(cid))
		{
			chunkId cId = cit->seqPos();
			auto outChunk = outArr->makeChunk(attrDesc->id_, cId);

			auto blockBitmap = this->getPlanBlockBitmap(cId);
			if (blockBitmap)
			{
				outChunk->copyBlockBitmap(blockBitmap);
			}
			else
			{
				// If there were no bitmap, set all blocks as true.
				outChunk->replaceBlockBitmap(std::make_shared<bitmap>(outChunk->getBlockCapacity(), true));
			}
			outChunk->makeBlocks();

			this->loadChunk(outArr, outChunk, attrDesc->id_, qry, currentThreadId);
			//io_service_->post(boost::bind(&load_action::loadChunk, this,
			//								outArr, outChunk, attrDesc->id_, qry, currentThreadId));
		}

		++(*cit);
	}

	this->threadStop();
	this->threadJoin();

	//----------------------------------------//
	qry->getTimer()->nextWork(0, workType::COMPUTING);
	//========================================//
}

void load_action::loadChunk(pArray outArr, pChunk outChunk, attributeId attrId, pQuery qry, const size_t parentThreadId)
{
	auto threadId = getThreadId();

	//========================================//
	qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
	//----------------------------------------//

	_MSDB_TRY_BEGIN
	{
		pSerializable serialChunk
		= std::static_pointer_cast<serializable>(outChunk);
		storageMgr::instance()->loadChunk(outArr->getId(), attrId, outChunk->getId(),
											serialChunk);
	}
	_MSDB_CATCH_EXCEPTION(e)
	{
		if (outChunk != nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "Error in 'Load chunk' (attrId: " << attrId << "chunkId: " << outChunk->getId() << e.what();
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "Error in 'Load chunk' (attrId: " << attrId << "chunkId: nullptr" << e.what();
		}
	}
	_MSDB_CATCH_ALL
	{
		if (outChunk != nullptr)
		{
			BOOST_LOG_TRIVIAL(error) << "Error in 'Load chunk' (attrId: " << attrId << "chunkId: " << outChunk->getId();
		}
		else
		{
			BOOST_LOG_TRIVIAL(error) << "Error in 'Load chunk' (attrId: " << attrId << "chunkId: nullptr";
		}
	}
	_MSDB_CATCH_END

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}
}		// core
}		// msdb