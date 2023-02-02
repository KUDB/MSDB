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
pArray lzw_huffman_encode_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<lzwHuffmanArray>(desc);
}

pArray lzw_huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::SAVE);

	return inputArrays[0];
}
}		// core
}		// msdb