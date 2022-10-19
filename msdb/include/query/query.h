#pragma once
#ifndef _MSDB_QUERY_H_
#define _MSDB_QUERY_H_

#include <pch.h>
#include <util/timer.h>
#include <util/status.h>
#include <array/array.h>

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

class query : public std::enable_shared_from_this<query>
{
public:
	query(std::shared_ptr<opPlan> qryPlan);

public:
	/** Submit the query to query manager */
	status submit();

	/** Process the query */
	status process();

	pTimer getTimer();
	pArrayDesc getArrayDesc();
	void setDimBuffer(std::shared_ptr<std::vector<coor>> buffer);
	void setAttrBuffer(attributeId attrId, outBuffer buffer);
	std::shared_ptr<std::vector<coor>> getDimBuffer();
	template <typename Ty_>
	std::shared_ptr<std::vector<Ty_>> getAttrBuffer(attributeId attrId)
	{
		return std::static_pointer_cast<std::vector<Ty_>>(attrBuffers_[attrId].buffer_);
	}

	inline void setErrorMsg(const std::string& errorMsg)
	{
		this->errorMsg_ = errorMsg;
	}
	inline const std::string& getErrorMsg()
	{
		return this->errorMsg_;
	}

	inline void setVerbose() { this->verbose_ = true; }
	inline void unsetVerbose() { this->verbose_ = false; }
	inline bool isVerbose() { return this->verbose_; }

	inline void setRawResultOut() { this->rawResultOut_ = true; }
	inline void unsetRawResultOut() { this->rawResultOut_ = false; }
	inline bool isRawResultOut() { return this->rawResultOut_; }

	inline void setIOOperator() 
	{ 
		this->ioOperator_ = true; 
	}
	inline void setIOBytes(const size_t ioBytes)
	{
		this->ioBytes_ = ioBytes;
	}
	inline bool isIOOperator()
	{
		return this->ioOperator_;
	}
	inline size_t getIOBytes()
	{
		return this->ioBytes_;
	}

	inline pArray getRawResult()
	{
		return this->outArr_;
	}

protected:
	pTimer timer_;
	pArrayDesc arrDesc_;		// infered array scheme
	pArray outArr_;				// query result array
	bool ioOperator_;
	size_t ioBytes_;
	std::shared_ptr<std::vector<coor>> dimBuffer_;
	std::map<attributeId, outBuffer> attrBuffers_;
	std::shared_ptr<opPlan> qryPlan_;
	std::string errorMsg_;
	bool verbose_;
	bool rawResultOut_;
};
}		// core
}		// msdb
#endif