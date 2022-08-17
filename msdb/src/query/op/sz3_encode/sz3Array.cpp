#include <pch.h>
#include <op/sz3_encode/sz3Array.h>
#include <op/sz3_encode/sz3Chunk.h>

namespace msdb
{
namespace core
{
sz3Array::sz3Array(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

sz3Array::~sz3Array()
{

}

void sz3Array::initChunkFactories()
{
	sz3ChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb