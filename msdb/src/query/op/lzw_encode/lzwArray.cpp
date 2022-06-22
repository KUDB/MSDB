#include <pch.h>
#include "lzwArray.h"
#include "lzwChunk.h"

namespace msdb
{
namespace core
{
lzwArray::lzwArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

lzwArray::~lzwArray()
{

}

void lzwArray::initChunkFactories()
{
	lzwChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb