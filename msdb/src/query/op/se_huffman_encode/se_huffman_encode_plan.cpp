#include <pch.h>
#include <op/se_huffman_encode/se_huffman_encode_plan.h>
#include <op/se_huffman_encode/se_huffman_encode_action.h>

namespace msdb
{
namespace core
{
se_huffman_encode_plan::se_huffman_encode_plan()
{
}
const char* se_huffman_encode_plan::name()
{
	return "se_huffman_encode_plan";
}
pAction se_huffman_encode_plan::makeAction()
{
	return std::make_shared<se_huffman_encode_action>();
}
se_huffman_encode_array_pset::se_huffman_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}		// core
}		// msdb