#pragma once
#ifndef _MSDB_encodingType_H_
#define _MSDB_encodingType_H_

#include <pch.h>
#include <util/enumType.h>

namespace msdb
{
namespace core
{
ENUM_MACRO(encodingType, 
		   NONE, RAW, SEACOW, SEACOW_HUFFMAN, 
		   SPIHT, COMPASS, HUFFMAN, ADAPTHUFFMAN,
		   LZW_HUFFMAN, LZW, ZIP, ZFP, TTHRESH);
}		// core
}		// msdb
#endif	// _MSDB_encodingType_H_