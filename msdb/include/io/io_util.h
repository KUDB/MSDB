#pragma once
#ifndef _MSDB_IO_H_
#define _MSDB_IO_H_

#include <pch.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
std::stringstream::pos_type size_of_stream(const std::stringstream& ss);
}		// core
}		// msdb
#endif	// _MSDB_IO_H_