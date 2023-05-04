#include <pch_op.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/haar.h>
#include <util/math.h>
#include <util/dataType.h>

namespace msdb
{
namespace core
{
msdb::core::wavelet_encode_action::wavelet_encode_action()
{
	this->waveletName_ = "HaarSimple";
}

wavelet_encode_action::~wavelet_encode_action()
{
}

pArray wavelet_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pArray inArr = inputArrays[0];

	//////////////////////////////
	// Build wavelet_encode_array
	auto outArr = std::make_shared<wavelet_encode_array>(
		this->getArrayDesc());
	outArr->setOrigianlChunkDims(inArr->getDesc()->getDimDescs()->getChunkDims());

	// Unsigned valus should be treated as signed values.
	// Wavelet transform with unsigned values would increase bits of result array.
	// Not decrease to 0.
	// For example, -1 would treated as 254.
	// Then the values synopsis would increaseed.
	for(auto attrDesc : *inArr->getDesc()->attrDescs_)
	{
		std::visit(
			visitHelper
			{
				[this, &outArr, &inArr, &attrDesc, &qry](const auto& vType)
				{
					attributeEncode(vType, outArr, inArr, attrDesc, qry);
				}
			},
			attrDesc->getDataType());
	}
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

const char* msdb::core::wavelet_encode_action::name()
{
	return "wavelet_encode";
}
}		// core
}		// msdb