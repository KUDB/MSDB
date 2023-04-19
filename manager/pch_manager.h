#pragma once
#ifndef PCH_MANAGER_H
#define PCH_MANAGER_H

#ifdef MANAGER_EXPORTS
#define MANAGER_DLL_API __declspec(dllexport)
#else
#define MANAGER_DLL_API __declspec(dllimport)
#endif

#endif	// PCH_MANAGER_H