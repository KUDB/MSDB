#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_
#define _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_

#include <pch.h>
#include <array/memBlockArray.h>

namespace msdb
{
namespace core
{
class mmt_delta_decode_array : public memBlockArray
{
public:
	using base_type = memBlockArray;

public:
	mmt_delta_decode_array(pArrayDesc desc);
	~mmt_delta_decode_array();
};
}		// core
}		// msdb
#endif		// _MSDB_OP_MMT_DELTA_DECODE_ARRAY_H_