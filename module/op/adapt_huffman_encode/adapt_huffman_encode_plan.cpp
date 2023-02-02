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

//////////////////////////////
// ParamSets
adapt_huffman_encode_array_pset::adapt_huffman_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
adapt_huffman_encode_plan_pset::adapt_huffman_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{

}
}		// core
}		// msdb