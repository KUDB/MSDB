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
	flattenChunkFactoryBuilder fcfc;


	for (auto attrDesc : *this->getDesc()->getAttrDescs())
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fcfc, attrDesc->dataType_));

		// TODO::Erase deprecated codes
		//switch (attrDesc->type_)
		//{
		//case eleType::CHAR:
		//case eleType::INT8:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<int8_t>>());
		//	break;
		//}
		//case eleType::INT16:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<int16_t>>());
		//	break;
		//}
		//case eleType::INT32:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<int32_t>>());
		//	break;
		//}
		//case eleType::INT64:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<int64_t>>());
		//	break;
		//}
		//case eleType::UINT8:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<uint8_t>>());
		//	break;
		//}
		//case eleType::UINT16:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<uint16_t>>());
		//	break;
		//}
		//case eleType::UINT32:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<uint32_t>>());
		//	break;
		//}
		//case eleType::UINT64:
		//{
		//	this->cFactories_.push_back(std::make_shared<flattenChunkFactory<uint64_t>>());
		//	break;
		//}
		//default:
		//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		//}
	}
}

// TODO::Erase deprecated codes
//pChunk flattenArray::makeChunk(const attributeId attrId, const chunkId cId)
//{
//	auto desc = this->getChunkDesc(attrId, cId);
//	//auto chunkObj = std::make_shared<flattenChunk<element>>(desc);
//	auto chunkObj = this->cFactories_[attrId]->requestNewChunk(desc);
//	this->insertChunk(chunkObj);
//	return chunkObj;
//}
//pChunk flattenArray::makeChunk(pChunkDesc desc)
//{
//	// TODO::Use factory
//	auto chunkObj = this->cFactories_[desc->attrDesc_->id_]->requestNewChunk(desc);
//	this->insertChunk(chunkObj);
//	return chunkObj;
//}
}		// core
}		// msdb