#include <pch.h>
#include <op/save/save_action.h>
#include <array/memArray.h>
#include <system/storageMgr.h>
#include <util/logger.h>

#include <op/encode_raw/encode_raw_action.h>

namespace msdb
{
namespace core
{
save_action::save_action()
{
}
save_action::~save_action()
{
}
const char* save_action::name()
{
	return "save";
}
pArray save_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	size_t mSizeTotal = 0;
	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//========================================//

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		switch (attr->compType_)
		{
		default:
			break;
		}
	}

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::IO);
	//========================================//

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(attr->id_, iterateMode::EXIST);

		while (!cit->isEnd())
		{
			pSerializable serialChunk
				= std::static_pointer_cast<serializable>(**cit);
			storageMgr::instance()->saveChunk(arrId, attr->id_, (**cit)->getId(),
													serialChunk);

			mSizeTotal += serialChunk->getSerializedSize();
			//std::cout << serialChunk->getSerializedSize() << std::endl;
			++(*cit);
		}
	}

	BOOST_LOG_TRIVIAL(debug) << "Total Save Chunk: " << mSizeTotal << " Bytes";

	//========================================//
	qry->getTimer()->pause(0);
	//========================================//

	return sourceArr;
}
}		// core
}		// msdb