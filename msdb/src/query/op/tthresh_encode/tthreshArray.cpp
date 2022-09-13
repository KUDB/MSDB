#include <pch.h>
#include <op/tthresh_encode/tthreshArray.h>
#include <op/tthresh_encode/tthreshChunk.h>

namespace msdb
{
namespace core
{
tthreshArray::tthreshArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

tthreshArray::~tthreshArray()
{

}

pArray tthreshArray::shallowClone(const bool takeOwnership)
{
	auto outDesc = std::make_shared<arrayDesc>(*this->getDesc());
	pArray outArr = std::make_shared<tthreshArray>(outDesc);

	outArr->shallowChunkCopy(*this, takeOwnership);

	return outArr;
}

void tthreshArray::initChunkFactories()
{
	tthreshChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb