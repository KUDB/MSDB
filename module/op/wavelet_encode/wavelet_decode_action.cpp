#include <pch_op.h>
#include <op/wavelet_encode/wavelet_decode_action.h>
#include <op/wavelet_encode/wavelet_decode_array.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/wavelet.h>

namespace msdb
{
namespace core
{
wavelet_decode_action::wavelet_decode_action()
{
	this->waveletName_ = "HaarSimple";
}

wavelet_decode_action::~wavelet_decode_action()
{
}

pArray wavelet_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//
	auto planBitmap = this->getPlanInChunkBitmap();		
	pArray inArr = inputArrays[0];

	//////////////////////////////
	// Build wavelet_decode_array
	auto outArr = std::make_shared<wavelet_decode_array>(this->getArrayDesc());
	auto attrDescs = inArr->getDesc()->getAttrDescs();
	for (auto attrDesc : *attrDescs)
	{
		std::visit(
			visitHelper
			{
				[this, &inArr, &outArr, &attrDesc, &qry](const auto& vType)
				{
					attributeDecode(vType, outArr, inArr, attrDesc, qry);
				}
			},
			attrDesc->getDataType());
	}
	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

const char* wavelet_decode_action::name()
{
	return "wavelet_decode";
}
}		// core
}		// msdb