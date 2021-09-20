#include <pch.h>
#include <op/compass_index_build/compass_index_build_action.h>
#include <array/arrayMgr.h>
#include <index/compass.h>

namespace msdb
{
namespace core
{
compass_index_build_action::compass_index_build_action()
{
}

compass_index_build_action::~compass_index_build_action()
{
}

const char* compass_index_build_action::name()
{
	return "compass_index_build_action";
}

pArray compass_index_build_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	////////////////////
	// Get parameters
	pArray arr = inputArrays[0];	// Param 0
	
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());	// Param 1
	eleDefault numBins;
	ele->getData(&numBins);

	arrayId arrId = arr->getId();

	////////////////////
	// Build index
	for (auto attr : *arr->getDesc()->getAttrDescs())
	{
		pCompassIndex cpIndex = compassIndex::createIndex(attr->type_, numBins);
		auto cit = arr->getChunkIterator(iterateMode::EXIST);
		cpIndex->build(cit);
		arrayMgr::instance()->setAttributeIndex(arrId, attr->id_, cpIndex);
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return arr;
}
}		// core
}		// msdb