#include <pch.h>
#include <array/memArray.h>
#include <array/memChunk.h>

namespace msdb
{
namespace core
{
memArray::memArray(pArrayDesc desc)
	: base_type(desc)
{
}

memArray::~memArray()
{
}
pChunk memArray::makeChunk(const attributeId attrId, const chunkId cId)
{
	auto desc = this->getChunkDesc(attrId, cId);
	auto chunkObj = std::make_shared<memChunk>(desc);
	this->insertChunk(attrId, chunkObj);
	return chunkObj;
}
pChunk memArray::makeChunk(const chunkDesc& desc)
{
	auto chunkObj
		= std::make_shared<memChunk>(std::make_shared<chunkDesc>(desc));
	this->insertChunk(desc.attrDesc_->id_, chunkObj);
	return chunkObj;
}
void memArray::freeChunk(const chunkId cId)
{
	if(this->chunks_[cId] != nullptr)
	{
		this->chunks_[cId]->flush();
	}
	this->chunks_[cId] = nullptr;
	this->chunkBitmap_->setNull(cId);
}
}		// core
}		// msdb