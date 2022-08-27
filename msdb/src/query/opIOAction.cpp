#include <pch.h>
#include <query/opIOAction.h>
#include <util/threadUtil.h>
#include <system/storageMgr.h>

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

void opIOAction::encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId)
{
	auto threadId = getThreadId() + 1;
	//========================================//
	qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::IO, std::string("chunk::") + std::to_string(outChunk->getId()));
	//----------------------------------------//

	storageMgr::instance()->saveChunk(arrId, attrId, outChunk->getId(), outChunk);

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
}		// core
}		// msdb