#pragma once
#ifndef _MSDB_ERRORCODES_H_
#define _MSDB_ERRORCODES_H_

#include <pch.h>

namespace msdb
{
namespace core
{
	enum
	{
#define X(_name, _code, _msg)	_name = _code,
#include "errorCategories.inc"
#include "errors.inc"
#undef X
	};

	const char* getErrorCategoryMsg(int32_t code);
	const char* getErrorMsg(int32_t code);
}		// core
}		// msdb
#endif
