#include "lzw_decode_action.h"
#include "lzwArray.h"

namespace msdb
{
namespace core
{
lzw_decode_action::lzw_decode_action()
{
}
lzw_decode_action::~lzw_decode_action()
{
}
pArray lzw_decode_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared <lzwArray>(desc);
}

pArray lzw_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::LOAD);

	return outArray;
}
}		// core
}		// msdb