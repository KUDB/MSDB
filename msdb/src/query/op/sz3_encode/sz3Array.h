#pragma once
#ifndef _MSDB_SZ3ARRAY_H_
#define _MSDB_SZ3ARRAY_H_

#include <pch.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class sz3Array : public array
{
public:
	using base_type = array;

public:
	sz3Array(pArrayDesc desc);
	virtual ~sz3Array();

public:
	virtual void initChunkFactories();
};
}		// core
}		// msdb
#endif _MSDB_SZ3ARRAY_H_