#pragma once
#ifndef _MSDB_ZFPARRAY_H_
#define _MSDB_ZFPARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class zfpArray : public array
{
public:
	using base_type = array;

public:
	zfpArray(pArrayDesc desc);
	virtual ~zfpArray();

public:
	pArray shallowClone(const bool takeOwnership = false) override;

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_ZFPARRAY_H_