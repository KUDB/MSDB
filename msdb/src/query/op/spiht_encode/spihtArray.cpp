#include <pch.h>
#include <op/spiht_encode/spihtArray.h>
#include <op/spiht_encode/spihtChunk.h>

namespace msdb
{
namespace core
{
spihtArray::spihtArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

spihtArray::~spihtArray()
{

}

void spihtArray::initChunkFactories()
{
	spihtChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb