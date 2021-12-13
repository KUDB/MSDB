#include <pch.h>
#include <array/array.h>
#include <util/math.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
arrayBase::arrayBase(pArrayDesc desc)
	: overallChunkBitmap_()
{
	this->desc_ = desc;
	this->overallChunkBitmap_ = std::make_shared<bitmap>(desc_->dimDescs_->getChunkSpace().area(), false);
	for (auto attr : *desc_->attrDescs_)
	{
		this->attrChunkBitmaps_[attr->id_] = std::make_shared<bitmap>(desc_->dimDescs_->getChunkSpace().area(), false);
	}
}
arrayBase::~arrayBase()
{
	//BOOST_LOG_TRIVIAL(debug) << "~arrayBase(): " << this->desc_->name_;
	this->chunks_.clear();
	this->overallChunkBitmap_ = nullptr;
	for (auto b : this->attrChunkBitmaps_)
	{
		b.second = nullptr;
	}
	this->desc_ = nullptr;
}
pArrayDesc arrayBase::getDesc()
{
	return this->desc_;
}
// TODO::required an attirubteId as an input parameter
pChunkIterator arrayBase::getChunkIterator(const attributeId attrId, const iterateMode itMode)
{
	return std::make_shared<chunkIterator>(this->desc_->dimDescs_->getChunkSpace(),
										   &(this->chunks_[attrId]), this->overallChunkBitmap_,
										   itMode);
}
arrayBase::size_type arrayBase::getNumChunks()
{
	return this->chunks_.size();
}
coor arrayBase::itemCoorToChunkCoor(const coor& itemCoor)
{
	coor chunkCoor(this->desc_->dimDescs_->size());
	for (dimensionId d = 0; d < this->desc_->dimDescs_->size(); d++)
	{
		chunkCoor[d] = floor(itemCoor[d] / (*this->desc_->dimDescs_)[d]->chunkSize_);
	}
	return chunkCoor;
}
pChunk arrayBase::insertChunk(const attributeId attrId, pChunk inputChunk)
{
	assert(attrId < this->desc_->attrDescs_->size());
	this->chunks_[attrId].insert(chunkPair(inputChunk->getId(), inputChunk));
	this->setChunkExist(attrId, inputChunk->getId());
	return inputChunk;
}

void arrayBase::flush()
{

}

pChunk arrayBase::makeChunk(const chunkDesc& desc)
{
	return this->makeChunk(desc.attrDesc_->id_, desc.id_);
}

void arrayBase::makeChunks(const attributeId attrId, const bitmap& input)
{
	chunkId capacity = this->getChunkIterator(attrId)->getCapacity();
	for(chunkId cid = 0; cid < capacity; ++cid)
	{
		if(input.isExist(cid) && !this->attrChunkBitmaps_[attrId]->isExist(cid))
		{
			this->makeChunk(attrId, cid);
		}
	}
}

pChunkDesc arrayBase::getChunkDesc(const attributeId attrId, const chunkId cId)
{
	dimension chunkDims = this->desc_->getDimDescs()->getChunkDims();
	dimension blockDims = this->desc_->getDimDescs()->getBlockDims();
	pAttributeDesc attrDesc = (*this->desc_->getAttrDescs())[attrId];
	auto cItr = this->getChunkIterator(attrId);
	coor chunkCoor = cItr->seqToCoor(cId);
	dimension sp = chunkDims * chunkCoor;
	dimension ep = sp + chunkDims;

	return std::make_shared<chunkDesc>(cId,
									   attrDesc,
									   chunkDims, blockDims,
									   sp, ep,
									   chunkDims.area() * attrDesc->typeSize_);
}

// TODO:: attributeId
pChunk arrayBase::getChunk(const attributeId attrId, const chunkId cId)
{
	assert(this->chunks_.find(attrId) != this->chunks_.end());
	return this->chunks_[attrId][cId];
}

arrayId arrayBase::getId()
{
	return this->desc_->id_;
}

void arrayBase::setId(const arrayId id)
{
	this->desc_->id_ = id;
}

chunkId arrayBase::getChunkId(pChunkDesc cDesc)
{
	return this->getChunkIdFromItemCoor(cDesc->sp_);
}

chunkId arrayBase::getChunkIdFromItemCoor(const coor& itemCoor)
{
	coor chunkCoor = itemCoor;
	for (dimensionId d = this->desc_->dimDescs_->size() - 1; d != -1; --d)
	{
		chunkCoor[d] /= this->desc_->dimDescs_->at(d)->chunkSize_;
	}
	return this->getChunkIdFromChunkCoor(chunkCoor);
}

chunkId arrayBase::getChunkIdFromChunkCoor(const coor& chunkCoor)
{
	chunkId id = 0;
	chunkId offset = 1;
	for (dimensionId d = this->desc_->dimDescs_->size() - 1; d != -1; d--)
	{
		id += offset * chunkCoor[d];
		offset *= (*this->desc_->dimDescs_)[d]->getChunkNum();
	}
	return id;
}
cpBitmap arrayBase::getChunkBitmap() const
{
	return this->overallChunkBitmap_;
}
void arrayBase::copyChunkBitmap(cpBitmap chunkBitmap)
{
	this->overallChunkBitmap_ = std::make_shared<bitmap>(*chunkBitmap);
}
void arrayBase::replaceChunkBitmap(pBitmap chunkBitmap)
{
	this->overallChunkBitmap_ = chunkBitmap;
}
void arrayBase::mergeChunkBitmap(pBitmap chunkBitmap)
{
	this->overallChunkBitmap_->andMerge(*chunkBitmap);
}
void arrayBase::print()
{
	for (auto attr : *this->getDesc()->attrDescs_)
	{
		auto cit = this->getChunkIterator(attr->id_);

		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				BOOST_LOG_TRIVIAL(debug) << "==============================\n";
				BOOST_LOG_TRIVIAL(trace) << "Chunk (" << cit->seqPos() << ") exist\n";
				(**cit)->print();
				BOOST_LOG_TRIVIAL(debug) << "==============================\n";
			}
			else
			{
				//BOOST_LOG_TRIVIAL(trace) << "==============================\n";
				//BOOST_LOG_TRIVIAL(trace) << "Chunk (" << cit->seqPos() << ") is not exist\n";
				//BOOST_LOG_TRIVIAL(trace) << "==============================\n";
			}

			++(*cit);
		}
	}
}
}		// core
}		// msdb