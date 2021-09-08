#include <pch.h>
#include <op/insert/insert_action.h>
#include <array/memBlockArray.h>
#include <array/blockChunk.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
insert_action::insert_action()
{
}

insert_action::~insert_action()
{
}

const char* insert_action::name()
{
	return "insert_action";
}

pArray insert_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray inArr = inputArrays[0];
	// pArray outArr = std::make_shared<memBlockArray>(this->getArrayDesc());

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		switch (attr->type_)
		{
		case eleType::CHAR:
			inserting<char>(inputArrays);
			break;
		case eleType::INT8:
			inserting<int8_t>(inputArrays);
			break;
		case eleType::INT16:
			inserting<int16_t>(inputArrays);
			break;
		case eleType::INT32:
			inserting<int32_t>(inputArrays);
			break;
		case eleType::INT64:
			inserting<int64_t>(inputArrays);
			break;
		case eleType::UINT8:
			inserting<uint8_t>(inputArrays);
			break;
		case eleType::UINT16:
			inserting<uint16_t>(inputArrays);
			break;
		case eleType::UINT32:
			inserting<uint32_t>(inputArrays);
			break;
		case eleType::UINT64:
			inserting<uint64_t>(inputArrays);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	return inArr;
}
}		// core
}		// msdb