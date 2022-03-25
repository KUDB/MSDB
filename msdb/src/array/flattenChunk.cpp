#include <pch.h>
#include <array/flattenChunk.h>
#include <array/monoChunkBuffer.h>
#include <array/flattenChunk.h>

namespace msdb
{
namespace core
{
blockChunkItemIterator::blockChunkItemIterator(void* data, const eleType eType,
											   const dimension& dims,
											   const dimension& csP,
											   pBlockIterator bItr)
	: chunkItemIterator(data, eType, dims, csP),
	mdItr(dims.size(), dims.data()),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
	this->initBlockItemItr();
}

// Only visits exist blocks
void blockChunkItemIterator::next()
{
	if (this->bItr_->isEnd() || this->curBlockItemItr_ == nullptr)	return;
	this->front_ = false;

	if (this->curBlockItemItr_->isEnd())
	{
		do
		{
			++(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				this->curBlockItemItr_ = (**this->bItr_)->getItemIterator();
				this->curBlockItemItr_->moveToStart();
				this->moveToStart();

				break;
			}
		} while (!this->bItr_->isEnd());

		if (this->bItr_->isEnd())
		{
			this->end_ = true;
		}
	} else
	{
		base_type::next();
		this->curBlockItemItr_->next();
	}
}

void blockChunkItemIterator::prev()
{
	if (this->curBlockItemItr_ == nullptr) return;
	if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())	return;
	this->end_ = false;

	if (this->curBlockItemItr_->isFront())
	{
		do
		{
			--(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				this->curBlockItemItr_ = (**this->bItr_)->getItemIterator();
				this->curBlockItemItr_->moveToLast();
				this->moveToLast();

				break;
			}
		} while (!this->bItr_->isFront());

		if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())
		{
			this->front_ = true;
		}
	} else
	{
		base_type::prev();
		this->curBlockItemItr_->prev();
	}
}

element blockChunkItemIterator::getAtDimPos(position_t dimPos)
{
	return this->curBlockItemItr_->getAtDimPos(dimPos);
}

element blockChunkItemIterator::getAtSeqPos(position_t seqPos)
{
	return this->curBlockItemItr_->getAtSeqPos(seqPos);
}

element blockChunkItemIterator::operator*()
{
	return (**this->curBlockItemItr_);
}

void blockChunkItemIterator::initBlockItemItr()
{
	while (!this->bItr_->isExist() && !this->bItr_->isEnd())
	{
		++(*this->bItr_);
	}
	if (this->bItr_->isExist())
	{
		this->curBlockItemItr_ = (**this->bItr_)->getItemIterator();
	}
}

blockChunkItemRangeIterator::blockChunkItemRangeIterator(void* data, eleType eType,
														 const dimension& dims,
														 const range& range,
														 const dimension& csP,
														 pBlockIterator bItr)
	: chunkItemRangeIterator(data, eType, dims, range, csP),
	mdItr(dims.size(), dims.data()),
	bItr_(bItr), curBlockItemItr_(nullptr)
{
}

void blockChunkItemRangeIterator::next()
{
	if (this->bItr_->isEnd() || this->curBlockItemItr_ == nullptr)	return;
	this->front_ = false;

	if (this->curBlockItemItr_->isEnd())
	{
		do
		{
			++(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				auto bDesc = (**this->bItr_)->getDesc();
				auto bRange = range(bDesc->getIsp(), bDesc->getIep());
				auto qRange = range(this->sP_, this->eP_);

				if (bRange.isFullyInside(qRange))
				{
					// Inside, full scan
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(bRange);
				} else
				{
					// Intersect
					auto sp = getOutsideCoor(bDesc->getIsp(), this->sP_);
					auto ep = getInsideCoor(bDesc->getIep(), this->eP_);
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(range(sp, ep));
				}

				this->curBlockItemItr_->moveToStart();
				this->moveToStart();

				break;
			}
		} while (!this->bItr_->isEnd());

		if (this->bItr_->isEnd())
		{
			this->end_ = true;
		}
	} else
	{
		base_type::next();
		this->curBlockItemItr_->next();
	}
}

void blockChunkItemRangeIterator::prev()
{
	if (this->curBlockItemItr_ == nullptr) return;
	if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())	return;
	this->end_ = false;

	if (this->curBlockItemItr_->isFront())
	{
		do
		{
			--(*this->curBlockItemItr_);
			if (this->bItr_->isExist())
			{
				auto bDesc = (**this->bItr_)->getDesc();
				auto bRange = range(bDesc->getIsp(), bDesc->getIep());
				auto qRange = range(this->sP_, this->eP_);

				if (bRange.isFullyInside(qRange))
				{
					// Inside, full scan
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(bRange);
				} else
				{
					// Intersect
					auto sp = getOutsideCoor(bDesc->getIsp(), this->sP_);
					auto ep = getInsideCoor(bDesc->getIep(), this->eP_);
					this->curBlockItemItr_ = (**this->bItr_)->getItemRangeIterator(range(sp, ep));
				}

				this->curBlockItemItr_->moveToLast();
				this->moveToLast();

				break;
			}
		} while (!this->bItr_->isFront());

		if (this->bItr_->isFront() && this->curBlockItemItr_->isFront())
		{
			this->front_ = true;
		}
	} else
	{
		base_type::prev();
		this->curBlockItemItr_->prev();
	}
}

element blockChunkItemRangeIterator::getAtDimPos(position_t dimPos)
{
	return this->curBlockItemItr_->getAtDimPos(dimPos);
}

element blockChunkItemRangeIterator::getAtSeqPos(position_t seqPos)
{
	return this->curBlockItemItr_->getAtSeqPos(seqPos);
}

element blockChunkItemRangeIterator::operator*()
{
	return (**this->curBlockItemItr_);
}
}		// core
}		// msdb