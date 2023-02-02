#include <pch.h>
#include <op/adapt_huffman_decode/adapt_huffman_decode_plan.h>
#include <op/adapt_huffman_decode/adapt_huffman_decode_action.h>

namespace msdb
{
namespace core
{
adapt_huffman_decode_plan::adapt_huffman_decode_plan()
{
}
const char* adapt_huffman_decode_plan::name()
{
	return "adapt_huffman_decode_plan";
}
pAction adapt_huffman_decode_plan::makeAction()
{
	return std::make_shared<adapt_huffman_decode_action>();
}
adapt_huffman_decode_array_pset::adapt_huffman_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}		// core
}		// msdb