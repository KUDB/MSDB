#include <pch.h>
#include <op/sz3_encode/sz3_encode_action.h>
#include <op/sz3_encode/sz3Array.h>
#include <system/exceptions.h>
#include <system/storageMgr.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
sz3_encode_action::sz3_encode_action()
    : base_type()
{
}
sz3_encode_action::~sz3_encode_action()
{
}
const char* sz3_encode_action::name()
{
    return "sz3_encode_action";
}
pArray sz3_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
    assert(inputArrays.size() == 1);

    //========================================//
    qry->getTimer()->nextJob(0, this->name(), workType::PARALLEL);
    //----------------------------------------//
    size_t currentThreadId = 0;
    this->threadCreate();

    size_t mSizeTotal = 0;
    pArray sourceArr = inputArrays[0];
    pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
    pArray outArr = std::make_shared<sz3Array>(outArrDesc);

    arrayId arrId = sourceArr->getId();

    for (auto attr : *sourceArr->getDesc()->attrDescs_)
    {
        // TODO::check compressionType
        // IF(compType != sz3)
        //  ignore or exception
        auto cit = sourceArr->getChunkIterator(attr->id_, iterateMode::EXIST);
        while (!cit->isEnd())
        {
            if (cit->isExist())
            {
                chunkId cid = cit->seqPos();

                auto cDesc = std::make_shared<chunkDesc>(*(*cit)->getDesc());
                auto outChunk = outArr->makeChunk(cDesc);
                outChunk->bufferRef(**cit);
                outChunk->makeAllBlocks();

                ////////////////////////////////////////
                // 1. Serialize::decompressChunk
                ////////////////////////////////////////
                this->compressChunk(arrId, attr, outChunk, currentThreadId);
                ////////////////////////////////////////

                ////////////////////////////////////////
                // 2. Parallel::decompressChunk
                ////////////////////////////////////////
                //io_service_->post(boost::bind(&sz3_encode_action::compressChunk, this,
                //                              arrId, attr, outChunk, currentThreadId));
                ////////////////////////////////////////
            }

            ++(*cit);
        }
    }

    this->threadStop();
    this->threadJoin();

    //----------------------------------------//
    qry->getTimer()->nextWork(0, workType::COMPUTING);
    //----------------------------------------//

    for (auto attr : *sourceArr->getDesc()->attrDescs_)
    {
        auto ocit = outArr->getChunkIterator(attr->id_, iterateMode::EXIST);
        while (!ocit->isEnd())
        {
            auto outChunk = (**ocit);
            mSizeTotal += outChunk->getSerializedSize();

            ++(*ocit);
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Total Save Chunk: " << mSizeTotal << " Bytes";
    
    //----------------------------------------//
    qry->getTimer()->pause(0);
    //========================================//

    return sourceArr;
}

void sz3_encode_action::compressChunk(arrayId arrId, pAttributeDesc attrDesc, pChunk outChunk, const size_t parentThreadId)
{
    pSerializable serialChunk
        = std::static_pointer_cast<serializable>(outChunk);
    storageMgr::instance()->saveChunk(arrId, attrDesc->id_, (outChunk)->getId(),
                                      serialChunk);
}
}		// core
}		// msdb