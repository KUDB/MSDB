#include <pch.h>
#include "lzw_huffman_encode_action.h"
#include "lzwHuffmanArray.h"

namespace msdb
{
namespace core
{
lzw_huffman_encode_action::lzw_huffman_encode_action()
	: opIOAction()
{
}
lzw_huffman_encode_action::~lzw_huffman_encode_action()
{
}

pArray lzw_huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	if (inputArrays.size() != 1)
	{
		this->throwExceptionWrongInputArray(inputArrays.size());
	}

	pArray inArray = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArray->getDesc());
	pArray outArray = std::make_shared<lzwHuffmanArray>(outArrDesc);

	this->executeIO(inArray, outArray, qry, IO_TYPE::SAVE);

	return inArray;
}
}		// core
}		// msdb