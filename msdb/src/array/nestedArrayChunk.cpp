#include <pch.h>
#include <array/nestedArrayChunk.h>

namespace msdb
{
namespace core
{
nestedArrayChunk::nestedArrayChunk(pChunkDesc desc)
	: chunk(desc)
{
	this->blockCapacity_ = this->desc_->getBlockSpace().area();
	this->blocks_.resize(this->getBlockCapacity(), nullptr);
}
nestedArrayChunk::~nestedArrayChunk()
{
}

blockId nestedArrayChunk::exBlockId2InBlockId(const level_type& level, const blockId& exId)
{
	return blockId();
}

dimension nestedArrayChunk::getBlockSpaceByLevel(const level_type& level)
{
	return this->getDesc()->getBlockSpace() * pow(2, level);
}

void nestedArrayChunk::makeBuffer()
{
	// TODO
}
void nestedArrayChunk::referenceBufferToBlock(const blockId bId)
{
	// TODO
}

pBlock nestedArrayChunk::makeBlock(const blockId bId)
{
	// TODO
	return nullptr;
}
void nestedArrayChunk::insertBlock(pBlock inBlock)
{
	// TODO
}
pBlock nestedArrayChunk::getBlock(const blockId bId)
{
	// TODO
	return nullptr;
}
void nestedArrayChunk::freeBlock(const blockId bid)
{
	// TODO
}
coor nestedArrayChunk::blockId2blockCoor(const blockId bId)
{
	// TODO
	return coor(0);
}
pBlockIterator nestedArrayChunk::getBlockIterator(const iterateMode itMode)
{
	// TODO
	return nullptr;
}

void nestedArrayChunk::serialize(std::ostream& os)
{
	// TODO
}
void nestedArrayChunk::deserialize(std::istream& is)
{
	// TODO
}
}		// core
}		// msdb