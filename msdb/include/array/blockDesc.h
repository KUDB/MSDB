#pragma once
#ifndef _MSDB_BLOCK_DESC_H_
#define _MSDB_BLOCK_DESC_H_

#include <array/buffer.h>
#include <array/blockId.h>
#include <array/dimensionDesc.h>
#include <util/coordinate.h>
#include <util/dataType.h>

namespace msdb
{
namespace core
{
class blockDesc;

using pBlockDesc = std::shared_ptr<blockDesc>;
using blockSize = bufferSize;
extern const blockSize INVALID_BLOCK_SIZE;

class blockDesc : std::enable_shared_from_this<blockDesc>
{
public:
	blockDesc();

	blockDesc(const blockId id, const eleType eType, const dataType type,
			  const dimension& dims,
			  const coor& sp, const coor& ep, 
			  const blockSize mSize = INVALID_BLOCK_SIZE,
			  const blockSize mOffset = INVALID_BLOCK_SIZE);

public:
	// Getter //
	coor getSp() const;
	coor getEp() const;
	coor getIsp() const;
	coor getIep() const;

	inline const dataType& getDataType() const;

	// Setter //
	void setSp(const coor& coor);
	void setEp(const coor& coor);
	void setIsp(const coor& coor);
	void setIep(const coor& coor);

	// Operator //
	bool operator== (const blockDesc& rhs) const;
	bool operator!= (const blockDesc& rhs) const;

protected:
	void initBlockCoor();

public:
	blockId id_;				// Block id in a Chunk
	blockSize mSize_;			// Materialized size
	blockSize mOffset_;			// Memory offset in ChunkBuffer
	eleType eType_;				// TODO::delete eleType [Deprecated]

	dimension dims_;			// Dimensions
	coor blockCoor_;			// Block coordinate in a Chunk

private:
	dataType _dataType;			// Item data type

	coor sp_;					// Start point in a Chunk
	coor ep_;					// End point in a Chunk
	coor isp_;					// Item start point
	coor iep_;					// Item end point
};
}		// core
}		// msdb
#endif	// _MSDB_BLOCK_DESC_H_