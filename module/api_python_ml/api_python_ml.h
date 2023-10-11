#pragma once
#ifndef _MSDB_API_PYTHON_ML_H_
#define _MSDB_API_PYTHON_ML_H_

#ifdef API_PYTHON_ML_EXPORTS
#define API_PYTHON_DLL	__declspec(dllexport)
#else
#define API_PYTHON_DLL	__declspec(dllimport)
#endif

#define EXTERN_C	extern "C"

#endif // _MSDB_API_PYTHON_ML_H_