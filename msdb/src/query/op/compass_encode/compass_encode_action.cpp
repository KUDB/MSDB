#include <pch.h>
#include <op/compass_encode/compass_encode_action.h>
#include <system/storageMgr.h>
#include <op/compass_encode/compassChunk.h>
#include <util/logger.h>
#include "compassArray.h"
#include <encoding/encodingParam.h>

namespace msdb
{
namespace core
{
compass_encode_action::compass_encode_action()
{
}
compass_encode_action::~compass_encode_action()
{
}
const char* compass_encode_action::name()
{
	return "compass_encode_action";
}
pArray compass_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	pArray inArr = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArr->getDesc());
	pArray outArr = std::make_shared<compassArray>(outArrDesc);
	arrayId arrId = inArr->getId();

	// Get Parameter - NumBin
	//eleDefault numBins;
	//std::static_pointer_cast<stableElement>(this->params_[1]->getParam())->getData(&numBins);

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		if (attr->getCompType() != encodingType::COMPASS)
		{
			continue;
		}

		size_t numBins = std::stoi(attr->getParam(_STR_PARAM_COMPASS_BINS_));

		std::visit(
			visitHelper
			{
				[this, &outArr, &inArr, &attr, &qry, &numBins](const auto& vType)
				{
					compressAttribute(vType, outArr, inArr, attr, qry, numBins);
				}
			},
			attr->getDataType());
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return inArr;
}

void compass_encode_action::encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId)
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