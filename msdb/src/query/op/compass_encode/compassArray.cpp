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