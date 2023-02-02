#include <pch.h>
#include <op/mmt_delta_encode/mmt_delta_encode_array.h>

namespace msdb
{
namespace core
{
mmt_delta_encode_array::mmt_delta_encode_array(pArrayDesc desc)
	: base_type(desc), attrMMT()
{
}
pMMT mmt_delta_encode_array::getMMT(attributeId attrId)
{
	auto mmtIt = this->attrMMT.find(attrId);
	if(mmtIt == this->attrMMT.end())
	{
		return nullptr;
	}
	return this->attrMMT[attrId];
}
void mmt_delta_encode_array::setMMT(attributeId attrId, pMMT myMMT)
{
	this->attrMMT[attrId] = myMMT;
}
}		// core
}		// msdb