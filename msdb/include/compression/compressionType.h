#pragma once
#ifndef _MSDB_COMPRESSIONTYPE_H_
#define _MSDB_COMPRESSIONTYPE_H_

#include <pch.h>
#include <util/enumType.h>

namespace msdb
{
namespace core
{
ENUM_MACRO(compressionType, 
		   NONE, RAW, SEACOW, SEACOW_HUFFMAN, 
		   SPIHT, COMPASS, HUFFMAN, ADAPTHUFFMAN,
		   LZW_HUFFMAN, LZW, ZIP, ZFP, TTHRESH, SZ);
}		// core
}		// msdb
#endif	// _MSDB_COMPRESSIONTYPE_H_