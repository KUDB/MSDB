#pragma once
#ifndef _MSDB_SPIHTARRAY_H_
#define _MSDB_SPIHTARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class spihtArray : public array
{
public:
	using base_type = array;

public:
	spihtArray(pArrayDesc desc);
	virtual ~spihtArray();

public:
	pArray shallowClone(const bool takeOwnership = false) override;

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_SPIHTARRAY_H_