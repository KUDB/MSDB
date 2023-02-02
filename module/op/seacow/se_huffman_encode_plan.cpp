#include "se_huffman_encode_plan.h"
#include "se_huffman_encode_action.h"

namespace msdb
{
namespace core
{
se_huffman_encode_plan::se_huffman_encode_plan()
{
}
const char* se_huffman_encode_plan::name()
{
	return "se_huffman_encode";
}
pAction se_huffman_encode_plan::makeAction()
{
	return std::make_shared<se_huffman_encode_action>();
}

//////////////////////////////
// ParamSets
se_huffman_encode_array_pset::se_huffman_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
se_huffman_encode_plan_pset::se_huffman_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{

}
}		// core
}		// msdb