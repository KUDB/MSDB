#include <pch.h>
#include <op/se_compression/se_compression_action.h>
#include <array/memArray.h>
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
	return "se_compression_action";
}
pArray se_compression_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	auto sourceArr = std::static_pointer_cast<wavelet_encode_array>(inputArrays[0]);
	auto arrId = sourceArr->getId();

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		switch (attr->type_)
		{
		case eleType::CHAR:
			compressAttribute<char>(sourceArr, attr);
			break;
		case eleType::INT8:
			compressAttribute<int8_t>(sourceArr, attr);
			break;
		case eleType::INT16:
			compressAttribute<int16_t>(sourceArr, attr);
			break;
		case eleType::INT32:
			compressAttribute<int32_t>(sourceArr, attr);
			break;
		case eleType::INT64:
			compressAttribute<int64_t>(sourceArr, attr);
			break;
		case eleType::UINT8:
			compressAttribute<int8_t>(sourceArr, attr);
			break;
		case eleType::UINT16:
			compressAttribute<int16_t>(sourceArr, attr);
			break;
		case eleType::UINT32:
			compressAttribute<int32_t>(sourceArr, attr);
			break;
		case eleType::UINT64:
			compressAttribute<int64_t>(sourceArr, attr);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}
	qry->getTimer()->pause(0);
	//========================================//

	return std::static_pointer_cast<arrayBase>(sourceArr);
}
pSeChunk se_compression_action::makeOutChunk(pWtChunk inChunk)
{
	auto outDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
	pSeChunk outChunk = std::make_shared<seChunk>(outDesc);
	outChunk->setLevel(inChunk->getLevel());
	//outChunk->setSourceChunkId(inChunk->getSourceChunkId());
	outChunk->bufferRef(inChunk);
	outChunk->makeAllBlocks();

	return outChunk;
}
}		// core
}		// msdb