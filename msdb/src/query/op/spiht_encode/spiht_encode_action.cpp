#include <pch.h>
#include <op/spiht_encode/spiht_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/monoChunk.h>
#include <op/spiht_encode/spihtChunk.h>
#include <util/logger.h>
#include "spihtArray.h"

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

    size_t mSizeTotal = 0;
    pArray sourceArr = inputArrays[0];
    pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
    pArray outArr = std::make_shared<spihtArray>(outArrDesc);

    arrayId arrId = sourceArr->getId();

    for (auto attr : *sourceArr->getDesc()->attrDescs_)
    {
        if (attr->getCompType() != compressionType::SPIHT)
        {
            continue;
        }
        auto cit = sourceArr->getChunkIterator(attr->id_, iterateMode::EXIST);
        while (!cit->isEnd())
        {
            if (cit->isExist())
            {
                auto cDesc = std::make_shared<chunkDesc>(*(*cit)->getDesc());
                auto outChunk = outArr->makeChunk(cDesc);
                outChunk->bufferCopy(**cit);
                outChunk->makeAllBlocks();

                //========================================//
                qry->getTimer()->nextWork(0, workType::IO);
                //----------------------------------------//
                pSerializable serialChunk
                    = std::static_pointer_cast<serializable>(outChunk);
                storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
                                                  serialChunk);

                //========================================//
                qry->getTimer()->nextWork(0, workType::COMPUTING);
                //----------------------------------------//
                mSizeTotal += serialChunk->getSerializedSize();
            }

            ++(*cit);
        }
    }

    BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
    qry->getTimer()->pause(0);
    //========================================//

    return sourceArr;
}
}		// core
}		// msdb