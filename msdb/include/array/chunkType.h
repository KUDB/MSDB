#pragma once
#ifndef _MSDB_ARRAY_CHUNKTYPE_H_
#define _MSDB_ARRAY_CHUNKTYPE_H_

#include <pch.h>
#include <array/chunkFactory.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// chunkType
//////////////////////////////
// 
// - hold name of chunk and its factory
//
class chunkType
{
public:
	chunkType(pChunkFactory myFactory);
	virtual ~chunkType();

public:
	virtual std::string name() = 0;
	inline pChunkFactory getChunkFactory()
	{
		return this->myFactory_;
	}

protected:
	pChunkFactory myFactory_;
};

template <class _FactoryBuilder>
pChunkFactory buildFactory(_FactoryBuilder&& _fConstructor, const dataType& type)
{
	return std::visit(_fConstructor, type);
}
}		// core
}		// msdb
#endif	// _MSDB_ARRAY_CHUNKTYPE_H_