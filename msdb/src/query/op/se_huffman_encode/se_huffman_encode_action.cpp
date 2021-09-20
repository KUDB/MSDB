#include <pch.h>
#include <op/se_huffman_encode/se_huffman_encode_action.h>
#include <system/storageMgr.h>
#include <compression/seHuffmanChunk.h>
#include <util/logger.h>

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
pSeHuffmanChunk se_huffman_encode_action::makeOutChunk(pWtChunk inChunk)
{
	auto outChunkDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
	pSeHuffmanChunk outChunk = std::make_shared<seHuffmanChunk>(outChunkDesc);
	outChunk->setLevel(inChunk->getLevel());
	//outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
	outChunk->bufferRef(inChunk);
	outChunk->makeAllBlocks();
	
	return outChunk;
}
}		// core
}		// msdb