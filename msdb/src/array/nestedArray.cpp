#include <pch.h>
#include <array/nestedArray.h>
#include <array/nestedChunk.h>

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
void nestedArray::initChunkFactories()
{
	nestedChunkFactoryBuilder ncfc;

	for (auto attrDesc : *this->getDesc()->getAttrDescs())
	{
		assert(attrDesc->id_ == this->cFactories_.size());
		this->cFactories_.push_back(std::visit(ncfc, attrDesc->dataType_));
	}
}

//pChunk nestedArray::makeChunk(const pChunkDesc desc)
//{
//	auto chunkObj = this->getChunkFactory(desc->attrDesc_->id_)->requestNewChunk(desc);
//	this->insertChunk(chunkObj);
//
//	return chunkObj;
//}
}		// core
}		// msdb