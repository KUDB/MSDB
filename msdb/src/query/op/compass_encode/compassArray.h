#pragma once
#ifndef _MSDB_COMPASSARRAY_H_
#define _MSDB_COMPASSARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class compassArray : public array
{
public:
	using base_type = array;

public:
	compassArray(pArrayDesc desc);
	virtual ~compassArray();

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_COMPASSARRAY_H_