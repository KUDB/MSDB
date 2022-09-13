#include "project_action.h"
#include <boost/range/adaptor/reversed.hpp>

namespace msdb
{
namespace op
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

	if (this->params_[1]->type() != opParamType::STRING_LIST)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT, "Operator(Project) requires STRING_LIST for second parameter."));
	}

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pArray sourceArr = inputArrays[0];
	pArray outArr = sourceArr->shallowClone(true);

	qry->setArrayDesc(outArr->getDesc());

	// Set a flag for projected attributes
	auto strAttrParam = std::static_pointer_cast<opParamStringList::paramType>(this->params_[1]->getParam());
	auto outAttrDescs = outArr->getDesc()->getAttrDescs();
	std::vector<bool> projectAttrIds(outAttrDescs->size(), false);
	for (auto strAttr : *strAttrParam)
	{
		for(auto attrDesc : *outAttrDescs)
		{
			if (attrDesc->name_ == strAttr)
			{
				projectAttrIds[attrDesc->getId()] = true;
			}
		}
	}

	// Delete un-projected attributes
	for (attributeId attrId = outAttrDescs->size() - 1; attrId != (attributeId)-1; --attrId)
	{
		if (!projectAttrIds[attrId])
		{
			outArr->deleteAttribute(attrId);
		}
	}

	qry->getTimer()->pause(0);
	//========================================//
	return sourceArr;
}
}		// op
}		// msdb