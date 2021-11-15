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
	return "insert";
}

pArray insert_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::IO);
	//========================================//

	pArray inArr = inputArrays[0];
	// pArray outArr = std::make_shared<memBlockArray>(this->getArrayDesc());

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		switch (attr->type_)
		{
		case eleType::CHAR:
			insertFromFile<char>(inArr);
			break;
		case eleType::INT8:
			insertFromFile<int8_t>(inArr);
			break;
		case eleType::INT16:
			insertFromFile<int16_t>(inArr);
			break;
		case eleType::INT32:
			insertFromFile<int32_t>(inArr);
			break;
		case eleType::INT64:
			insertFromFile<int64_t>(inArr);
			break;
		case eleType::UINT8:
			insertFromFile<uint8_t>(inArr);
			break;
		case eleType::UINT16:
			insertFromFile<uint16_t>(inArr);
			break;
		case eleType::UINT32:
			insertFromFile<uint32_t>(inArr);
			break;
		case eleType::UINT64:
			insertFromFile<uint64_t>(inArr);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	//========================================//
	qry->getTimer()->pause(0);
	//========================================//

	return inArr;
}
size_t insert_action::getBufferSize(dimension inArrDim, size_t typeSize)
{
	size_t capacity = 1;
	size_t dSize = inArrDim.size();
	for (int i = 0; i < dSize; ++i)
	{
		capacity *= inArrDim[i];
	}
	return typeSize * capacity;
}
}		// core
}		// msdb