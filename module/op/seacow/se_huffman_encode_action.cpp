#include "se_huffman_encode_action.h"
#include <system/storageMgr.h>
#include <util/logger.h>
#include "seHuffmanChunk.h"
#include "seHuffmanArray.h"

namespace msdb
{
namespace core
{
se_huffman_encode_action::se_huffman_encode_action()
{
}
se_huffman_encode_action::~se_huffman_encode_action()
{
}
const char* se_huffman_encode_action::name()
{
	return "se_huffman_encode_action";
}
pArray se_huffman_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	auto inArr = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArr->getDesc());
	auto outArr = std::make_shared<seHuffmanArray>(outArrDesc);
	auto arrId = inArr->getId();

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		if (attr->getCompType() != encodingType::SEACOW_HUFFMAN)
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

	return std::static_pointer_cast<array>(inArr);
}
}		// core
}		// msdb