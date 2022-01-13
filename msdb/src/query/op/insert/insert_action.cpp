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

	// TODO::Fork according to insert param type
	// this->params_[1]->getParam()
	// pass attribute id


	auto typeParam = std::static_pointer_cast<opInsertType>(this->params_[1]->getParam());

	switch (*typeParam)
	{
	case opInsertType::FILE: 
	{
		for (auto attr : *inArr->getDesc()->attrDescs_)
		{
			switch (attr->type_)
			{
			case eleType::CHAR:
				insertFromFile<char>(inArr, attr);
				break;
			case eleType::INT8:
				insertFromFile<int8_t>(inArr, attr);
				break;
			case eleType::INT16:
				insertFromFile<int16_t>(inArr, attr);
				break;
			case eleType::INT32:
				insertFromFile<int32_t>(inArr, attr);
				break;
			case eleType::INT64:
				insertFromFile<int64_t>(inArr, attr);
				break;
			case eleType::UINT8:
				insertFromFile<uint8_t>(inArr, attr);
				break;
			case eleType::UINT16:
				insertFromFile<uint16_t>(inArr, attr);
				break;
			case eleType::UINT32:
				insertFromFile<uint32_t>(inArr, attr);
				break;
			case eleType::UINT64:
				insertFromFile<uint64_t>(inArr, attr);
				break;
			default:
				_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
			}
		}
		break;
	}
	case opInsertType::MEMORY:
	{
		for (auto attr : *inArr->getDesc()->attrDescs_)
		{
			switch (attr->type_)
			{
			case eleType::CHAR:
				insertFromMemory<char>(inArr, attr);
				break;
			case eleType::INT8:
				insertFromMemory<int8_t>(inArr, attr);
				break;
			case eleType::INT16:
				insertFromMemory<int16_t>(inArr, attr);
				break;
			case eleType::INT32:
				insertFromMemory<int32_t>(inArr, attr);
				break;
			case eleType::INT64:
				insertFromMemory<int64_t>(inArr, attr);
				break;
			case eleType::UINT8:
				insertFromMemory<uint8_t>(inArr, attr);
				break;
			case eleType::UINT16:
				insertFromMemory<uint16_t>(inArr, attr);
				break;
			case eleType::UINT32:
				insertFromMemory<uint32_t>(inArr, attr);
				break;
			case eleType::UINT64:
				insertFromMemory<uint64_t>(inArr, attr);
				break;
			default:
				_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
			}
		}
		break;
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