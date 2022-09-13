#include <pch.h>
#include "compassArray.h"
#include "compassChunk.h"

namespace msdb
{
namespace core
{
compassArray::compassArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

compassArray::~compassArray()
{

}

pArray compassArray::shallowClone(const bool takeOwnership)
{
	auto outDesc = std::make_shared<arrayDesc>(*this->getDesc());
	pArray outArr = std::make_shared<compassArray>(outDesc);

	outArr->shallowChunkCopy(*this, takeOwnership);

	return outArr;
}

void compassArray::initChunkFactories()
{
	compassChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb