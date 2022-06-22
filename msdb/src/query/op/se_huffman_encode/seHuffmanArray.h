#pragma once
#ifndef _MSDB_SEHUFFMANARRAY_H_
#define _MSDB_SEHUFFMANARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class seHuffmanArray : public array
{
public:
	using base_type = array;

public:
	seHuffmanArray(pArrayDesc desc);
	virtual ~seHuffmanArray();

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_SEHUFFMANARRAY_H_