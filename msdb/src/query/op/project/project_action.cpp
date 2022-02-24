#include <pch.h>
#include "project_action.h"

namespace msdb
{
namespace core
{
project_action::project_action()
{
}
project_action::~project_action()
{
}
const char* project_action::name()
{
	return "project";
}
pArray project_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pArray sourceArr = inputArrays[0];
	auto sourceArrDesc = sourceArr->getDesc();

	qry->setArrayDesc(sourceArrDesc);
	arrayId arrId = sourceArr->getId();

	auto strAttrParam = std::static_pointer_cast<opParamStringList::paramType>(this->params_[1]->getParam());
	auto attrDesc = sourceArr->getDesc()->getAttrDescs();
	for (auto strAttr : *strAttrParam)
	{

		for (auto attrDesc : *attrDesc)
		{
			if(attrDesc->name_ == strAttr)
			{

				break;
			}
		}
	}
	
	qry->getTimer()->pause(0);
	//========================================//
	return sourceArr;
}
}		// core
}		// msdb