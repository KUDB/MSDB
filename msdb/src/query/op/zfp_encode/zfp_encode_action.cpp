#include <pch.h>
#include "zfp_encode_action.h"
#include "zfpArray.h"

namespace msdb
{
namespace core
{
zfp_encode_action::zfp_encode_action()
    : opIOAction()
{
}
zfp_encode_action::~zfp_encode_action()
{
}
pArray zfp_encode_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<zfpArray>(desc);
}

pArray zfp_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::SAVE);

	return inputArrays[0];
}
}		// core
}		// msdb