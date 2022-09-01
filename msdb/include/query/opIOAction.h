#pragma once
#ifndef _MSDB_IOACTION_H_
#define _MSDB_IOACTION_H_

#include <pch.h>
#include <array/attributeId.h>
#include <encoding/encodingType.h>
#include <array/chunk.h>
#include <array/array.h>
#include <query/opAction.h>
#include <query/query.h>

namespace msdb
{
namespace core
{
class opIOAction;
using pEncodeAction = std::shared_ptr<opIOAction>;

class opIOAction : public opAction
{
protected:
	enum class IO_TYPE {SAVE, LOAD};
public:
	opIOAction();
	virtual ~opIOAction();

	virtual const char* name() = 0;
	virtual int encodingType() = 0;
	virtual pArray getOutArray(pArrayDesc desc) = 0;

protected:
	pArray executeIO(std::vector<pArray>& inputArrays, pQuery qry, const IO_TYPE type);

	void saveAttribute(pArray inArr, pArray outArr, pAttributeDesc attrDesc, pQuery qry, const size_t curThreadId);
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry, const size_t curThreadId);

	void saveChunk(const arrayId arrId, const attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId);
	void loadChunk(const arrayId arrId, const attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId);
	size_t getSerializedChunkSize(attributeId attrId, pArray arr);

private:
	void throwExceptionWrongInputArray(const size_t actual, const size_t expect = 1);
};
}
}
#endif	// _MSDB_IOACTION_H_
