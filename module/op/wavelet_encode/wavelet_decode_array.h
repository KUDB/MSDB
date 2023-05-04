#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_ARRAY_H_
#define _MSDB_OP_WAVELET_DECODE_ARRAY_H_

#include <pch_op.h>
#include <array/flattenArray.h>

namespace msdb
{
namespace core
{
class wavelet_decode_array : public flattenArray
{
public:
	using base_type = flattenArray;

public:
	wavelet_decode_array(pArrayDesc desc);
	virtual ~wavelet_decode_array();
};
}		// core
}		// msdb
#endif	// _MSDB_OP_WAVELET_DECODE_ARRAY_H_