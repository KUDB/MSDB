#include "opParam.h"

namespace msdb
{
namespace core
{
//////////////////////////////
// opParamEnum
template <typename TyEnum_>
opParamEnum<TyEnum_>::opParamEnum(TyEnum_ value)
	: opParam()
{
	this->value_ = std::make_shared<TyEnum_>(value);
}

template <typename TyEnum_>
opParam::void_pointer opParamEnum<TyEnum_>::getParam()
{
	return this->value_;
}

template <typename TyEnum_>
opParamType opParamEnum<TyEnum_>::type()
{
	return opParamType::ENUM;
}

//////////////////////////////
// opParamContainer
template<typename TyKey_, typename TyValue_>
opParamContainer<TyKey_, TyValue_>::opParamContainer(pContainer container)
	: container_(container)
{
}
template<typename TyKey_, typename TyValue_>
opParam::void_pointer opParamContainer<TyKey_, TyValue_>::getParam()
{
	return this->container_;
}
template<typename TyKey_, typename TyValue_>
opParamType opParamContainer<TyKey_, TyValue_>::type()
{
	return opParamType::CONTAINER;
}
}		// core
}		// msdb