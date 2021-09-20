#include <pch.h>
#include <op/mmt_build/mmt_build_action.h>
#include <array/arrayMgr.h>
#include <index/mmt.h>

namespace msdb
{
namespace core
{
mmt_build_action::mmt_build_action()
{
}

mmt_build_action::~mmt_build_action()
{
}

const char* mmt_build_action::name()
{
	return "mmt_build_action";
}

pArray mmt_build_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	// Get parameters
	// Param 0
	pArray arr = inputArrays[0];
	// Param 1
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault refineLevel;
	ele->getData(&refineLevel);

	// Setup array
	arrayId arrId = arr->getId();
	auto dims = arr->getDesc()->getDimDescs()->getDims();
	auto chunkDims = arr->getDesc()->getDimDescs()->getChunkDims();
	auto blockDims = arr->getDesc()->getDimDescs()->getBlockDims();

	for (auto attr : *arr->getDesc()->getAttrDescs())
	{
		//pMMT mmtIndex = std::make_shared<mmt>(attr->type_, dims, chunkDims, refineLevel);
		pMMT mmtIndex = MinMaxTree::createMMT(attr->type_, dims, chunkDims, blockDims, refineLevel);
		auto cit = arr->getChunkIterator(iterateMode::EXIST);
		mmtIndex->build(cit);
		arrayMgr::instance()->setAttributeIndex(arrId, attr->id_, mmtIndex);
	}
	qry->getTimer()->pause(0);
	//========================================//

	return arr;
}
}		// core
}		// msdb