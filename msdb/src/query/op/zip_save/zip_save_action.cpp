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
pArray zip_save_action::getOutArray(pArrayDesc desc)
{
	return std::make_shared<zipArray>(desc);
}

pArray zip_save_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	pArray outArray = this->executeIO(inputArrays, qry, IO_TYPE::SAVE);

	return inputArrays[0];
}
}		// core
}		// msdb