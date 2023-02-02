#include <pch_op.h>
#include <op/encode_raw/encode_raw_action.h>
#include <array/flattenArray.h>
#include <system/storageMgr.h>
#include <util/logger.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
encode_raw_action::encode_raw_action()
{
}
encode_raw_action::~encode_raw_action()
{
}
const char* encode_raw_action::name()
{
	return "encode_raw";
}
pArray encode_raw_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	size_t currentThreadId = 0;
	//========================================//
	qry->getTimer()->nextJob(currentThreadId, this->name(), workType::PARALLEL);
	//----------------------------------------//
	this->threadCreate();

	size_t mSizeTotal = 0;
	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(attr->id_, iterateMode::EXIST);

		while (!cit->isEnd())
		{
			////////////////////////////////////////
			// 1. Serialize::encodeChunk
			////////////////////////////////////////
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(**cit);
			storageMgr::instance()->saveChunk(arrId, attr->id_, (**cit)->getId(),
											  serialChunk);
			////////////////////////////////////////

			////////////////////////////////////////
			// 2. Parallel::encodeChunk
			////////////////////////////////////////
			io_service_->post(boost::bind(&encode_raw_action::encodeChunk, this,
										  arrId, attr->id_, **cit, qry, currentThreadId));
			////////////////////////////////////////

			//mSizeTotal += serialChunk->getSerializedSize();
			//std::cout << serialChunk->getSerializedSize() << std::endl;
			++(*cit);
		}
	}

	this->threadStop();
	this->threadJoin();

	//----------------------------------------//
	qry->getTimer()->nextWork(currentThreadId, workType::COMPUTING);
	//----------------------------------------//

	for (auto attrDesc : *sourceArr->getDesc()->attrDescs_)
	{
		auto ocit = sourceArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		while (!ocit->isEnd())
		{
			auto outChunk = (**ocit);
			mSizeTotal += outChunk->getSerializedSize();

			++(*ocit);
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return sourceArr;
}

void encode_raw_action::encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId)
{
	auto threadId = getThreadId() + 1;
	//========================================//
	qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::IO, std::string("chunk::") + std::to_string(outChunk->getId()));
	//----------------------------------------//
	storageMgr::instance()->saveChunk(arrId, attrId, outChunk->getId(), outChunk);
	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}
}		// core
}		// msdb