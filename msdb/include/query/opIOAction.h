#pragma once
#ifndef _MSDB_IOACTION_H_
#define _MSDB_IOACTION_H_

#include <pch.h>
#include <array/arrayId.h>
#include <array/attributeId.h>
#include <array/chunk.h>
#include <query/query.h>
#include <array/array.h>

namespace msdb
{
namespace core
{
class opIOAction;
using pEncodeAction = std::shared_ptr<opIOAction>;

class opIOAction : public std::enable_shared_from_this<opIOAction>
{
public:
	opIOAction();
	virtual ~opIOAction();

	virtual const char* name() = 0;

public:
	void encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId);
	size_t getSerializedChunkSize(attributeId attrId, pArray arr);
};
}
}
#endif	// _MSDB_IOACTION_H_
