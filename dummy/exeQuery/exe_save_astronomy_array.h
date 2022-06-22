#pragma once
#ifndef _MSDB_DUMMY_EXE_SAVE_ASTRONOMY_ARRAY_H_
#define _MSDB_DUMMY_EXE_SAVE_ASTRONOMY_ARRAY_H_

#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy.h>
#include <dummy_astronomy_array.h>

namespace msdb
{
namespace dummy
{
msdb::Query executeInsertSaveArray(const std::string& arrName, const std::string& filePath,
								   const core::compressionType& compType = core::compressionType::NONE);

msdb::Query executeLoadArray(const std::string& arrName, const core::compressionType& compType);
}
}

#endif	// _MSDB_DUMMY_EXE_SAVE_ASTRONOMY_ARRAY_H_