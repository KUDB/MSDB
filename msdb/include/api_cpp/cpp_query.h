#pragma once
#ifndef _MSDB_API_CPP_QUERY_H_
#define _MSDB_API_CPP_QUERY_H_

#include <pch.h>
#include <api_cpp/cpp_array.h>
#include <api_cpp/cpp_operators.h>
#include <array/attributeId.h>
#include <query/query.h>

namespace msdb
{
using msdbQuery = core::query;

class Query
{
public:
	enum class Status
	{
		READY,
		INPROGRESS,
		COMPLETE,
		FAIL
	};

public:
	Query(std::shared_ptr<AFLOperator> afl);

public:
	ResultArray execute();
	inline Status getStatus() const
	{
		return this->status_;
	}
	core::pTimer getTimer();
	std::string strStatus();

	void setVerbose();
	void unsetVerbose();
	inline bool isVerbose() { return this->qry_->isVerbose(); }

	inline void setRawResultOut() { this->qry_->setRawResultOut(); }
	inline void unsetRawResultOut() { this->qry_->unsetRawResultOut(); }
	inline bool isRawResultOut() { return this->qry_->isRawResultOut(); }
	inline core::pArray getRawResult() { return this->qry_->getRawResult(); }

	inline core::pArrayDesc getArrayDesc()
	{
		return this->qry_->getArrayDesc();
	}

protected:
	Status status_;		// Initial status: READY
	core::pQuery qry_;
};
}		// msdb
#endif	// _MSDB_API_CPP_QUERY_H_