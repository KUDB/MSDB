#include <pch.h>
#include <op/copy_to_buffer/copy_to_buffer_action.h>

namespace msdb
{
namespace core
{
copy_to_buffer_action::copy_to_buffer_action()
{
}
copy_to_buffer_action::~copy_to_buffer_action()
{
}
const char* copy_to_buffer_action::name()
{
	return "copy_to_buffer";
}
pArray copy_to_buffer_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COPY);

	pArray sourceArr = inputArrays[0];
	auto sourceArrDesc = sourceArr->getDesc();

	qry->setArrayDesc(sourceArrDesc);
	arrayId arrId = sourceArr->getId();

	auto attrDesc = sourceArr->getDesc()->getAttrDescs();
	if(attrDesc->size())
	{
		this->dimensionEncode(sourceArr, qry);
	}

	for (auto attrDesc : *attrDesc)
	{
		switch (attrDesc->type_)
		{
		case eleType::CHAR:
			this->attributeEncode<char>(sourceArr, attrDesc, qry);
			break;
		case eleType::INT8:
			this->attributeEncode<int8_t>(sourceArr, attrDesc, qry);
			break;
		case eleType::INT16:
			this->attributeEncode<int16_t>(sourceArr, attrDesc, qry);
			break;
		case eleType::INT32:
			this->attributeEncode<int32_t>(sourceArr, attrDesc, qry);
			break;
		case eleType::INT64:
			this->attributeEncode<int64_t>(sourceArr, attrDesc, qry);
			break;
		case eleType::UINT8:
			this->attributeEncode<uint8_t>(sourceArr, attrDesc, qry);
			break;
		case eleType::UINT16:
			this->attributeEncode<uint16_t>(sourceArr, attrDesc, qry);
			break;
		case eleType::UINT32:
			this->attributeEncode<uint32_t>(sourceArr, attrDesc, qry);
			break;
		case eleType::UINT64:
			this->attributeEncode<uint64_t>(sourceArr, attrDesc, qry);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	qry->getTimer()->pause(0);
	//========================================//
	return sourceArr;
}
void copy_to_buffer_action::dimensionEncode(pArray inArr, pQuery qry)
{
	auto cItr = inArr->getChunkIterator();
	auto dSize = cItr->dSize();
	auto cSize = cItr->getSeqEnd();

	auto dimBuffer = std::make_shared<std::vector<coor>>();

	while (!cItr->isEnd())
	{
		if (cItr->isExist())
		{
			pChunk inChunk = (**cItr);
			auto bItr = inChunk->getBlockIterator();
			while (!bItr->isEnd())
			{
				if (bItr->isExist())
				{
					pBlock inBlock = (**bItr);
					auto itemItr = inBlock->getItemIterator();
					while (!itemItr->isEnd())
					{
						if (itemItr->isExist())
						{
							dimBuffer->push_back(itemItr->coor());
						}
						++(*itemItr);
					}
				}
				++(*bItr);
			}
		}
		++(*cItr);
	}

	qry->setDimBuffer(dimBuffer);
}
}		// core
}		// msdb