#include <pch_op.h>
#include "zipArray.h"
#include "zipChunk.h"

namespace msdb
{
namespace core
{
zipArray::zipArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

zipArray::~zipArray()
{

}

pArray zipArray::shallowClone(const bool takeOwnership)
{
	auto outDesc = std::make_shared<arrayDesc>(*this->getDesc());
	pArray outArr = std::make_shared<zipArray>(outDesc);

	outArr->shallowChunkCopy(*this, takeOwnership);

	return outArr;
}

void zipArray::initChunkFactories()
{
	zipChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb