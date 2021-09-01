#include <pch.h>
#include <array/chunk.h>
#include <array/memChunkBuffer.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
chunk::chunk(pChunkDesc desc)
	: cached_(nullptr), desc_(desc), 
	blockCapacity_(desc->getBlockSpace().area()),
	blockBitmap_(std::make_shared<bitmap>(this->blockCapacity_, false)),
	serializable(std::make_shared<chunkHeader>())
{

}

chunk::~chunk()
{
	this->freeBuffer();
}

void chunk::referenceAllBufferToBlock()
{
	blockId capacity = this->getBlockCapacity();
	for(blockId bid = 0; bid < capacity; ++bid)
	{
		this->referenceBufferToBlock(bid);
	}
}

void chunk::bufferAlloc()
{
	this->freeBuffer();
	this->makeBuffer();
	this->cached_->bufferAlloc(this->desc_->mSize_);
	this->referenceAllBufferToBlock();
}

void chunk::bufferAlloc(bufferSize size)
{
	this->freeBuffer();
	this->makeBuffer();
	this->cached_->bufferAlloc(size);
	this->desc_->mSize_ = size;
	this->referenceAllBufferToBlock();
}

// Deep copy
void chunk::bufferCopy(void* data, bufferSize size)
{
	this->bufferAlloc(size);
	this->cached_->copy(data, size);
}

void chunk::bufferCopy(pChunk source)
{
	bufferSize size = source->getDesc()->mSize_;
	this->bufferCopy(source->getBuffer()->getData(), size);
}

void chunk::bufferCopy(pBlock source)
{
	bufferSize size = source->getDesc()->mSize_;
	this->bufferCopy(source->getBuffer()->getData(), size);
}

// Copy pointer
//void chunk::bufferRef(void* data, bufferSize size)
//{
//	this->free();
//	this->makeBuffer();
//	this->cached_->refChunkBufferWithoutOwnership(data, size);
//	this->referenceAllBufferToBlock();
//	this->desc_->mSize_ = size;
//}

void chunk::bufferRef(pChunk source)
{
	bufferSize size = source->getDesc()->mSize_;
	//this->bufferRef(source->getBuffer()->getData(), size);
	this->freeBuffer();
	this->makeBuffer();
	this->getBuffer()->ref(source->getBuffer(), size);
	this->referenceAllBufferToBlock();
	this->desc_->mSize_ = size;
}

bool chunk::isMaterialized() const
{
	if (this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}

pChunkBuffer chunk::getBuffer()
{
	return this->cached_;
}

chunkId chunk::getId() const
{
	return this->desc_->id_;
}

void chunk::setId(chunkId id)
{
	this->desc_->id_ = id;
}

const pChunkDesc chunk::getDesc() const
{
	return this->desc_;
}
void chunk::setChunkDesc(const pChunkDesc inDesc)
{
	*this->desc_ = *inDesc;
}
chunk::size_type chunk::getDSize()
{
	return this->desc_->getDimSize();
}
chunk::size_type chunk::numCells()
{
	size_type output = 1;
	for (dimensionId d = 0; d < this->desc_->dims_.size(); d++)
	{
		output *= this->desc_->dims_[d];
	}
	return output;
}
void chunk::print()
{
	auto bit = this->getBlockIterator();

	while (!bit->isEnd())
	{
		if (bit->isExist())
		{
			BOOST_LOG_TRIVIAL(debug) << "------------------------------\n";
			BOOST_LOG_TRIVIAL(debug) << "Chunk [" << this->getChunkCoor().toString() << " (" << static_cast<int64_t>(this->getId()) << ")]";
			(**bit)->print();
			BOOST_LOG_TRIVIAL(debug) << "------------------------------\n";
		} else
		{
			BOOST_LOG_TRIVIAL(trace) << "------------------------------\n";
			BOOST_LOG_TRIVIAL(trace) << "Chunk [" << this->getChunkCoor().toString() << " (" << static_cast<int64_t>(this->getId()) << ")]";
			BOOST_LOG_TRIVIAL(trace) << "Block (" << bit->seqPos() << ") is not exist;";
			BOOST_LOG_TRIVIAL(trace) << "------------------------------\n";
		}
		

		++(*bit);
	}
}

coor chunk::getChunkCoor()
{
	return this->desc_->chunkCoor_;
}

coorRange chunk::getChunkRange()
{
	return coorRange(this->desc_->sp_, this->desc_->ep_);
}

void chunk::flush()
{
	this->freeBuffer();
}

void chunk::freeBuffer()
{
	if (this->isMaterialized())
	{
		if(this->cached_ != nullptr)
		{
			this->cached_->free();
		}

		this->cached_ = nullptr;
	}
}

void chunk::makeBlocks(const bitmap& blockBitmap)
{
	assert(blockBitmap.getCapacity() == this->getBlockCapacity());
	blockId capacity = this->getBlockCapacity();
	for(blockId bid = 0; bid < capacity; ++bid)
	{
		if(blockBitmap[bid])
		{
			this->makeBlock(bid);
		}else
		{
			this->freeBlock(bid);
		}
	}
}

void chunk::makeBlocks()
{
	assert(this->blockBitmap_ != nullptr);

	blockId capacity = this->getBlockCapacity();
	for (blockId bid = 0; bid < capacity; ++bid)
	{
		if (this->blockBitmap_->isExist(bid))
		{
			this->makeBlock(bid);
		} else
		{
			this->freeBlock(bid);
		}
	}
}

void chunk::makeAllBlocks()
{
	for(blockId bid = 0; bid < this->blockCapacity_; ++bid)
	{
		this->makeBlock(bid);
	}
}

pBlockDesc chunk::getBlockDesc(const blockId bId)
{
	pAttributeDesc attrDesc = this->desc_->attrDesc_;
	dimension blockDims = this->desc_->getBlockDims();
	coor blockCoor = this->getBlockCoor(bId);
	coor sp = blockCoor * blockDims;
	coor ep = sp + blockDims;

	return std::make_shared<blockDesc>(bId, attrDesc->type_,
									   blockDims,
									   sp, ep);
}

size_t chunk::getBlockCapacity()
{
	return this->blockCapacity_;
}

void chunk::updateToHeader()
{
	auto curHeader = std::static_pointer_cast<chunkHeader>(this->getHeader());
	curHeader->version_ = chunk::chunkHeader::chunk_header_version;
	curHeader->bodySize_ = this->getSerializedSize();
}

void chunk::updateFromHeader()
{
	auto curHeader = std::static_pointer_cast<chunkHeader>(this->getHeader());
	this->setSerializedSize(curHeader->bodySize_);
	this->bufferAlloc();
}
coor chunk::getBlockCoor(const blockId bId)
{
	return this->getBlockIterator()->seqToCoor(bId);
}
void chunk::copyBlockBitmap(cpBitmap blockBitmap)
{
	this->blockBitmap_ = std::make_shared<bitmap>(*blockBitmap);
}
void chunk::replaceBlockBitmap(pBitmap blockBitmap)
{
	this->blockBitmap_ = blockBitmap;
}
void chunk::mergeBlockBitmap(pBitmap blockBitmap)
{
	this->blockBitmap_->andMerge(*blockBitmap);
}
pBitmap chunk::getBlockBitmap()
{
	return this->blockBitmap_;
}
//cpBitmap chunk::getBlockBitmap() const
//{
//	return this->blockBitmap_;
//}
}		// core
}		// msdb
