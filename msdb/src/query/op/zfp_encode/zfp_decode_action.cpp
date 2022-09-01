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

pArray zfp_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
    if (inputArrays.size() != 1)
    {
        this->throwExceptionWrongInputArray(inputArrays.size());
    }

    pArray inArray = inputArrays[0];
    pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArray->getDesc());
    pArray outArray = std::make_shared<zfpArray>(outArrDesc);

    this->executeIO(inArray, outArray, qry, IO_TYPE::LOAD);

    return outArray;
}
}		// core
}		// msdb