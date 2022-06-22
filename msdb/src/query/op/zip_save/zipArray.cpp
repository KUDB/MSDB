#include <pch.h>
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