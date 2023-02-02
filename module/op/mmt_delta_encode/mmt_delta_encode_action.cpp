#include <pch.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>
#include <op/mmt_delta_encode/mmt_delta_encode_array.h>

namespace msdb
{
namespace core
{
mmt_delta_encode_action::mmt_delta_encode_action()
{
}

mmt_delta_encode_action::~mmt_delta_encode_action()
{
}

const char* mmt_delta_encode_action::name()
{
	return "msdb_delta_encode_action";
}

pArray mmt_delta_encode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	auto sourceArr = std::static_pointer_cast<wavelet_encode_array>(inputArrays[0]);
	arrayId arrId = sourceArr->getId();

	auto outArr = std::make_shared<wavelet_encode_array>(this->getArrayDesc());
	outArr->setLevel(sourceArr->getMaxLevel());
	outArr->setOrigianlChunkDims(sourceArr->getOrigianlChunkDims());

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		switch(attr->type_)
		{
		case eleType::CHAR:
			this->saveAttribute<char>(outArr, sourceArr, attr);
			break;
		case eleType::INT8:
			this->saveAttribute<int8_t>(outArr, sourceArr, attr);
			break;
		case eleType::INT16:
			this->saveAttribute<int16_t>(outArr, sourceArr, attr);
			break;
		case eleType::INT32:
			this->saveAttribute<int32_t>(outArr, sourceArr, attr);
			break;
		case eleType::INT64:
			this->saveAttribute<int64_t>(outArr, sourceArr, attr);
			break;
		case eleType::UINT8:
			this->saveAttribute<uint8_t>(outArr, sourceArr, attr);
			break;
		case eleType::UINT16:
			this->saveAttribute<uint16_t>(outArr, sourceArr, attr);
			break;
		case eleType::UINT32:
			this->saveAttribute<uint32_t>(outArr, sourceArr, attr);
			break;
		case eleType::UINT64:
			this->saveAttribute<uint64_t>(outArr, sourceArr, attr);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}
}		// core
}		// msdb