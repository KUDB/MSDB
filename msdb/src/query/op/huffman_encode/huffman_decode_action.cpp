#include <pch.h>
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

pArray huffman_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	if (inputArrays.size() != 1)
	{
		this->throwExceptionWrongInputArray(inputArrays.size());
	}

	pArray inArray = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArray->getDesc());
	pArray outArray = std::make_shared<huffmanArray>(outArrDesc);

	this->executeIO(inArray, outArray, qry, IO_TYPE::LOAD);

	return outArray;
}
}		// core
}		// msdb