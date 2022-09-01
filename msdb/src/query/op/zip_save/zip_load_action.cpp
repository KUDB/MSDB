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
pArray zip_load_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<zipArray>(desc);
}

pArray zip_load_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::LOAD);

	return outArray;
}
}		// core
}		// msdb