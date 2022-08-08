#include <pch.h>
#include <op/wavelet_encode/fast_wavelet_decode_action.h>
#include <op/wavelet_encode/wavelet_decode_array.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/wavelet.h>

namespace msdb
{
namespace core
{
fast_wavelet_decode_action::fast_wavelet_decode_action()
{
	this->waveletName_ = "HaarSimple";
}

fast_wavelet_decode_action::~fast_wavelet_decode_action()
{
}

pArray fast_wavelet_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	auto planBitmap = this->getPlanInChunkBitmap();
	pArray inArr = inputArrays[0];

	//////////////////////////////
	// Get parameters
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);
	//pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());

	auto weArray = std::static_pointer_cast<wavelet_encode_array>(inArr);
	auto originalChunkDims = weArray->getOrigianlChunkDims();

	//////////////////////////////
	// Build wavelet_decode_array
	auto outArr = std::make_shared<wavelet_decode_array>(this->getArrayDesc(), maxLevel);
	auto outDimDesc = outArr->getDesc()->getDimDescs();
	for (dimensionId d = 0; d < outDimDesc->size(); ++d)
	{
		(*outArr->getDesc()->getDimDescs())[d]->chunkSize_ = originalChunkDims[d];
	}

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

const char* fast_wavelet_decode_action::name()
{
	return "fast_wavelet_decode";
}
}		// core
}		// msdb