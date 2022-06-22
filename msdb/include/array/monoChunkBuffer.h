#pragma once
#ifndef _MSDB_RAWCHUNK_H_
#define _MSDB_RAWCHUNK_H_

#include <pch.h>
#include <array/chunk.h>
#include <array/chunkBuffer.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class monoChunkBuffer : public chunkBuffer
{
public:
	monoChunkBuffer();
	virtual ~monoChunkBuffer();

public:
	virtual bool isCompressed() const;
};
}		// core
}		// msdb
#endif	// _MSDB_RAWCHUNK_H_
