#include <pch.h>
#include <op/tthresh_encode/tthresh_encode_action.h>
#include <op/tthresh_encode/tthreshArray.h>
#include <system/exceptions.h>
#include <system/storageMgr.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
tthresh_encode_action::tthresh_encode_action()
    : base_type()
{
}
tthresh_encode_action::~tthresh_encode_action()
{
}
const char* tthresh_encode_action::name()
{
    return "tthresh_encode_action";
}
pArray tthresh_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
    assert(inputArrays.size() == 1);

    //========================================//
    qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

    size_t mSizeTotal = 0;
    pArray sourceArr = inputArrays[0];
    pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
    pArray outArr = std::make_shared<tthreshArray>(outArrDesc);

    arrayId arrId = sourceArr->getId();

    for (auto attr : *sourceArr->getDesc()->attrDescs_)
    {
        // TODO::check compressionType
        // IF(compType != tthresh)
        //  ignore or exception
        auto cit = sourceArr->getChunkIterator(attr->id_, iterateMode::EXIST);
        while (!cit->isEnd())
        {
            auto cDesc = std::make_shared<chunkDesc>(*(*cit)->getDesc());
            auto outChunk = outArr->makeChunk(cDesc);
            outChunk->bufferRef(**cit);
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