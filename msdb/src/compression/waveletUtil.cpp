#include <pch.h>
#include <compression/waveletUtil.h>

namespace msdb
{
namespace core
{
coorRange getBandRange(const size_t bandId, const coor& bandDims)
{
	size_t dSize = bandDims.size();
	coor bandSp(dSize);
	coor bandEp(bandDims);
	
	for (dimensionId d = 0; d < dSize; ++d)
	{
		if (bandId & (0x1 << (dSize - d - 1)))
		{
			bandSp[d] += bandDims[d];
			bandEp[d] += bandDims[d];
		}
	}
	return coorRange(bandSp, bandEp);
}

//pVecBlockInfo getWaveletBlockInfo(size_t maxLevel, size_t dSize, size_t blockId)
//{
//	static std::vector<std::vector<pVecBlockInfo>> infoStorage;
//
//	if(infoStorage.size() <= maxLevel)
//	{
//		infoStorage.resize(maxLevel + 1);
//	}
//
//	if(infoStorage[maxLevel].size() < dSize)
//	{
//		infoStorage[maxLevel].resize(dSize + 1, nullptr);
//	}
//
//	if(infoStorage[maxLevel][dSize] == nullptr)
//	{
//		infoStorage[maxLevel][dSize] = std::make_shared<pVecBlockInfo>();
//	}else
//	{
//		return infoStorage[maxLevel][dSize];
//	}
//
//	pVecBlockInfo curBlockInfo = infoStorage[maxLevel][dSize];
//	size_t width = pow(2, maxLevel + 1);
//	size_t numBlocks = pow(width, dSize);
//	curBlockInfo->resize(numBlocks);
//
//	for(size_t level = 0; level <= maxLevel; ++level)
//	{
//		for(size_t dim = 0; dim < dSize; ++dim)
//		{
//			for(size_t i = 0; i < width; ++i)
//			{
//				
//			}
//		}
//	}
//
//	return std::shared_ptr<std::vector<blockInfo>>();
//}
}		// core
}		// msdb