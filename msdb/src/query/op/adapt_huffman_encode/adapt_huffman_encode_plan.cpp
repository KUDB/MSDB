#include <pch.h>
#include <op/adapt_huffman_encode/adapt_huffman_encode_plan.h>
#include <op/adapt_huffman_encode/adapt_huffman_encode_action.h>

namespace msdb
{
namespace core
{
adapt_huffman_encode_plan::adapt_huffman_encode_plan()
{
}
const char* adapt_huffman_encode_plan::name()
{
	return "adapt_huffman_encode_plan";
}
pAction adapt_huffman_encode_plan::makeAction()
{
	return std::make_shared<adapt_huffman_encode_action>();
}
adapt_huffman_encode_array_pset::adapt_huffman_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}		// core
}		// msdb