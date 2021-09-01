#include <pch.h>
#include <array/memBlockArray.h>
#include <array/blockChunk.h>

namespace msdb
{
namespace core
{
memBlockArray::memBlockArray(pArrayDesc desc)
	: base_type(desc)
{
}
memBlockArray::~memBlockArray()
{
	auto cit = this->getChunkIterator();
	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			this->freeChunk(cit->seqPos());
		}
		++(*cit);
	}
	this->chunks_.clear();
}
pChunk memBlockArray::makeChunk(const attributeId attrId, const chunkId cId)
{
	auto desc = this->getChunkDesc(attrId, cId);
	auto chunkObj = std::make_shared<memBlockChunk>(desc);
	this->insertChunk(attrId, chunkObj);
	return chunkObj;
}
pChunk memBlockArray::makeChunk(const chunkDesc& desc)
{
	auto chunkObj 
		= std::make_shared<memBlockChunk>(std::make_shared<chunkDesc>(desc));
	this->insertChunk(desc.attrDesc_->id_, chunkObj);
	return chunkObj;
}
void memBlockArray::freeChunk(const chunkId cId)
{
	if (this->chunks_[cId] != nullptr)
	{
		this->chunks_[cId]->flush();
	}
	this->chunks_[cId] = nullptr;
	this->chunkBitmap_->setNull(cId);
}
}		// core
}		// msdb