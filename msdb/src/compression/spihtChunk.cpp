#include <pch.h>
#include <compression/spihtChunk.h>
#include <compression/spihtBlock.h>
#include <array/memChunkBuffer.h>
#include <array/memChunk.h>

namespace msdb
{
namespace core
{
spihtChunk::spihtChunk(pChunkDesc desc)
	: memChunk(desc)
{
}

spihtChunk::~spihtChunk()
{
}

void spihtChunk::initBufferZero()
{
	memset(this->cached_->getData(), 0, this->desc_->mSize_);
}

//void spihtChunk::makeBlocks(std::vector<bool> bitmap)
//{
//	if (bitmap[0])
//	{
//		this->block_ = std::make_shared<spihtBlock>(
//			std::make_shared<blockDesc>(
//			0,						// id
//			this->desc_->attrDesc_->type_,	// eType
//			this->desc_->getDims(),	// dims
//			this->desc_->sp_,		// sp
//			this->desc_->ep_,		// ep
//			this->desc_->mSize_		// mSize
//			));
//		pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(this->block_);
//		spBlock->setLevel(this->maxLevel_);
//	}
//}

void spihtChunk::makeBuffer()
{
	this->cached_ = std::make_shared<memChunkBuffer>();
}

pBlock spihtChunk::makeBlock(const blockId bId)
{
	//if(bId == 0 && this->block_ == nullptr)
	//{
		auto desc = this->getBlockDesc(bId);
		desc->mSize_ = desc->mSize_;
		desc->mOffset_ = 0;
		auto blockObj = std::make_shared<spihtBlock>(desc);
		blockObj->setLevel(this->maxLevel_);
		this->insertBlock(blockObj);
		return blockObj;
	//}

	//return nullptr;
}

//pChunkItemIterator spihtChunk::getItemIterator()
//{
//	_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_NOT_IMPLEMENTED, "spihtChunk has no itemiterator");
//	return nullptr;
//}
//
//pChunkItemRangeIterator spihtChunk::getItemRangeIterator(const coorRange& range)
//{
//	_MSDB_EXCEPTIONS_MSG(MSDB_EC_LOGIC_ERROR, MSDB_ER_NOT_IMPLEMENTED, "spihtChunk has no itemiterator");
//	return nullptr;
//}

void spihtChunk::serialize(std::ostream& os)
{
	bstream bs;
	switch (this->desc_->attrDesc_->type_)
	{
	case eleType::CHAR:
		this->serialize<char>(bs);
		break;
	case eleType::INT8:
		this->serialize<int8_t>(bs);
		break;
	case eleType::INT16:
		this->serialize<int16_t>(bs);
		break;
	case eleType::INT32:
		this->serialize<int32_t>(bs);
		break;
	case eleType::INT64:
		this->serialize<int64_t>(bs);
		break;
	case eleType::UINT8:
		this->serialize<uint8_t>(bs);
		break;
	case eleType::UINT16:
		this->serialize<uint16_t>(bs);
		break;
	case eleType::UINT32:
		this->serialize<uint32_t>(bs);
		break;
	case eleType::UINT64:
		this->serialize<uint64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}

	this->serializedSize_ = bs.capacity();
	this->getOutHeader()->serialize(os);
	os.write(bs.data(), bs.capacity());
}

void spihtChunk::deserialize(std::istream& is)
{
	this->getHeader()->deserialize(is);
	this->updateFromHeader();

	bstream bs;
	bs.resize(this->serializedSize_);
	is.read(bs.data(), this->serializedSize_);

	switch (this->desc_->attrDesc_->type_)
	{
	case eleType::CHAR:
		this->deserialize<char>(bs);
		break;
	case eleType::INT8:
		this->deserialize<int8_t>(bs);
		break;
	case eleType::INT16:
		this->deserialize<int16_t>(bs);
		break;
	case eleType::INT32:
		this->deserialize<int32_t>(bs);
		break;
	case eleType::INT64:
		this->deserialize<int64_t>(bs);
		break;
	case eleType::UINT8:
		this->deserialize<uint8_t>(bs);
		break;
	case eleType::UINT16:
		this->deserialize<uint16_t>(bs);
		break;
	case eleType::UINT32:
		this->deserialize<uint32_t>(bs);
		break;
	case eleType::UINT64:
		this->deserialize<uint64_t>(bs);
		break;
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
}
}		// core
}		// msdb