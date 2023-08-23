#pragma once
#ifndef _MSDB_PYTHON_ML_H_
#define _MSDB_PYTHON_ML_H_

#include "api_python_ml.h"

EXTERN_C{
	// CTypes string type: wchar_t (wchar_t should be converted to char in MSDB
	// Returns:
	// - If there is no array with arrName, it returns nullptr
	// - If the array exists, returns numpy array
	API_PYTHON_DLL void* getItem(wchar_t* arrName, unsigned int attrId, long long idx, size_t batchSize);
}	// EXTERN_C

#endif	// _MSDB_PYTHON_ML_H_