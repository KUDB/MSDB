#pragma once
#ifndef _MSDB_ZIPARRAY_H_
#define _MSDB_ZIPARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class zipArray : public array
{
public:
	using base_type = array;

public:
	zipArray(pArrayDesc desc);
	virtual ~zipArray();

public:
	pArray shallowClone(const bool takeOwnership = false) override;

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_ZIPARRAY_H_