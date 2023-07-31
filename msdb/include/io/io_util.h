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

void readImageFile(const std::string filePath, void** data, size_t& bytes, size_t& length);
//void readImageFile(const std::string filePath, std::shared_ptr<void>& data, size_t& bytes, size_t& length);
}		// core
}		// msdb
#endif	// _MSDB_IO_H_