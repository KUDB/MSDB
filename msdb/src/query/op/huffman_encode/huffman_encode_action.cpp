#include <pch.h>
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

pArray huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	if (inputArrays.size() != 1)
	{
		this->throwExceptionWrongInputArray(inputArrays.size());
	}

	pArray inArray = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArray->getDesc());
	pArray outArray = std::make_shared<huffmanArray>(outArrDesc);

	this->executeIO(inArray, outArray, qry, IO_TYPE::SAVE);

	return inArray;
}
}	// core
}	// msdb