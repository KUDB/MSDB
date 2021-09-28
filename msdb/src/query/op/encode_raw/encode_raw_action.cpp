#include <pch.h>
#include <op/encode_raw/encode_raw_action.h>
#include <array/memArray.h>
#include <system/storageMgr.h>
#include <util/logger.h>

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
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::IO);
	//========================================//

	size_t mSizeTotal = 0;
	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		auto cit = sourceArr->getChunkIterator(iterateMode::EXIST);

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