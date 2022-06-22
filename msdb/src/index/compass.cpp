#include <pch.h>
#include <index/compass.h>

namespace msdb
{
namespace core
{
pCompassIndex compassIndex::createIndex(const eleType eType, const size_type numBins)
{
	switch (eType)
	{
	case eleType::CHAR:
		return std::make_shared<compassIndexImpl<char>>(eType, numBins);
	case eleType::INT8:
		return std::make_shared<compassIndexImpl<int8_t>>(eType, numBins);
	case eleType::INT16:
		return std::make_shared<compassIndexImpl<int16_t>>(eType, numBins);
	case eleType::INT32:
		return std::make_shared<compassIndexImpl<int32_t>>(eType, numBins);
	case eleType::INT64:
		return std::make_shared<compassIndexImpl<int64_t>>(eType, numBins);
	case eleType::UINT8:
		return std::make_shared<compassIndexImpl<uint8_t>>(eType, numBins);
	case eleType::UINT16:
		return std::make_shared<compassIndexImpl<uint16_t>>(eType, numBins);
	case eleType::UINT32:
		return std::make_shared<compassIndexImpl<uint32_t>>(eType, numBins);
	case eleType::UINT64:
		return std::make_shared<compassIndexImpl<uint64_t>>(eType, numBins);
	}

	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_UNKNOWN_ERROR, MSDB_ER_NOT_IMPLEMENTED));
}

compassBlockIndex::compassBlockIndex(size_t numBins, size_t binValueRange, bool hasNegative, uint64_t negativeToPositive)
	: numBins_(numBins), binValueRange_(binValueRange), hasNegative_(hasNegative), negativeToPositive_(negativeToPositive)
{
	this->blockBins_.resize(this->numBins_, compassBin());
}
}		// core
}		// msdb