#include <pch.h>
#include <op/wavelet_decode/wavelet_decode_action.h>
#include <op/wavelet_decode/wavelet_decode_array.h>
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
	// Get parameters
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);
	pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());

	auto weArray = std::static_pointer_cast<wavelet_encode_array>(inArr);
	auto originalChunkDims = weArray->getOrigianlChunkDims();

	//////////////////////////////
	// Build wavelet_decode_array
	auto outArr = std::make_shared<wavelet_decode_array>(this->getArrayDesc(), maxLevel);
	auto outDimDesc = outArr->getDesc()->getDimDescs();
	for(dimensionId d = 0; d < outDimDesc->size(); ++d)
	{
		(*outArr->getDesc()->getDimDescs())[d]->chunkSize_ = originalChunkDims[d];
	}

	auto attrDescs = inArr->getDesc()->getAttrDescs();
	for (auto attrDesc : *attrDescs)
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			attributeDecode<char>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT8:
			attributeDecode<int8_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT16:
			attributeDecode<int16_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT32:
			attributeDecode<int32_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT64:
			attributeDecode<int64_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT8:
			attributeDecode<int8_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT16:
			attributeDecode<int16_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT32:
			attributeDecode<int32_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT64:
			attributeDecode<int64_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		//case eleType::DOUBLE:
		//	attributeDecode<double>(inChunk, arrRange,w, d, q);
		//	break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
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