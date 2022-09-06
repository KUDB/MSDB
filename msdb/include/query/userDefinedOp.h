#pragma once
#ifndef _MSDB_USERDEFINEDOP_H_
#define _MSDB_USERDEFINEDOP_H_

#include <pch.h>

#ifdef OP_EXPORTS
#define OP_DLL_API __declspec(dllexport)
#else
#define OP_DLL_API __declspec(dllimport)
#endif

#endif	// _MSDB_USERDEFINEDOP_H_