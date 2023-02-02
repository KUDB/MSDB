#include "lzw_encode_action.h"
#include "lzwArray.h"

namespace msdb
{
namespace core
{
lzw_encode_action::lzw_encode_action()
{
}
lzw_encode_action::~lzw_encode_action()
{
}
pArray lzw_encode_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<lzwArray>(desc);
}

pArray lzw_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::SAVE);

	return inputArrays[0];
}
}		// core
}		// msdb