#pragma once
#ifndef _MSDB_IO_H_
#define _MSDB_IO_H_

#include <pch.h>
#include <system/exceptions.h>

#define _MSDB_TRY_IO_BEGIN	_MSDB_TRY_BEGIN
#define _MSDB_CATCH_IO_END										\
_MSDB_CATCH_ALL                                                 \
	throw _MSDB_EXCEPTIONS(MSDB_EC_IO_ERROR, MSDB_ER_ALL_IO_ERROR);   \
_MSDB_CATCH_END

#endif