#include <pch.h>
#include <op/zfp_encode/zfpArray.h>
#include <op/zfp_encode/zfpChunk.h>

namespace msdb
{
namespace core
{
zfpArray::zfpArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

zfpArray::~zfpArray()
{

}

pArray zfpArray::shallowClone(const bool takeOwnership)
{
	auto outDesc = std::make_shared<arrayDesc>(*this->getDesc());
	pArray outArr = std::make_shared<zfpArray>(outDesc);

	outArr->shallowChunkCopy(*this, takeOwnership);

	return outArr;
}

void zfpArray::initChunkFactories()
{
	zfpChunkFactoryBuilder fb;

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb