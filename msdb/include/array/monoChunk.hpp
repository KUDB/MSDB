#pragma once
#include <array/monoChunk.h>
#include <array/monoChunk.h>
#include <array/monoChunkBuffer.h>
#include <array/monoChunkItemIterator.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
monoChunk<Ty_>::monoChunk(pChunkDesc desc)
	: chunk(desc), block_(nullptr)
{
	this->blockCapacity_ = 1;
}

template <typename Ty_>
monoChunk<Ty_>::~monoChunk()
{
}

template <typename Ty_>
void monoChunk<Ty_>::makeBuffer()
{
	this->cached_ = std::make_shared<monoChunkBuffer>();
}

template <typename Ty_>
pBlock monoChunk<Ty_>::makeBlock(const blockId bId)
{
	if (bId == 0 && this->block_ == nullptr)
	{
		auto desc = this->getBlockDesc(bId);
		desc->mSize_ = desc->dims_.area() * this->desc_->attrDesc_->typeSize_;
		desc->mOffset_ = 0;
		auto blockObj = std::make_shared<flattenBlock<Ty_>>(desc);
		this->insertBlock(blockObj);
		return blockObj;
	}
	return nullptr;
}

template <typename Ty_>
void monoChunk<Ty_>::insertBlock(pBlock inBlock)
{
	this->block_ = inBlock;
	this->getBlockBitmap()->setExist(inBlock->getId());
	this->referenceBufferToBlock(inBlock->getId());
}

template <typename Ty_>
void monoChunk<Ty_>::referenceBufferToBlock(const blockId bId)
{
	if (bId == 0 && this->block_ && this->cached_)
	{
		this->block_->refChunkBufferWithoutOwnership(this->cached_->getData(), this->block_->getDesc()->mSize_);
	}
}

template <typename Ty_>
void monoChunk<Ty_>::serialize(std::ostream& os)
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

template <typename Ty_>
void monoChunk<Ty_>::deserialize(std::istream& is)
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

template <typename Ty_>
pBlock monoChunk<Ty_>::getBlock(const blockId bId)
{
	assert(bId == 0);
	return this->block_;
}

template <typename Ty_>
void monoChunk<Ty_>::freeBlock(const blockId bid)
{
	this->block_ = nullptr;
	this->blockBitmap_->setNull(bid);
}

template <typename Ty_>
coor monoChunk<Ty_>::blockId2blockCoor(const blockId bId)
{
	assert(bId == 0);
	return coor(this->getDSize());
}

template <typename Ty_>
pBlockIterator monoChunk<Ty_>::getBlockIterator(const iterateMode itMode)
{
	return std::make_shared<singleBlockIterator>(
		this->desc_->getBlockSpace(),
		this->block_, itMode);
}
}		// core
}		// msdb