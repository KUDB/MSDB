#include <pch.h>
#include <op/lzw_huffman_encode/lzwHuffmanArray.h>
#include <op/lzw_huffman_encode/lzwHuffmanChunk.h>

namespace msdb
{
namespace core
{
lzwHuffmanArray::lzwHuffmanArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

lzwHuffmanArray::~lzwHuffmanArray()
{

}

pArray lzwHuffmanArray::shallowClone(const bool takeOwnership)
{
	auto outDesc = std::make_shared<arrayDesc>(*this->getDesc());
	pArray outArr = std::make_shared<lzwHuffmanArray>(outDesc);

	outArr->shallowChunkCopy(*this, takeOwnership);

	return outArr;
}

void lzwHuffmanArray::initChunkFactories()
{
	lzwHuffmanChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb