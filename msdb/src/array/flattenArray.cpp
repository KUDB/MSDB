#include <pch.h>
#include <array/flattenArray.h>
#include <array/flattenChunk.h>

namespace msdb
{
namespace core
{
flattenArray::flattenArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

flattenArray::~flattenArray()
{

}

pArray flattenArray::shallowClone(const bool takeOwnership)
{
	auto outDesc = std::make_shared<arrayDesc>(*this->getDesc());
	pArray outArr = std::make_shared<flattenArray>(outDesc);

	outArr->shallowChunkCopy(*this, takeOwnership);

	return outArr;
}

void flattenArray::initChunkFactories()
{
	flattenChunkFactoryBuilder fcfc;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fcfc, attrDesc->dataType_));
	}
}
}		// core
}		// msdb