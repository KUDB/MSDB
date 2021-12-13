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
	for (auto attr : *this->getDesc()->attrDescs_)
	{
		auto cit = this->getChunkIterator(attr->id_);
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				this->freeChunk(attr->id_, cit->seqPos());
			}
			++(*cit);
		}
		this->chunks_[attr->id_].clear();
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
void memBlockArray::freeChunk(const attributeId attrId, const chunkId cId)
{
	assert(this->chunks_.find(attrId) != this->chunks_.end());
	if (this->chunks_[attrId][cId] != nullptr)
	{
		this->chunks_[attrId][cId]->flush();
	}
	this->setChunkNull(attrId, cId);
	//this->chunks_[cId] = nullptr;
	//this->overallChunkBitmap_->setNull(cId);
}
}		// core
}		// msdb