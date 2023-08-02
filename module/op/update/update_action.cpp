#include <pch_op.h>
#include <op/update/update_action.h>
#include <array/flattenArray.h>
#include <array/flattenChunk.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
////////////////////////////////////////////////////////////
// Fast update action
//
fast_update_action::fast_update_action()
{}
fast_update_action::~fast_update_action()
{}
const char* fast_update_action::name()
{
	return "fast_update";
}
std::map<attributeId, std::string> fast_update_action::getFilePathMapper(const pArray inArr)
{
	std::map<attributeId, std::string> filePathMapper;

	switch (this->params_[1]->type())
	{
	case opParamType::STRING:
	{
		if (inArr->getDesc()->getAttrDescs()->size() != 1)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT,
				std::string("Array ") + inArr->getDesc()->name_ + " has " + std::to_string(inArr->getDesc()->getAttrDescs()->size()) + ", but takes 1 attribute."));
		}

		filePathMapper[0] = *std::static_pointer_cast<opParamString::paramType>(this->params_[1]->getParam());
		break;
	}
	case opParamType::CONTAINER:
	{
		filePathMapper = *std::static_pointer_cast<opParamContainer<attributeId, std::string>::paramType> (this->params_[1]->getParam());

		if (inArr->getDesc()->getAttrDescs()->size() != filePathMapper.size())
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT,
				std::string("Array ") + inArr->getDesc()->name_ + " has " + std::to_string(inArr->getDesc()->getAttrDescs()->size()) + ", but takes 1 attribute."));
		}
		break;
	}
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT,
			std::string("The 'Update' operator can only take STRING or CONTAINER.")));
	}

	return filePathMapper;
}

size_t fast_update_action::getBufferSize(const dimension inArrDim, const size_t typeSize)
{
	size_t capacity = 1;
	size_t dSize = inArrDim.size();
	for (int i = 0; i < dSize; ++i)
	{
		capacity *= inArrDim[i];
	}
	return typeSize * capacity;
}
pArray fast_update_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(this->params_.size() >= 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::IO);
	//========================================//

	pArray inArr = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArr->getDesc());
	pArray outArr = std::make_shared<flattenArray>(outArrDesc);

	auto arrId = inArr->getId();
	auto filePathMapper = this->getFilePathMapper(inArr);

	for (auto& attr : *inArr->getDesc()->getAttrDescs())
	{
		if (filePathMapper.find(attr->getId()) != filePathMapper.end())
		{
			std::string filePath = filePathMapper[attr->getId()];
			std::visit(
				visitHelper
				{
					[this, &outArr, &inArr, &attr, &filePath, &qry](const auto& vType)
					{
						this->updateFromFile(vType, outArr, inArr, attr, filePath, qry);
					}
				},
				attr->getDataType());
		}
	}

	//========================================//
	qry->getTimer()->pause(0);
	//========================================//

	return inArr;
}
}		// core
}		// msdb