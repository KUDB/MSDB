#pragma once
#ifndef _MSDB_LZWHUFFMANARRAY_H_
#define _MSDB_LZWHUFFMANARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class lzwHuffmanArray : public array
{
public:
	using base_type = array;

public:
	lzwHuffmanArray(pArrayDesc desc);
	virtual ~lzwHuffmanArray();

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_LZWHUFFMANARRAY_H_