#include <pch.h>
#include <op/lzw_huffman_decode/lzw_huffman_decode_plan.h>
#include <op/lzw_huffman_decode/lzw_huffman_decode_action.h>

namespace msdb
{
namespace core
{
lzw_huffman_decode_plan::lzw_huffman_decode_plan()
{
}
const char* lzw_huffman_decode_plan::name()
{
	return "lzw_huffman_decode_plan";
}
pAction lzw_huffman_decode_plan::makeAction()
{
	return std::make_shared<lzw_huffman_decode_action>();
}
lzw_huffman_decode_array_pset::lzw_huffman_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
}
}		// core
}		// msdb