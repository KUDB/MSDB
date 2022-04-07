#pragma once
#include <array/flattenChunk.h>
#include <array/monoChunkBuffer.h>
#include <array/flattenBlockItemIterator.h>
#include <io/iterator.h>

namespace msdb
{
namespace core
{
/******************************
 * Block
 * 
 ******************************/
template <typename Ty_>
flattenBlock<Ty_>::flattenBlock(pBlockDesc desc)
	: block(desc)
{
	if (this->desc_->mSize_ == INVALID_BLOCK_SIZE ||
		this->desc_->mOffset_ == INVALID_BLOCK_SIZE)
	{
		this->desc_->mSize_
			= this->desc_->dims_.area() * getEleSize(this->desc_->eType_);
		this->desc_->mOffset_ = this->desc_->mSize_ * this->desc_->id_;
	}
}

template <typename Ty_>
flattenBlock<Ty_>::~flattenBlock()
{
	this->desc_ = nullptr;
}

template <typename Ty_>
void flattenBlock<Ty_>::serialize(bstream& bs)
{
	bs << setw(sizeof(Ty_) * CHAR_BIT);
	auto it = this->getItemIterator();
	while (!it->isEnd())
	{
		bs << (**it).get<Ty_>();
		++(*it);
	}

	//// TODO::Directly serialize in Ty_ instead of calling function 'serializeTy'
	//switch (this->desc_->eType_)
	//{
	//case eleType::CHAR:
	//	this->serializeTy<char>(bs);
	//	break;
	//case eleType::INT8:
	//	this->serializeTy<int8_t>(bs);
	//	break;
	//case eleType::INT16:
	//	this->serializeTy<int16_t>(bs);
	//	break;
	//case eleType::INT32:
	//	this->serializeTy<int32_t>(bs);
	//	break;
	//case eleType::INT64:
	//	this->serializeTy<int64_t>(bs);
	//	break;
	//case eleType::UINT8:
	//	this->serializeTy<uint8_t>(bs);
	//	break;
	//case eleType::UINT16:
	//	this->serializeTy<uint16_t>(bs);
	//	break;
	//case eleType::UINT32:
	//	this->serializeTy<uint32_t>(bs);
	//	break;
	//case eleType::UINT64:
	//	this->serializeTy<uint64_t>(bs);
	//	break;
	//default:
	//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	//}
}

template <typename Ty_>
void flattenBlock<Ty_>::deserialize(bstream& bs)
{
	bs >> setw(sizeof(Ty_) * CHAR_BIT);
	auto it = this->getItemIterator();
	while (!it->isEnd())
	{
		Ty_ value;
		bs >> value;
		(**it).set<Ty_>(value);
		++(*it);
	}

	//// TODO::Directly deserialize in Ty_ instead of calling function 'serializeTy'
	//switch (this->desc_->eType_)
	//{
	//case eleType::CHAR:
	//	this->deserializeTy<char>(bs);
	//	break;
	//case eleType::INT8:
	//	this->deserializeTy<int8_t>(bs);
	//	break;
	//case eleType::INT16:
	//	this->deserializeTy<int16_t>(bs);
	//	break;
	//case eleType::INT32:
	//	this->deserializeTy<int32_t>(bs);
	//	break;
	//case eleType::INT64:
	//	this->deserializeTy<int64_t>(bs);
	//	break;
	//case eleType::UINT8:
	//	this->deserializeTy<uint8_t>(bs);
	//	break;
	//case eleType::UINT16:
	//	this->deserializeTy<uint16_t>(bs);
	//	break;
	//case eleType::UINT32:
	//	this->deserializeTy<uint32_t>(bs);
	//	break;
	//case eleType::UINT64:
	//	this->deserializeTy<uint64_t>(bs);
	//	break;
	//default:
	//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	//}
}

template <typename Ty_>
pBlockItemIterator flattenBlock<Ty_>::getItemIterator()
{
	//return std::make_shared<flattenBlockItemIterator>(this->cached_->getData(),
	//											  this->desc_->eType_,
	//											  this->desc_->dims_,
	//											  this->desc_->sp_,
	//											  this->itemBitmap_);
	return std::make_shared<flattenBlockItemIterator>(this->cached_->getData(),
													  this->desc_->eType_,
													  this->desc_->dims_,
													  range(this->desc_->getIsp(), this->desc_->getIep()),
													  this->desc_->getSp(),
													  this->itemBitmap_);
}

template <typename Ty_>
pBlockItemRangeIterator flattenBlock<Ty_>::getItemRangeIterator(const range& itRange)
{
	auto sp = getOutsideCoor(this->desc_->getIsp(), itRange.getSp());
	auto ep = getInsideCoor(this->desc_->getIep(), itRange.getEp());
	return std::make_shared<flattenBlockItemRangeIterator>(this->cached_->getData(),
														   this->desc_->eType_,
														   this->desc_->dims_,
														   range(sp, ep),
														   this->desc_->getSp(),
														   this->itemBitmap_);
}
template <typename Ty_>
vpItemIterator flattenBlock<Ty_>::getValueIterator()
{
	vpItemIterator vpOut = { 
			std::make_shared<flattenBufferItemIterator<Ty_>>(
				(Ty_*)this->cached_->getData(),
				this->desc_->dims_,
				range(this->desc_->getIsp(), this->desc_->getIep()),
				this->itemBitmap_) };
	return vpOut;
}

template <typename Ty_>
 vpItemIterator flattenBlock<Ty_>::getValueRangeIterator(const range& itRange)
{
	 auto sp = getOutsideCoor(this->desc_->getIsp(), itRange.getSp());
	 auto ep = getInsideCoor(this->desc_->getIep(), itRange.getEp());
	 vpItemIterator vpOut = { 
		 std::make_shared<flattenBufferItemIterator<Ty_>>(
				 (Ty_*)this->cached_->getData(),
				 this->desc_->dims_,
				 range(sp, ep),
				 this->itemBitmap_) };
	 return vpOut;
}
template <typename Ty_>
void flattenBlock<Ty_>::refChunkBufferWithoutOwnership(void* data, bufferSize size)
{
	this->cached_ = std::make_shared<flattenBlockBuffer>(data, size);	// TODO::make mem block buffer
}

/******************************
 * Chunk
 * 
 ******************************/
template <typename Ty_>
flattenChunk<Ty_>::flattenChunk(pChunkDesc desc)
	: chunk(desc)
{
	this->blockCapacity_ = this->desc_->getBlockSpace().area();
	this->blocks_.resize(this->getBlockCapacity(), nullptr);
}

template <typename Ty_>
flattenChunk<Ty_>::~flattenChunk()
{
}

template <typename Ty_>
void flattenChunk<Ty_>::makeBuffer()
{
	this->cached_ = std::make_shared<monoChunkBuffer>();
}

template <typename Ty_>
pBlock flattenChunk<Ty_>::makeBlock(const blockId bId)
{
	assert(this->blockCapacity_ > bId);
	if (this->blocks_[bId] == nullptr)
	{
		auto desc = this->getBlockDesc(bId);
		auto blockObj = std::make_shared<flattenBlock<Ty_>>(desc);
		this->insertBlock(blockObj);
		return blockObj;
	}
	return this->blocks_[bId];
}

template <typename Ty_>
void flattenChunk<Ty_>::insertBlock(pBlock inBlock)
{
	assert(this->blockCapacity_ > inBlock->getId());
	this->blocks_[inBlock->getId()] = inBlock;
	this->blockBitmap_->setExist(inBlock->getId());
	this->referenceBufferToBlock(inBlock->getId());
}

template <typename Ty_>
void flattenChunk<Ty_>::referenceBufferToBlock(blockId bId)
{
	// Reference block buffers to the chunk buffer
	if (this->blocks_[bId] && this->cached_)
	{
		bufferSize mSizeBlock = this->blocks_[bId]->getDesc()->mSize_;
		this->blocks_[bId]->refChunkBufferWithoutOwnership(
			(char*)this->cached_->getData() + (bId * mSizeBlock),
			mSizeBlock);
	}
}

template <typename Ty_>
pBlock flattenChunk<Ty_>::getBlock(const blockId bId)
{
	assert(this->blockCapacity_ > bId);
	return this->blocks_[bId];
}

template <typename Ty_>
void flattenChunk<Ty_>::freeBlock(const blockId bid)
{
	this->blocks_[bid] = nullptr;
	this->blockBitmap_->setNull(bid);
}

template <typename Ty_>
pBlockIterator flattenChunk<Ty_>::getBlockIterator(const iterateMode itMode)
{
	return std::make_shared<blockIterator>(this->desc_->getBlockSpace(),
										   &this->blocks_, this->blockBitmap_,
										   itMode);
}

template <typename Ty_>
pChunkItemIterator flattenChunk<Ty_>::getItemIterator()
{
	return std::make_shared<blockChunkItemIterator>(this->cached_->getData(),
													this->desc_->attrDesc_->type_,
													this->desc_->dims_,
													this->desc_->sp_,
													this->getBlockIterator());
}

template <typename Ty_>
pChunkItemRangeIterator flattenChunk<Ty_>::getItemRangeIterator(const range& range)
{
	return std::make_shared<blockChunkItemRangeIterator>(this->cached_->getData(),
														 this->desc_->attrDesc_->type_,
														 this->desc_->dims_,
														 range,
														 this->desc_->sp_,
														 this->getBlockIterator());
}

template <typename Ty_>
void flattenChunk<Ty_>::serialize(std::ostream& os)
{
	bstream bs;
	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		(*it)->serialize(bs);
		++(*it);
	}
	this->serializedSize_ = bs.capacity();

	this->getOutHeader()->serialize(os);
	os.write(bs.data(), bs.capacity());
}

template <typename Ty_>
void flattenChunk<Ty_>::deserialize(std::istream& is)
{
	this->getHeader()->deserialize(is);
	this->updateFromHeader();

	bstream bs;
	bs.resize(this->serializedSize_);
	is.read(bs.data(), this->serializedSize_);

	auto it = this->getBlockIterator();
	while (!it->isEnd())
	{
		(*it)->deserialize(bs);
		++(*it);
	}
}

//////////////////////////////
// flattenChunkFactory
//
template <typename Ty_>
pChunk flattenChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<flattenChunk<Ty_>>(cDesc);
}
}		// core
}		// msdb