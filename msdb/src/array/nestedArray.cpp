#include <pch.h>
#include <array/nestedArray.h>
#include <array/nestedArrayChunk.h>

namespace msdb
{
namespace core
{
nestedArray::nestedArray(pArrayDesc desc)
	: base_type(desc)
{
}

nestedArray::~nestedArray()
{
	
}
pChunk nestedArray::makeChunk(const attributeId attrId, const chunkId cId)
{
	auto desc = this->getChunkDesc(attrId, cId);
	auto chunkObj = std::make_shared<nestedArrayChunk>(desc);
	this->insertChunk(attrId, chunkObj);
	return chunkObj;
}
pChunk nestedArray::makeChunk(const chunkDesc& desc)
{
	auto chunkObj
		= std::make_shared<nestedArrayChunk>(std::make_shared<chunkDesc>(desc));
	this->insertChunk(desc.attrDesc_->id_, chunkObj);
	return chunkObj;
}
}		// core
}		// msdb