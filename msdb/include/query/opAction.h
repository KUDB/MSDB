#pragma once
#ifndef _MSDB_OPACTION_H_
#define _MSDB_OPACTION_H_

#include <pch.h>
#include <array/array.h>
#include <query/opParam.h>
#include <query/query.h>

namespace msdb
{
namespace core
{
class opAction;
using pAction = std::shared_ptr<opAction>;

class opAction : public std::enable_shared_from_this<opAction>
{
public:
	opAction();
	virtual ~opAction();

public:
	const pArrayDesc getArrayDesc();
	void setParams(const parameters& params);
	void setArrayDesc(pArrayDesc aDesc);
	void setPlanInBitmap(pBitmap planBitmap);
	void setPlanOutBitmap(pBitmap planBitmap);
	cpBitmap getPlanInChunkBitmap() const;
	cpBitmap getPlanOutChunkBitmap() const;
	void getArrayStatus(pArray arr);

	virtual cpBitmap getPlanBlockBitmap(chunkId cid) const;
	virtual const char* name() = 0;

	virtual pArray execute(std::vector<pArray>&inputArrays, pQuery q) = 0;

protected:
	pArrayDesc aDesc_;
	pBitmap planInBitmap_;	// chunk bitmap or chunk-block bitmap tree
	pBitmap planOutBitmap_;	// chunk bitmap or chunk-block bitmap tree
	//std::vector<action> sources_;
	parameters params_;

	void threadCreate(size_t threadNums);
	void threadStop();
	void threadJoin();

protected:
	bool threadExist_;
	size_t threadNums_;
	std::shared_ptr<boost::asio::io_service> io_service_;
	std::shared_ptr<boost::asio::io_service::work> work_;
	boost::thread_group threadpool_;
};
}		// core
}		// msdb
#endif	// _MSDB_OPACTION_H_