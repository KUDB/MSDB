#pragma once
#ifndef _MSDB_BLOCK_DESC_H_
#define _MSDB_BLOCK_DESC_H_

#include <array/buffer.h>
#include <array/blockId.h>
#include <array/dimensionDesc.h>
#include <util/coordinate.h>

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

	blockDesc(const blockId id, const eleType eType,
			  const dimension& dims,
			  const coor& sp, const coor& ep, 
			  const blockSize mSize = INVALID_BLOCK_SIZE,
			  const blockSize mOffset = INVALID_BLOCK_SIZE);

public:
	coor getSp();
	coor getEp();
	coor getIsp();
	coor getIep();

	void setSp(const coor& coor);
	void setEp(const coor& coor);
	void setIsp(const coor& coor);
	void setIep(const coor& coor);

protected:
	void initBlockCoor();

public:
	blockId id_;				// in chunk
	blockSize mSize_;			// materialized size
	blockSize mOffset_;			// memory offset in chunkBuffer
	eleType eType_;

	dimension dims_;			// dimensions
	coor blockCoor_;			// in chunk

private:
	coor sp_;					// in chunk
	coor ep_;					// in chunk
	coor isp_;					// item start point
	coor iep_;					// item end point
};
}		// core
}		// msdb
#endif	// _MSDB_BLOCK_DESC_H_