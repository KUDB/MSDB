#include <pch.h>
#include "seHuffmanArray.h"
#include "seHuffmanChunk.h"

namespace msdb
{
namespace core
{
seHuffmanArray::seHuffmanArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

seHuffmanArray::~seHuffmanArray()
{

}

void seHuffmanArray::initChunkFactories()
{
	seHuffmanChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb