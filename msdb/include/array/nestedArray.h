#pragma once
#ifndef _MSDB_NESTEDARRAY_H_
#define _MSDB_NESTEDARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class nestedArray;
using pNestedArray = std::shared_ptr<nestedArray>;

class nestedArray : public array
{
public:
	using base_type = array;

public:
	nestedArray(pArrayDesc desc);
	virtual ~nestedArray();

public:
	virtual void initChunkFactories();
	//virtual pChunk makeChunk(pChunkDesc desc) override;
};
}	// core
}	// msdb

#endif _MSDB_NESTEDARRAY_H_