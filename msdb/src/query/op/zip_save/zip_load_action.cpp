#include <pch.h>
#include "zip_load_action.h"
#include "zipArray.h"

namespace msdb
{
namespace core
{
zip_load_action::zip_load_action()
	: opIOAction()
{
}
zip_load_action::~zip_load_action()
{
}

pArray zip_load_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	if (inputArrays.size() != 1)
	{
		this->throwExceptionWrongInputArray(inputArrays.size());
	}

	pArray inArray = inputArrays[0];
	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*inArray->getDesc());
	pArray outArray = std::make_shared<zipArray>(outArrDesc);

	this->executeIO(inArray, outArray, qry, IO_TYPE::LOAD);

	return outArray;
}
}		// core
}		// msdb