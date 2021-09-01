#pragma once
#ifndef _MSDB_COMPRESSION_H_
#define _MSDB_COMPRESSION_H_

#include <pch.h>

namespace msdb
{
namespace core
{
enum class CompressionMethod
{
	NONE = 0,
	WAVELET,
	SPIHT,
	CA
};
}		// core
}		// msdb
#endif	// _MSDB_COMPRESSION_H_
