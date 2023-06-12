#pragma once
#include "nestedChunk.h"

namespace msdb
{
namespace core
{
template <typename Ty_>
nestedChunk<Ty_>::nestedChunk(pChunkDesc desc)
	: chunk(desc)
{
	this->blockCapacity_ = this->desc_->getBlockSpace().area();
	this->blocks_.resize(this->getBlockCapacity(), nullptr);
}
template <typename Ty_>
nestedChunk<Ty_>::~nestedChunk()
{
}

template <typename Ty_>
blockId nestedChunk<Ty_>::exBlockId2InBlockId(const nestedChunk<Ty_>::level_type& level, const blockId& exId)
{
	return blockId();
}

template <typename Ty_>
dimension nestedChunk<Ty_>::getBlockSpaceByLevel(const nestedChunk<Ty_>::level_type& level)
{
	return this->getDesc()->getBlockSpace() * pow(2, level);
}

template <typename Ty_>
void nestedChunk<Ty_>::makeBuffer()
{
	// TODO
}
template <typename Ty_>
void nestedChunk<Ty_>::referenceBufferToBlock(const blockId bId)
{
	// TODO
}

template <typename Ty_>
pBlock nestedChunk<Ty_>::makeBlock(const blockId bId)
{
	// TODO
	return nullptr;
}
template <typename Ty_>
void nestedChunk<Ty_>::insertBlock(pBlock inBlock)
{
	// TODO
}
template <typename Ty_>
pBlock nestedChunk<Ty_>::getBlock(const blockId bId)
{
	// TODO
	return nullptr;
}
template <typename Ty_>
void nestedChunk<Ty_>::freeBlock(const blockId bid)
{
	// TODO
}
template <typename Ty_>
coor nestedChunk<Ty_>::blockId2blockCoor(const blockId bId)
{
	// TODO
	return coor(0);
}
template <typename Ty_>
pBlockIterator nestedChunk<Ty_>::getBlockIterator(const iterateMode itMode)
{
	// TODO
	return nullptr;
}

template <typename Ty_>
pConstBlockIterator nestedChunk<Ty_>::getBlockIterator(const iterateMode itMode) const
{
	// TODO
	return nullptr;
}

template <typename Ty_>
void nestedChunk<Ty_>::serialize(std::ostream& os)
{
	// TODO
}
template <typename Ty_>
void nestedChunk<Ty_>::deserialize(std::istream& is)
{
	// TODO
}

//////////////////////////////
// nestedChunkFactory
//
template <typename Ty_>
pChunk nestedChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<nestedChunk<Ty_>>(cDesc);
}
}		// core
}		// msdb