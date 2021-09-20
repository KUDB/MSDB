#include <pch.h>
#include <op/mmt_delta_decode/mmt_delta_decode_array.h>
#include <array/blockChunk.h>

namespace msdb
{
namespace core
{
mmt_delta_decode_array::mmt_delta_decode_array(pArrayDesc desc)
	: base_type(desc)
{
}

mmt_delta_decode_array::~mmt_delta_decode_array()
{
}
}		// core
}		// msdb