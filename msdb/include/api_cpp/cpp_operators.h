#pragma once
#ifndef _MSDB_API_CPP_OPERATORS_H_
#define _MSDB_API_CPP_OPERATORS_H_

#include <pch.h>
#include <api_cpp/cpp_array.h>
#include <api_cpp/cpp_query.h>
#include <api_cpp/cpp_domain.h>

namespace msdb
{
class Load : public AFLQuery
{
public:
	Load(Array arr);
};

class Between : public AFLQuery
{
public:
	Between(Array arr, Domain d);
	Between(AFLQuery q, Domain d);

private:
	AFLQuery parentQuery_;
	Domain domain_;
};
}		// msdb
#endif	// _MSDB_API_CPP_OPERATORS_H_