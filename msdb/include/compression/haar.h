#pragma once
#ifndef _MSDB_HAAR_H_
#define _MSDB_HAAR_H_

#include <pch.h>

namespace msdb
{
namespace core
{
	namespace haar
	{
		void getWavelet(const double** h_0, const double** g_0,
						const double** h_1, const double** g_1,
						size_t* c, size_t* offset);
	}

	namespace haar_simple
	{
		void getWavelet(const double** h_0, const double** g_0,
						const double** h_1, const double** g_1,
						size_t* c, size_t* offset);
	}
}		// core
}		// msdb
#endif