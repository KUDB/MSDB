#include <pch.h>
#include <op/huffman_decode/huffman_decode_plan.h>
#include <op/huffman_decode/huffman_decode_action.h>

namespace msdb
{
namespace core
{
huffman_decode_plan::huffman_decode_plan()
{
}
const char* huffman_decode_plan::name()
{
	return "huffman_decode_plan";
}
pAction huffman_decode_plan::makeAction()
{
	return std::make_shared<huffman_decode_action>();
}
huffman_decode_array_pset::huffman_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}		// core
}		// msdb