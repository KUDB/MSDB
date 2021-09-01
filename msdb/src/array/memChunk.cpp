#include <pch.h>
#include <array/memBlock.h>
#include <array/memChunk.h>
#include <array/memChunkBuffer.h>
#include <array/memChunkItemIterator.h>

namespace msdb
{
namespace core
{
memChunk::memChunk(pChunkDesc desc)
	: chunk(desc), block_(nullptr)
{
	this->blockCapacity_ = 1;
}

memChunk::~memChunk()
{
}

void memChunk::makeBuffer()
{
	this->cached_ = std::make_shared<memChunkBuffer>();
}

pBlock memChunk::makeBlock(const blockId bId)
{
	if(bId == 0 && this->block_ == nullptr)
	{
		auto desc = this->getBlockDesc(bId);
		desc->mSize_ = desc->dims_.area() * this->desc_->attrDesc_->typeSize_;
		desc->mOffset_ = 0;
		auto blockObj = std::make_shared<memBlock>(desc);
		this->insertBlock(blockObj);
		return blockObj;
	}
	return nullptr;
}

void memChunk::insertBlock(pBlock inBlock)
{
	this->block_ = inBlock;
	this->getBlockBitmap()->setExist(inBlock->getId());
	this->referenceBufferToBlock(inBlock->getId());
}

void memChunk::referenceBufferToBlock(const blockId bId)
{
	if (bId == 0 && this->block_ && this->cached_)
	{
		this->block_->refChunkBufferWithoutOwnership(this->cached_->getData(), this->block_->getDesc()->mSize_);
	}
}

//pChunkItemIterator memChunk::getItemIterator()
//{
//	return std::make_shared<memChunkItemIterator>(this->cached_->getData(),
//												  this->desc_->attrDesc_->type_,
//												  this->desc_->dims_,
//												  this->desc_->sp_);
//}
//pChunkItemRangeIterator memChunk::getItemRangeIterator(const coorRange& range)
//{
//	return std::make_shared<memChunkItemRangeIterator>(this->cached_->getData(),
//													   this->desc_->attrDesc_->type_,
//													   this->desc_->dims_,
//													   range,
//													   this->desc_->sp_);
//}

void memChunk::serialize(std::ostream& os)
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

void memChunk::deserialize(std::istream& is)
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
pBlock memChunk::getBlock(const blockId bId)
{
	assert(bId == 0);
	return this->block_;
}
void memChunk::freeBlock(const blockId bid)
{
	this->block_ = nullptr;
	this->blockBitmap_->setNull(bid);
}
coor memChunk::getBlockCoor(const blockId bId)
{
	assert(bId == 0);
	return coor(this->getDSize());
}
//blockId memChunk::getBlockId(pBlockDesc cDesc)
//{
//	// TODO::getBlockId
//	return 0;
//}
//blockId memChunk::getBlockIdFromItemCoor(coor& itemCoor)
//{
//	return 0;
//}
//blockId memChunk::getBlockIdFromBlockCoor(coor& blockCoor)
//{
//	assert(blockCoor == coor(this->desc_->getDimSize()));
//	return 0;
//}
//coor memChunk::itemCoorToBlockCoor(coor& itemCoor)
//{
//	return itemCoor;
//}
pBlockIterator memChunk::getBlockIterator(const iterateMode itMode)
{
	//return std::make_shared<blockIterator>(this->desc_->getBlockSpace(),
	//									   &this->blocks_, &this->blockBitmap_,
	//									   itMode);
	return std::make_shared<singleBlockIterator>(
		this->desc_->getBlockSpace(),
		this->block_, itMode);
	//return nullptr;
}
}		// core
}		// msdb