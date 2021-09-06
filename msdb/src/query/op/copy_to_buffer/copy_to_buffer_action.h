#pragma once
#ifndef _MSDB_OP_COPY_TO_BUFFER_ACTION_H_
#define _MSDB_OP_COPY_TO_BUFFER_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class copy_to_buffer_action : public opAction
{
public:
	copy_to_buffer_action();
	~copy_to_buffer_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery qry) override;
	void dimensionEncode(pArray inArr, pQuery qry);

private:
	template <class Ty_>
	void attributeEncode(pArray inArr, pAttributeDesc attrDesc, pQuery qry)
	{
		auto cItr = inArr->getChunkIterator();
		auto dSize = cItr->dSize();
		auto cSize = cItr->getSeqEnd();

		auto attrBuffer = std::make_shared<std::vector<Ty_>>();

		while (!cItr->isEnd())
		{
			if(cItr->isExist())
			{
				this->chunkEncode<Ty_>((**cItr), attrBuffer, qry);
			}
			++(*cItr);
		}

		qry->setAttrBuffer(
			attrDesc->id_, 
			{ 
				std::static_pointer_cast<void>(attrBuffer), 
				attrBuffer->size() 
			});
	}

	template <class Ty_>
	void chunkEncode(pChunk inChunk, std::shared_ptr<std::vector<Ty_>> attrBuffer, pQuery qry)
	{
		auto bItr = inChunk->getBlockIterator();
		while (!bItr->isEnd())
		{
			if(bItr->isExist())
			{
				this->blockEncode<Ty_>((**bItr), attrBuffer, qry);
			}
			++(*bItr);
		}
	}

	template <class Ty_>
	void blockEncode(pBlock inBlock, std::shared_ptr<std::vector<Ty_>> attrBuffer, pQuery qry)
	{
		auto itemItr = inBlock->getItemIterator();
		while (!itemItr->isEnd())
		{
			if(itemItr->isExist())
			{
				attrBuffer->push_back((**itemItr).get<Ty_>());
			}
			++(*itemItr);
		}
	}
};
}		// core
}		// msdb
#endif	// _MSDB_OP_COPY_TO_BUFFER_ACTION_H_