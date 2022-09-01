#include <pch.h>
#include "zip_save_action.h"
#include "zipArray.h"

namespace msdb
{
namespace core
{
zip_save_action::zip_save_action()
	: opIOAction()
{
}
zip_save_action::~zip_save_action()
{
}

pArray zip_save_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	if (inputArrays.size() != 1)
	{
		this->throwExceptionWrongInputArray(inputArrays.size());
	}

	pArray inArray = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArray->getDesc());
	pArray outArray = std::make_shared<zipArray>(outArrDesc);

	this->executeIO(inArray, outArray, qry, IO_TYPE::SAVE);

	return inArray;
}
}		// core
}		// msdb