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

//////////////////////////////////////////////////
// Read Image file in 'filePath'
// 
// - bytes: returns bytes of image file
// - length: returns number of pixels in image file 
//
// Exceptions:
//		- _MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_CANNOT_OPEN_FILE)
//
void readImageFile(const std::string& filePath, void** data, size_t& bytes, size_t& length);
//void readImageFile(const std::string filePath, std::shared_ptr<void>& data, size_t& bytes, size_t& length);
}		// core
}		// msdb
#endif	// _MSDB_IO_H_