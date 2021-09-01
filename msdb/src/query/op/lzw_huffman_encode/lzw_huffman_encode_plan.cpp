#include <pch.h>
#include <op/lzw_huffman_encode/lzw_huffman_encode_plan.h>
#include <op/lzw_huffman_encode/lzw_huffman_encode_action.h>

namespace msdb
{
namespace core
{
lzw_huffman_encode_plan::lzw_huffman_encode_plan()
{
}
const char* lzw_huffman_encode_plan::name()
{
	return "lzw_huffman_encode_plan";
}
pAction lzw_huffman_encode_plan::makeAction()
{
	return std::make_shared<lzw_huffman_encode_action>();
}
lzw_huffman_encode_array_pset::lzw_huffman_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}		// core
}		// msdb