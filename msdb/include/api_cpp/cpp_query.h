#pragma once
#ifndef _MSDB_API_CPP_QUERY_H_
#define _MSDB_API_CPP_QUERY_H_

#include <pch.h>
#include <api_cpp/cpp_array.h>
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
	Query();

public:
	ResultArray execute();
	inline Status getStatus() const
	{
		return this->status_;
	}

protected:
	Status status_;		// Initial status: READY
	std::shared_ptr<msdbQuery> qry_;
};

class AFLQuery : public Query
{
public:
	AFLQuery();
};
}		// msdb
#endif	// _MSDB_API_CPP_QUERY_H_