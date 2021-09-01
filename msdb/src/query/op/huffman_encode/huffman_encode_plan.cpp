#include <pch.h>
#include <op/huffman_encode/huffman_encode_plan.h>
#include <op/huffman_encode/huffman_encode_action.h>

namespace msdb
{
namespace core
{
huffman_encode_plan::huffman_encode_plan()
{
}
const char* huffman_encode_plan::name()
{
	return "huffman_encode_plan";
}
pAction huffman_encode_plan::makeAction()
{
	return std::make_shared<huffman_encode_action>();
}
huffman_encode_array_pset::huffman_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}		// core
}		// msdb