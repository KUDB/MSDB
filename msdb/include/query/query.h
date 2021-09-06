#pragma once
#ifndef _MSDB_QUERY_H_
#define _MSDB_QUERY_H_

#include <pch.h>
#include <util/timer.h>
#include <util/status.h>
#include <array/arrayDesc.h>

namespace msdb
{
namespace core
{
class opPlan;
class query;
using pQuery = std::shared_ptr<query>;

struct outBuffer
{
	std::shared_ptr<void> buffer_;
	uint64_t size_;
};

class query : std::enable_shared_from_this<query>
{
public:
	query(std::shared_ptr<opPlan> qryPlan);

public:
	/** Submit the query to query manager */
	status submit();

	/** Process the query */
	status process();

	pTimer getTimer();
	void setArrayDesc(pArrayDesc arrDesc);
	pArrayDesc getArrayDesc();
	void setDimBuffer(std::shared_ptr<std::vector<coor>> buffer);
	void setAttrBuffer(attributeId attrId, outBuffer buffer);
	std::shared_ptr<std::vector<coor>> getDimBuffer();
	template <typename Ty_>
	std::shared_ptr<std::vector<Ty_>> getAttrBuffer(attributeId attrId)
	{
		return std::static_pointer_cast<std::vector<Ty_>>(attrBuffers_[attrId].buffer_);
	}

protected:
	pTimer timer_;
	pArrayDesc arrDesc_;
	std::shared_ptr<std::vector<coor>> dimBuffer_;
	std::map<attributeId, outBuffer> attrBuffers_;
	std::shared_ptr<opPlan> qryPlan_;
};
}		// core
}		// msdb
#endif