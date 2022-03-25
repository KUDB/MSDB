#include <pch.h>
#include <array/flattenArray.h>
#include <array/flattenChunk.h>

namespace msdb
{
namespace core
{
flattenArray::flattenArray(pArrayDesc desc)
	: base_type(desc)
{
	initChunkFactories();
}

flattenArray::~flattenArray()
{

}

void flattenArray::initChunkFactories()
{
	for (auto attrDesc : *this->getDesc()->getAttrDescs())
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		switch (attrDesc->type_)
		{
		case eleType::CHAR:
		case eleType::INT8:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<int8_t>>());
			break;
		}
		case eleType::INT16:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<int16_t>>());
			break;
		}
		case eleType::INT32:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<int32_t>>());
			break;
		}
		case eleType::INT64:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<int64_t>>());
			break;
		}
		case eleType::UINT8:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<uint8_t>>());
			break;
		}
		case eleType::UINT16:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<uint16_t>>());
			break;
		}
		case eleType::UINT32:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<uint32_t>>());
			break;
		}
		case eleType::UINT64:
		{
			this->cFactories_.push_back(std::make_unique<flattenChunkFactory<uint64_t>>());
			break;
		}
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}
}

pChunk flattenArray::makeChunk(const attributeId attrId, const chunkId cId)
{
	auto desc = this->getChunkDesc(attrId, cId);
	auto chunkObj = std::make_shared<flattenChunk<element>>(desc);
	this->insertChunk(attrId, chunkObj);
	return chunkObj;
}
pChunk flattenArray::makeChunk(const chunkDesc& desc)
{
	auto chunkObj
		= std::make_shared<flattenChunk<element>>(std::make_shared<chunkDesc>(desc));
	this->insertChunk(desc.attrDesc_->id_, chunkObj);
	return chunkObj;
}
}		// core
}		// msdb