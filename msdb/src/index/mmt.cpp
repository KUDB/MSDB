#include <pch.h>
#include <index/mmt.h>

namespace msdb
{
namespace core
{
MinMaxTree::MinMaxTree(const eleType eType, size_const maxLevel)
	: attributeIndex(), serializable(std::make_shared<mmtHeader>()),
	eType_(eType), lowerLevel_(maxLevel)
{

}

eleType MinMaxTree::getEleType()
{
	return this->eType_;
}

MinMaxTree::size_type MinMaxTree::getMaxLevel()
{
	return this->maxLevel_;
}

pMMT MinMaxTree::createMMT(const eleType eType,
						   const dimension& dim,
						   const dimension& chunkDim,
						   const dimension& blockDim,
						   size_const maxLevel)
{
	switch (eType)
	{
	case eleType::CHAR:
		return std::make_shared<MinMaxTreeImpl<char>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::INT8:
		return std::make_shared<MinMaxTreeImpl<int8_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::INT16:
		return std::make_shared<MinMaxTreeImpl<int16_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::INT32:
		return std::make_shared<MinMaxTreeImpl<int32_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::INT64:
		return std::make_shared<MinMaxTreeImpl<int64_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::UINT8:
		return std::make_shared<MinMaxTreeImpl<uint8_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::UINT16:
		return std::make_shared<MinMaxTreeImpl<uint16_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::UINT32:
		return std::make_shared<MinMaxTreeImpl<uint32_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	case eleType::UINT64:
		return std::make_shared<MinMaxTreeImpl<uint64_t>>(eType, dim, chunkDim, blockDim, maxLevel);
	}
	//return std::make_shared<MinMaxTreeImpl<uint32_t>>(eType, dim, chunkDim, blockDim, maxLevel);

	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_UNKNOWN_ERROR, MSDB_ER_NOT_IMPLEMENTED));
}
}		// core
}		// msdb