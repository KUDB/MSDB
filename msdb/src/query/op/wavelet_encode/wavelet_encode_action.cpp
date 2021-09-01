#include <pch.h>
#include <op/wavelet_encode/wavelet_encode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/haar.h>
#include <util/math.h>

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
	// Get parameters
	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);
	pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());

	//////////////////////////////
	// Build wavelet_encode_array
	auto outArr = std::make_shared<wavelet_encode_array>(
		this->getArrayDesc());
	outArr->setLevel(maxLevel);
	outArr->setOrigianlChunkDims(inArr->getDesc()->getDimDescs()->getChunkDims());

	// maxLevel value is checked in wavelet_encode_array constructor
	// which can be used for current array.
	maxLevel = outArr->getMaxLevel();

	// Unsigned valus should be treated as signed values.
	// Wavelet transform with unsigned values would increase bits of result array.
	// Not decrease to 0.
	// For example, -1 would treated as 254.
	// Then the values synopsis would increaseed.
	for(auto attrDesc : *inArr->getDesc()->getAttrDescs())
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			attributeEncode<char>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT8:
			attributeEncode<int8_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT16:
			attributeEncode<int16_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT32:
			attributeEncode<int32_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::INT64:
			attributeEncode<int64_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT8:
			attributeEncode<int8_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT16:
			attributeEncode<int16_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT32:
			attributeEncode<int32_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		case eleType::UINT64:
			attributeEncode<int64_t>(outArr, inArr, attrDesc, w, maxLevel, qry);
			break;
		//case eleType::DOUBLE:
		//	attributeEncode<double>(inChunk, arrRange,w, d, q);
		//	break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
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