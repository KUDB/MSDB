#pragma once
#ifndef _MSDB_CPP_ARRAY_H_
#define _MSDB_CPP_ARRAY_H_

#include <pch.h>
#include <api_cpp/cpp_context.h.>
#include <array/array.h>

namespace msdb
{
class Array
{
public:
	Array(Context context, core::arrayId arrId);
	Array(Context context, std::string arrName);

private:
	Context context_;
	core::pArrayDesc desc_;
};

class ResultArray
{
public:
	ResultArray(Context context, core::pArray arr);

public:
	void close();

private:
	Context context_;
	core::pArray arr_;
};
}		// msdb
#endif	// _MSDB_CPP_ARRAY_H_