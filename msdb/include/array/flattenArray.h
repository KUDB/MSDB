#pragma once
#ifndef _MSDB_FLATTENARRAY_H_
#define _MSDB_FLATTENARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class flattenArray : public array
{
public:
	using base_type = array;

public:
	flattenArray(pArrayDesc desc);
	virtual ~flattenArray();

public:
	virtual void initChunkFactories();
	//virtual pChunk makeChunk(pChunkDesc desc) override;
};
}		// core
}		// msdb
#endif _MSDB_FLATTENARRAY_H_