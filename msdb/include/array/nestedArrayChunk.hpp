#pragma once
#include "nestedArrayChunk.h"

namespace msdb
{
namespace core
{
template <typename Ty_>
nestedArrayChunk<Ty_>::nestedArrayChunk(pChunkDesc desc)
	: chunk(desc)
{
	this->blockCapacity_ = this->desc_->getBlockSpace().area();
	this->blocks_.resize(this->getBlockCapacity(), nullptr);
}
template <typename Ty_>
nestedArrayChunk<Ty_>::~nestedArrayChunk()
{
}

template <typename Ty_>
blockId nestedArrayChunk<Ty_>::exBlockId2InBlockId(const nestedArrayChunk<Ty_>::level_type& level, const blockId& exId)
{
	return blockId();
}

template <typename Ty_>
dimension nestedArrayChunk<Ty_>::getBlockSpaceByLevel(const nestedArrayChunk<Ty_>::level_type& level)
{
	return this->getDesc()->getBlockSpace() * pow(2, level);
}

template <typename Ty_>
void nestedArrayChunk<Ty_>::makeBuffer()
{
	// TODO
}
template <typename Ty_>
void nestedArrayChunk<Ty_>::referenceBufferToBlock(const blockId bId)
{
	// TODO
}

template <typename Ty_>
pBlock nestedArrayChunk<Ty_>::makeBlock(const blockId bId)
{
	// TODO
	return nullptr;
}
template <typename Ty_>
void nestedArrayChunk<Ty_>::insertBlock(pBlock inBlock)
{
	// TODO
}
template <typename Ty_>
pBlock nestedArrayChunk<Ty_>::getBlock(const blockId bId)
{
	// TODO
	return nullptr;
}
template <typename Ty_>
void nestedArrayChunk<Ty_>::freeBlock(const blockId bid)
{
	// TODO
}
template <typename Ty_>
coor nestedArrayChunk<Ty_>::blockId2blockCoor(const blockId bId)
{
	// TODO
	return coor(0);
}
template <typename Ty_>
pBlockIterator nestedArrayChunk<Ty_>::getBlockIterator(const iterateMode itMode)
{
	// TODO
	return nullptr;
}

template <typename Ty_>
void nestedArrayChunk<Ty_>::serialize(std::ostream& os)
{
	// TODO
}
template <typename Ty_>
void nestedArrayChunk<Ty_>::deserialize(std::istream& is)
{
	// TODO
}
}		// core
}		// msdb