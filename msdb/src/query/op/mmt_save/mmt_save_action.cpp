#include <pch.h>
#include <array/arrayMgr.h>
#include <system/storageMgr.h>
#include <op/mmt_save/mmt_save_action.h>
#include <index/mmt.h>

namespace msdb
{
namespace core
{
mmt_save_action::mmt_save_action()
{
}

mmt_save_action::~mmt_save_action()
{
}

const char* mmt_save_action::name()
{
	return "mmt_save";
}

pArray mmt_save_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	size_t mSizeTotal = 0;
	pArray arr = inputArrays[0];
	arrayId arrId = arr->getId();

	for (auto attr : *arr->getDesc()->attrDescs_)
	{
		auto arrIndex = arrayMgr::instance()->getAttributeIndex(arrId, attr->id_);
		if(arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		pMMT mmtIndex = std::static_pointer_cast<mmt>(arrIndex);
		// dynamic_pointer_cast / C2683
		
		//========================================//
		qry->getTimer()->nextWork(0, workType::IO);
		//----------------------------------------//
		storageMgr::instance()->saveAttrIndex(arrId, attr->id_, mmtIndex);

		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//

		mSizeTotal += mmtIndex->getSerializedSize();
	}

	std::cout << "mSizeTotal: " << mSizeTotal << std::endl;
	qry->getTimer()->pause(0);
	//========================================//

	return arr;
}
}		// core
}		// msdb