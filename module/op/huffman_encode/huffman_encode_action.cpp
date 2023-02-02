#include <pch_op.h>
#include "huffman_encode_action.h""
#include "huffmanArray.h"

namespace msdb
{
namespace core
{
huffman_encode_action::huffman_encode_action()
	: opIOAction()
{
}
huffman_encode_action::~huffman_encode_action()
{
}
pArray huffman_encode_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<huffmanArray>(desc);
}

pArray huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::SAVE);

	return inputArrays[0];
}
}	// core
}	// msdb