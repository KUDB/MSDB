#include <pch.h>

#include <op/index_filter/index_filter_action.h>
#include <array/memBlockArray.h>

namespace msdb
{
namespace core
{
index_filter_action::index_filter_action()
{
}

index_filter_action::~index_filter_action()
{
}

const char* index_filter_action::name()
{
	return "index_filter";
}

pArray index_filter_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pArray inArr = inputArrays[0];
	auto inArrDesc = inArr->getDesc();
	auto outArrDesc = std::make_shared<arrayDesc>(*inArrDesc);
	auto outArr = std::make_shared<memBlockArray>(outArrDesc);

	auto inPredicate = std::static_pointer_cast<predicate>(this->params_[1]->getParam());

	auto attrDesc = inArr->getDesc()->getAttrDescs();
	for (auto attrDesc : *attrDesc)
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			this->attributeFilter<char>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::INT8:
			this->attributeFilter<int8_t>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::INT16:
			this->attributeFilter<int16_t>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::INT32:
			this->attributeFilter<int32_t>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::INT64:
			this->attributeFilter<int64_t>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::UINT8:
			this->attributeFilter<uint8_t>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::UINT16:
			this->attributeFilter<uint16_t>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::UINT32:
			this->attributeFilter<uint32_t>(outArr, inArr, attrDesc, inPredicate);
			break;
		case eleType::UINT64:
			this->attributeFilter<uint64_t>(outArr, inArr, attrDesc, inPredicate);
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