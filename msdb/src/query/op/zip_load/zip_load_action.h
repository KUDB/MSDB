#pragma once
#ifndef _MSDB_OP_ZIP_ACTION_H_
#define _MSDB_OP_ZIP_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <compression/zipChunk.h>

namespace msdb
{
namespace core
{
class zip_load_action : public opAction
{
public:
	zip_load_action();
	virtual ~zip_load_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry);
	void loadChunk(pArray outArr, pChunk outChunk, pZipChunk inChunk, attributeId attrId, pQuery qry, const size_t parentThreadId);
	pZipChunk makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZIP_ACTION_H_