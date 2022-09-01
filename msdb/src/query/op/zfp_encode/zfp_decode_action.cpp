#include <pch.h>
#include "zfp_decode_action.h"
#include "zfpArray.h"

namespace msdb
{
namespace core
{
zfp_decode_action::zfp_decode_action()
    : opIOAction()
{
}
zfp_decode_action::~zfp_decode_action()
{
}
pArray zfp_decode_action::getOutArray(pArrayDesc desc)
{
    return std::make_shared<zfpArray>(desc);
}

pArray zfp_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
    pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::LOAD);

    return outArray;
}
}		// core
}		// msdb