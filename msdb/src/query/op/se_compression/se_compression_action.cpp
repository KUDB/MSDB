#include <pch.h>
#include "se_compression_action.h"
#include "seArray.h"
#include <system/storageMgr.h>

namespace msdb
{
namespace core
{
se_compression_action::se_compression_action()
{
}
se_compression_action::~se_compression_action()
{
}
const char* se_compression_action::name()
{
	return "se_compression";
}
pArray se_compression_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	//auto sourceArr = std::static_pointer_cast<wavelet_encode_array>(inputArrays[0]);
	pArray inArr = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArr->getDesc());
	pArray outArr = std::make_shared<seArray>(outArrDesc);

	auto arrId = inArr->getId();

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		if (attr->getCompType() != compressionType::SEACOW)
		{
			continue;
		}

		std::visit(
			visitHelper
			{
				[this, &outArr, &inArr, &attr, &qry](const auto& vType)
				{
					compressAttribute(vType, outArr, inArr, attr, qry);
				}
			}, 
			attr->getDataType());
	}
	qry->getTimer()->pause(0);
	//========================================//

	return inArr;
}

}		// core
}		// msdb