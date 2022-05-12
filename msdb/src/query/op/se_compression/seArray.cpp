#include <pch.h>
#include "seArray.h"
#include "seChunk.h"

namespace msdb
{
namespace core
{
seArray::seArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

seArray::~seArray()
{

}

void seArray::initChunkFactories()
{
	seChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb