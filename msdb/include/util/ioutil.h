#pragma once
#ifndef _MSDB_IOUTIL_H_
#define _MSDB_IOUTIL_H_

#include <pch.h>

namespace msdb
{
namespace core
{
// TODO::MOVE TO io/io_util.h
inline size_t getSize(std::stringstream& ss)
{
	ss.seekg(0, std::ios::end);
	size_t ssSize = ss.tellg();
	ss.seekg(0, std::ios::beg);

	return ssSize;

	//// Get file size
	//std::streampos begin, end;
	//begin = input.tellg();
	//input.seekg(0, std::ios::end);
	//end = input.tellg();
	//fileLength = end - begin;

	//// Init file pointer
	//input.seekg(0, std::ios::beg);
}
}		// core
}		// msdb
#endif	// _MSDB_IOUTIL_H_