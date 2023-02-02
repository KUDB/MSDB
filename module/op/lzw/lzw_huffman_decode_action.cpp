#include "lzw_huffman_decode_action.h"
#include "lzwHuffmanArray.h"

namespace msdb
{
namespace core
{
lzw_huffman_decode_action::lzw_huffman_decode_action()
	: opIOAction()
{
}
lzw_huffman_decode_action::~lzw_huffman_decode_action()
{
}
pArray lzw_huffman_decode_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<lzwHuffmanArray>(desc);
}

pArray lzw_huffman_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::LOAD);

	return outArray;
}
}		// core
}		// msdb