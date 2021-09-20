#include <pch.h>
#include <op/mmt_load/mmt_load_action.h>
#include <array/arrayMgr.h>
#include <index/mmt.h>
#include <system/storageMgr.h>

namespace msdb
{
namespace core
{
mmt_load_action::mmt_load_action()
{
}

mmt_load_action::~mmt_load_action()
{
}

const char* mmt_load_action::name()
{
	return "mmt_load_action";
}

pArray mmt_load_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pArray arr = inputArrays[0];
	auto dims = arr->getDesc()->getDimDescs()->getDims();
	auto chunkDims = arr->getDesc()->getDimDescs()->getChunkDims();
	auto blockDims = arr->getDesc()->getDimDescs()->getBlockDims();

	arrayId arrId = arr->getId();

	for(auto attr : *arr->getDesc()->getAttrDescs())
	{
		pMMT mmtIndex = MinMaxTree::createMMT(attr->type_, dims, chunkDims, blockDims);		// maxLevel is not setted

		//========================================//
		qry->getTimer()->nextWork(0, workType::IO);
		//----------------------------------------//
		storageMgr::instance()->loadAttrIndex(arrId, attr->id_, mmtIndex);

		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//
		arrayMgr::instance()->setAttributeIndex(arrId, attr->id_, mmtIndex);
	}
	qry->getTimer()->pause(0);
	//========================================//

	return arr;
}
}		// core
}		// msdb