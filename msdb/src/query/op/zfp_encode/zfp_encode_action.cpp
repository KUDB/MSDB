#include <pch.h>
#include <op/zfp_encode/zfp_encode_action.h>
#include <op/zfp_encode/zfpArray.h>
#include <system/exceptions.h>
#include <system/storageMgr.h>
#include <util/logger.h>

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

    //========================================//
    qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

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
            //if (cit->seqPos() != 27)
            //{
            //    ++(*cit);
            //    continue;
            //}
            //std::cout << "27" << std::endl;

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