#pragma once
#ifndef _MSDB_OP_BETWEEN_ACTION_H_
#define _MSDB_OP_BETWEEN_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
	class between_action : public opAction
	{
	public:
		between_action();
		virtual ~between_action();
		virtual const char* name() override;

	public:
		pArray execute(std::vector<pArray>& inputArrays, pQuery q);

	private:
		void betweenChunk(pChunk outChunk, pChunk inChunk, coorRange& betweenRange);
		void fullyInsideChunk(pChunk outChunk, pChunk inChunk);
		void betweenBlock(pBlock outBlock, pBlock inBlock, coorRange& betweenRangeInChunk);
		void fullyInsideBlock(pBlock outBlock, pBlock inBlock);
	};
}		// core
}		// msdb
#endif // _MSDB_OP_BETWEEN_ACTION_H_
