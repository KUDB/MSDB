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
static const int saveExpId = 4;
static const int loadExpId = 5;

msdb::Query executeInsertSaveArray(const std::string& arrName, const std::string& filePath,
								   const core::encodingType& compType = core::encodingType::NONE);

msdb::Query executeLoadArray(const std::string& arrName, const core::encodingType& compType);
}
}

#endif	// _MSDB_DUMMY_EXE_SAVE_ASTRONOMY_ARRAY_H_