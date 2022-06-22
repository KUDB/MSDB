#pragma once
#ifndef _MSDB_SEARRAY_H_
#define _MSDB_SEARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class seArray : public array
{
public:
	using base_type = array;

public:
	seArray(pArrayDesc desc);
	virtual ~seArray();

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_SEARRAY_H_