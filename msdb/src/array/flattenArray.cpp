#include <pch.h>
#include <array/flattenArray.h>
#include <array/blockChunk.h>

namespace msdb
{
namespace core
{
flattenArray::flattenArray(pArrayDesc desc)
	: base_type(desc)
{
}

flattenArray::~flattenArray()
{
	this->flush();
}

pChunk flattenArray::makeChunk(const attributeId attrId, const chunkId cId)
{
	auto desc = this->getChunkDesc(attrId, cId);
	auto chunkObj = std::make_shared<memBlockChunk>(desc);
	this->insertChunk(attrId, chunkObj);
	return chunkObj;
}
pChunk flattenArray::makeChunk(const chunkDesc& desc)
{
	auto chunkObj
		= std::make_shared<memBlockChunk>(std::make_shared<chunkDesc>(desc));
	this->insertChunk(desc.attrDesc_->id_, chunkObj);
	return chunkObj;
}
}		// core
}		// msdb