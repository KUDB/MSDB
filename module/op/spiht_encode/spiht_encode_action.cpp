#include <pch_op.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/monoChunk.h>
#include <op/spiht_encode/spihtChunk.h>
#include <util/logger.h>
#include "spihtArray.h"
#include <encoding/encodingParam.h>

namespace msdb
{
namespace core
{
spiht_encode_action::spiht_encode_action()
{
}

spiht_encode_action::~spiht_encode_action()
{
}

const char* spiht_encode_action::name()
{
	return "spiht_encode_action";
}

pArray spiht_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
    assert(inputArrays.size() == 1);

    //========================================//
    qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
    //----------------------------------------//

    size_t mSizeTotal = 0;
    pArray inArr = inputArrays[0];
    pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArr->getDesc());
    pArray outArr = std::make_shared<spihtArray>(outArrDesc);

    arrayId arrId = inArr->getId();

    for (auto attr : *inArr->getDesc()->attrDescs_)
    {
        if (attr->getCompType() != encodingType::SPIHT)
        {
            continue;
        }

        size_t level = std::stoi(attr->getParam(_STR_PARAM_WAVELET_LEVEL_));

        std::visit(
            visitHelper
            {
                [this, &outArr, &inArr, &attr, &qry, &level](const auto& vType)
                {
                    compressAttribute(vType, outArr, inArr, attr, qry, level);
                }
            },
            attr->getDataType());
    }

    BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";

    //----------------------------------------//
    qry->getTimer()->pause(0);
    //========================================//

    return inArr;
}
}		// core
}		// msdb