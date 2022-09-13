#include <pch.h>
#include <op/huffman_encode/huffmanArray.h>
#include <op/huffman_encode/huffmanChunk.h>

namespace msdb
{
namespace core
{
huffmanArray::huffmanArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

huffmanArray::~huffmanArray()
{

}

pArray huffmanArray::shallowClone(const bool takeOwnership)
{
	auto outDesc = std::make_shared<arrayDesc>(*this->getDesc());
	pArray outArr = std::make_shared<huffmanArray>(outDesc);

	outArr->shallowChunkCopy(*this, takeOwnership);

	return outArr;
}

void huffmanArray::initChunkFactories()
{
	huffmanChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb