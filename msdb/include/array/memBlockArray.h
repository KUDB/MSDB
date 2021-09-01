#pragma once
#ifndef _MSDB_MEMBLOCKARRAY_H_
#define _MSDB_MEMBLOCKARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class memBlockArray : public arrayBase
{
public:
	using base_type = arrayBase;

public:
	memBlockArray(pArrayDesc desc);
	~memBlockArray();

public:
	virtual pChunk makeChunk(const attributeId attrId, const chunkId cId) override;
	virtual pChunk makeChunk(const chunkDesc& desc) override;
	virtual void freeChunk(const chunkId cId) override;
};
}		// core
}		// msdb
#endif	// _MSDB_MEMBLOCKARRAY_H_