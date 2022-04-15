#pragma once
#ifndef _MSDB_CHUNKDESC_H_
#define _MSDB_CHUNKDESC_H_

#include <pch.h>
#include <array/chunkId.h>
#include <array/attributeDesc.h>
#include <array/dimensionDesc.h>
#include <compression/compressionType.h>
#include <util/coordinate.h>
#include <array/buffer.h>

namespace msdb
{
namespace core
{
class chunkDesc;
class blockChunkDesc;
using pChunkDesc = std::shared_ptr<chunkDesc>;

using chunkSize = bufferSize;
extern const chunkSize INVALID_CHUNK_SIZE;

class chunkDesc : std::enable_shared_from_this<chunkDesc>
{
	struct chunkHeader
	{
		uint64_t bytes;

	};
public:
	chunkDesc();

	chunkDesc(const chunkId id,
			  pAttributeDesc attrDesc,
			  const dimension& dims, const dimension& blockDims,
			  const coor& sp, const coor& ep,
			  const chunkSize mSize = INVALID_CHUNK_SIZE);

	//chunkDesc(const chunkId id,
	//		  pAttributeDesc attrDesc, const dimension& dims,
	//		  const dimension& blockDims,
	//		  const coor sp, const coor ep,
	//		  const chunkSize mSize, const chunkSize cSize,
	//		  const CompressionMethod cType);

	chunkDesc(const chunkDesc& src);
	chunkDesc(chunkDesc&& src);
	
	friend void swap(chunkDesc& first, chunkDesc& second) noexcept;

public:
	void setDim(dimensionId dId, position_t value);
	dimension getDims();
	virtual dimension getBlockDims();
	virtual dimension getBlockSpace();
	size_t getDimSize();

public:
	//////////////////////////////
	// Operators
	// ***************************
	// Assign
	chunkDesc& operator=(const chunkDesc& src);
	chunkDesc& operator=(chunkDesc&& src) noexcept;

public:
	void initPhysicalChunkSizeFromDims();

protected:
	void initChunkCoor();

public:
	chunkId id_;				// chunk id
	pAttributeDesc attrDesc_;	// attribute desc

	chunkSize mSize_;			// materialized size
	chunkSize cSize_;			// compressed size,
								// if not: cSize == mSize or INVALID_CHUNK_SIZE
	bool useCompression_;		// can be compressed
	compressionType cType_;		// compressed method type
								// if not: CompressionMethod::NONE

	dimension dims_;			// dimensions
	dimension blockDims_;		// block dimensions
	coor sp_;					// start point
	coor ep_;					// end point
	coor chunkCoor_;			// chunk coordinate
};

void swap(chunkDesc& first, chunkDesc& second) noexcept;
}		// core
}		// msdb
#endif	// _MSDB_CHUNKDESC_H_
