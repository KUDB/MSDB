#include <pch.h>
#include <op/zfp_encode/zfp_encode_action.h>
#include <op/zfp_encode/zfpArray.h>
#include <system/exceptions.h>
#include <system/storageMgr.h>
#include <util/logger.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
zfp_encode_action::zfp_encode_action()
    : base_type()
{
}
zfp_encode_action::~zfp_encode_action()
{
}
const char* zfp_encode_action::name()
{
    return "zfp_encode_action";
}
pArray zfp_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
    assert(inputArrays.size() == 1);

    size_t currentThreadId = 0;
    //========================================//
    qry->getTimer()->nextJob(currentThreadId, this->name(), workType::PARALLEL);
    //----------------------------------------//
    this->threadCreate();

    size_t mSizeTotal = 0;
    pArray sourceArr = inputArrays[0];
    pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
    pArray outArr = std::make_shared<zfpArray>(outArrDesc);

    arrayId arrId = sourceArr->getId();

    for (auto attr : *sourceArr->getDesc()->attrDescs_)
    {
        if (attr->getCompType() != compressionType::ZFP)
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

                ////////////////////////////////////////
                // 1. Serialize::encodeChunk
                ////////////////////////////////////////
                //pSerializable serialChunk
                //    = std::static_pointer_cast<serializable>(outChunk);
                //storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
                //                                  serialChunk);
                ////////////////////////////////////////

                ////////////////////////////////////////
                // 2. Parallel::encodeChunk
                ////////////////////////////////////////
                io_service_->post(boost::bind(&zfp_encode_action::encodeChunk, this,
                                              arrId, attr->id_, outChunk, qry, currentThreadId));
                ////////////////////////////////////////
            }

            ++(*cit);
        }
    }

    this->threadStop();
    this->threadJoin();

    //----------------------------------------//
    qry->getTimer()->nextWork(currentThreadId, workType::COMPUTING);
    //----------------------------------------//
    for (auto attrDesc : *outArr->getDesc()->attrDescs_)
    {
        auto ocit = outArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
        while (!ocit->isEnd())
        {
            if (ocit->isExist())
            {
                auto outChunk = (**ocit);
                mSizeTotal += outChunk->getSerializedSize();
            }
            ++(*ocit);
        }
    }

    BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
    //========================================//
    qry->getTimer()->pause(currentThreadId);
    //========================================//

    return sourceArr;
}

void zfp_encode_action::encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId)
{
    auto threadId = getThreadId() + 1;
    //========================================//
    qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::IO, std::string("chunk::") + std::to_string(outChunk->getId()));
    //----------------------------------------//
    pSerializable serialChunk
        = std::static_pointer_cast<serializable>(outChunk);
    storageMgr::instance()->saveChunk(arrId, attrId, (outChunk)->getId(),
                                      serialChunk);
    //----------------------------------------//
    qry->getTimer()->pause(threadId);
    //========================================//
}
}		// core
}		// msdb