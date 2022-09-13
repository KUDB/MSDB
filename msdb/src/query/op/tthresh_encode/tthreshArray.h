#pragma once
#ifndef _MSDB_TTHRESHARRAY_H_
#define _MSDB_TTHRESHARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class tthreshArray : public array
{
public:
	using base_type = array;

public:
	tthreshArray(pArrayDesc desc);
	virtual ~tthreshArray();

public:
	pArray shallowClone(const bool takeOwnership = false) override;

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_ZFPARRAY_H_