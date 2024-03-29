﻿#include <pch.h>
#include <query/opIOAction.h>
#include <util/threadUtil.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
opIOAction::opIOAction()
{
}
opIOAction::~opIOAction()
{
}

pArray opIOAction::executeIO(std::vector<pArray>& inputArrays, pQuery qry, const IO_TYPE type)
{
	if (inputArrays.size() != 1)
	{
		this->throwExceptionWrongInputArray(inputArrays.size());
	}

	size_t curThreadId = 0;
	//========================================//
	qry->getTimer()->nextJob(curThreadId, this->name(), workType::COMPUTING);
	//----------------------------------------//

	pArray inArray = inputArrays[0];
	arrayId arrId = inArray->getId();
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArray->getDesc());
	pArray outArray = this->getOutArray(outArrDesc);

	switch (type)
	{
	case IO_TYPE::SAVE:
	{
		auto attrDescs = inArray->getDesc()->attrDescs_;
		for (auto attr : *attrDescs)
		{
			if (attr->getCompType() != this->encodingType())
			{
				continue;
			}
			this->saveAttribute(inArray, outArray, attr, qry, curThreadId);
		}
		qry->setIOOperator();
		break;
	}
	case IO_TYPE::LOAD:
	{
		// In load operators, the operators should know the list of chunks to be loaded.
		// Copies the inferred chunk bitmap from OperatorPlans.
		outArray->copyChunkBitmap(this->getPlanInChunkBitmap());

		auto attrDescs = inArray->getDesc()->attrDescs_;
		for (auto attr : *attrDescs)
		{
			if (attr->getCompType() != this->encodingType())
			{
				continue;
			}
			this->loadAttribute(outArray, attr, qry, curThreadId);
		}
		break;
	}
	default:
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED, "opIOAction::executeIO: unknown IO_TYPE"));
	}
	}
	//----------------------------------------//
	qry->getTimer()->pause(curThreadId);
	//========================================//

	size_t mSizeTotal = 0;
	for (auto attrDesc : *outArray->getDesc()->attrDescs_)
	{
		auto ocit = outArray->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		while (!ocit->isEnd())
		{
			if (ocit->isExist())
			{
				auto outChunk = (**ocit);
				mSizeTotal += outChunk->getSerializedSize();
			}

			++(*ocit);
		}
	}
	qry->setIOBytes(mSizeTotal);

	this->getArrayStatus(outArray);
	return outArray;
}

void opIOAction::saveAttribute(pArray inArr, pArray outArr, pAttributeDesc attrDesc, pQuery qry, const size_t curThreadId)
{
	//========================================//
	qry->getTimer()->nextWork(curThreadId, workType::PARALLEL);
	//----------------------------------------//
#ifdef NDEBUG
	this->threadCreate();
#endif

	auto cit = inArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			auto cDesc = std::make_shared<chunkDesc>(*(*cit)->getDesc());
			auto outChunk = outArr->makeChunk(cDesc);
			outChunk->bufferCopy(**cit);
			outChunk->makeAllBlocks();

			////////////////////////////////////////
			// 1. Serialize::saveChunk
			////////////////////////////////////////
		#ifndef NDEBUG
			this->saveChunk(outArr->getId(), attrDesc->getId(), outChunk, qry, curThreadId);
		#endif
			////////////////////////////////////////

			////////////////////////////////////////
			// 2. Parallel::saveChunk
			////////////////////////////////////////
		#ifdef NDEBUG
			io_service_->post(boost::bind(&opIOAction::saveChunk, this,
										  outArr->getId(), attrDesc->getId(), outChunk, qry, curThreadId));
		#endif
			////////////////////////////////////////
		}

		++(*cit);
	}

#ifdef NDEBUG
	this->threadStop();
	this->threadJoin();
#endif

	//----------------------------------------//
	qry->getTimer()->nextWork(curThreadId, workType::COMPUTING);
	//========================================//
}

void opIOAction::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry, const size_t curThreadId)
{
	//========================================//
	qry->getTimer()->nextWork(curThreadId, workType::PARALLEL);
	//----------------------------------------//
#ifdef NDEBUG
	this->threadCreate();
#endif

	auto cit = outArr->getChunkIterator(attrDesc->id_, iterateMode::ALL);
	while (!cit->isEnd())
	{
		if (cit->needToMake())
		{
			chunkId cid = cit->seqPos();
			auto outChunk = outArr->makeChunk(attrDesc->id_, cid);
			outChunk->makeAllBlocks();

			////////////////////////////////////////
			// 1. Serialize::loadChunk
			////////////////////////////////////////
		#ifndef NDEBUG
			this->loadChunk(outArr->getId(), attrDesc->getId(), outChunk, qry, curThreadId);
		#endif
			////////////////////////////////////////

			////////////////////////////////////////
			// 2. Parallel::loadChunk
			////////////////////////////////////////
		#ifdef NDEBUG
			io_service_->post(boost::bind(&opIOAction::loadChunk, this,
										  outArr->getId(), attrDesc->getId(), outChunk, qry, curThreadId));
		#endif
			////////////////////////////////////////
		}

		++(*cit);
	}

#ifdef NDEBUG
	this->threadStop();
	this->threadJoin();
#endif

	//----------------------------------------//
	qry->getTimer()->nextWork(curThreadId, workType::COMPUTING);
	//========================================//
}

void opIOAction::saveChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId)
{
	auto threadId = getThreadId() + 1;
	//========================================//
	qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::IO,
							 std::string("chunk::") + std::to_string(outChunk->getId()));
	//----------------------------------------//

	storageMgr::instance()->saveChunk(arrId, attrId, outChunk->getId(), outChunk);

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}

void opIOAction::loadChunk(const arrayId arrId, const attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId)
{
	auto threadId = getThreadId() + 1;
	//========================================//
	qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::IO, 
							 std::string("chunk::") + std::to_string(outChunk->getId()));
	//----------------------------------------//

	storageMgr::instance()->loadChunk(arrId, attrId, outChunk->getId(), outChunk);

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}

size_t opIOAction::getSerializedChunkSize(attributeId attrId, pArray arr)
{
	size_t chunkSize = 0;
	auto cit = arr->getChunkIterator(attrId, iterateMode::EXIST);
	while (!cit->isEnd())
	{
		auto outChunk = (**cit);
		chunkSize += outChunk->getSerializedSize();

		++(*cit);
	}

	return chunkSize;
}
void opIOAction::throwExceptionWrongInputArray(const size_t actual, const size_t expect)
{
	std::stringstream ss;
	ss << "Wrong InputArray size for " << this->name() << " (Expected: " << expect << ", Actual: " << actual;
	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT, ss.str()));
}
}		// core
}		// msdb