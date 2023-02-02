#pragma once
#ifndef _MSDB_HUFFMANARRAY_H_
#define _MSDB_HUFFMANARRAY_H_

#include <pch_op.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class huffmanArray : public array
{
public:
	using base_type = array;

public:
	huffmanArray(pArrayDesc desc);
	virtual ~huffmanArray();

public:
	pArray shallowClone(const bool takeOwnership = false) override;

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_HUFFMANARRAY_H_