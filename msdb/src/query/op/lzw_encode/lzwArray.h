#pragma once
#ifndef _MSDB_LZWARRAY_H_
#define _MSDB_LZWARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class lzwArray : public array
{
public:
	using base_type = array;

public:
	lzwArray(pArrayDesc desc);
	virtual ~lzwArray();

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_LZWARRAY_H_