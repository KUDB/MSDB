#include <pch_op.h>
#include "huffman_decode_action.h"
#include "huffmanArray.h"

namespace msdb
{
namespace core
{
huffman_decode_action::huffman_decode_action()
	: opIOAction()
{
}
huffman_decode_action::~huffman_decode_action()
{
}
pArray huffman_decode_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<huffmanArray>(desc);
}

pArray huffman_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::LOAD);

	return outArray;
}
}		// core
}		// msdb