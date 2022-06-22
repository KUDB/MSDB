#pragma once
#ifndef _MSDB_COMPOSITEARRAY_H_
#define _MSDB_COMPOSITEARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class compositeArray : public array
{
public:

public:
	compositeArray(pArrayDesc desc);
	virtual ~compositeArray();

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif	// _MSDB_COMPOSITEARRAY_H_