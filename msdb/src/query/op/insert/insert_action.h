#pragma once
#ifndef _MSDB_OP_INSERT_ACTION_H_
#define _MSDB_OP_INSERT_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <util/enumType.h>

namespace msdb
{
namespace core
{
class insert_action : public opAction
{
public:
	insert_action();
	~insert_action();
	
public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

private:
	size_t getBufferSize(dimension inArrDim, size_t typeSize);

	template<typename Ty_>
	void insertFromFile(pArray inArr, pAttributeDesc attr);

	template <typename Ty_>
	void insertFromMemory(pArray inArr, pAttributeDesc attr);

	/**
	 * length: number of Ty_ elements
	 */
	template<typename Ty_>
	void insertData(pArray inArr, pAttributeDesc attr, Ty_* data, size_t length);
};
}		// core
}		// msdb

#include "insert_action.hpp"

#endif // _MSDB_OP_INSERT_ACTION_H_
