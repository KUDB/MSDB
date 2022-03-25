#pragma once
#ifndef _MSDB_NESTEDARRAYCHUNK_H_
#define _MSDB_NESTEDARRAYCHUNK_H_

#include <pch.h>
#include <array/chunkIterator.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class nestedArrayChunk : public chunk
{
public:
	using size_type = chunk::size_type;
	using level_type = int16_t;

public :
	nestedArrayChunk(pChunkDesc desc);
	virtual ~nestedArrayChunk();

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	virtual void makeBuffer();
	virtual void referenceBufferToBlock(const blockId bId);

public:
	virtual pBlock makeBlock(const blockId bId) override;
	virtual void insertBlock(pBlock inBlock) override;
	virtual pBlock getBlock(const blockId bId) override;
	virtual void freeBlock(const blockId bid) override;
	virtual coor blockId2blockCoor(const blockId bId);
	virtual pBlockIterator getBlockIterator(
		const iterateMode itMode = iterateMode::ALL) override;

public:
//////////////////////////////
// Serializable
//////////////////////////////
public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

protected:
	// Convert Level with External Block ID to Internal Block ID
	blockId exBlockId2InBlockId(const level_type& level, const blockId& exId);
	dimension getBlockSpaceByLevel(const level_type& level);		// TODO::method for chunkDesc ??

private:
	level_type _level;
	blockContainer blocks_;
};

}		// core
}		// msdb

#include "nestedArrayChunk.hpp"

#endif	_MSDB_NESTEDARRAYCHUNK_H_