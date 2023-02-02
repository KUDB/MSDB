#pragma once
#ifndef _MSDB_OP_COMPASS_ACTION_H_
#define _MSDB_OP_COMPASS_ACTION_H_

#include <query/opAction.h>
#include "compassChunk.h"

namespace msdb
{
namespace core
{
class compass_decode_action : public opAction
{
public:
	compass_decode_action();
	virtual ~compass_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
	void decodeAttribute(pArray outArr, pAttributeDesc attrDesc, size_t numBins, pQuery qry);
	void decodeChunk(pChunk outChunk, pQuery qry, pArray outArr,
					 const attributeId attrId, const size_t parentThreadId);
	//pCompassChunk makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid, eleDefault numBins);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_COMPASS_ACTION_H_