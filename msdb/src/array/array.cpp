#include <pch.h>
#include <array/array.h>
#include <util/math.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
array::array(pArrayDesc desc)
	: overallChunkBitmap_()
{
	this->desc_ = desc;
	this->overallChunkBitmap_ = std::make_shared<bitmap>(desc_->dimDescs_->getChunkSpace().area(), false);
	for (auto attr : *desc_->attrDescs_)
	{
		this->attrChunkBitmaps_[attr->id_] = std::make_shared<bitmap>(desc_->dimDescs_->getChunkSpace().area(), false);
	}
}
array::~array()
{
	//BOOST_LOG_TRIVIAL(debug) << "~array(): " << this->desc_->name_;
	this->chunks_.clear();
	this->overallChunkBitmap_ = nullptr;
	for (auto b : this->attrChunkBitmaps_)
	{
		b.second = nullptr;
	}
	this->desc_ = nullptr;
}
pArrayDesc array::getDesc()
{
	return this->desc_;
}
// TODO::required an attirubteId as an input parameter
pChunkIterator array::getChunkIterator(const attributeId attrId, const iterateMode itMode)
{
	return std::make_shared<chunkIterator>(this->desc_->dimDescs_->getChunkSpace(),
										   &(this->chunks_[attrId]), this->overallChunkBitmap_,
										   itMode);

	//if (this->chunks_.find(attrId) != this->chunks_.end())
	//{

	//}
	//else
	//{
	//	return nullptr;
	//}								   
}
//array::size_type array::getNumChunks(const attributeId attrId)
//{
//	return this->chunks_[attrId].size();
//}
coor array::itemCoorToChunkCoor(const coor& itemCoor)
{
	coor chunkCoor(this->desc_->dimDescs_->size());
	for (dimensionId d = 0; d < this->desc_->dimDescs_->size(); d++)
	{
		chunkCoor[d] = floor(itemCoor[d] / (*this->desc_->dimDescs_)[d]->chunkSize_);
	}
	return chunkCoor;
}
pChunk array::insertChunk(const attributeId attrId, pChunk inputChunk)
{
	assert(attrId < this->desc_->attrDescs_->size());
	this->chunks_[attrId].insert(chunkPair(inputChunk->getId(), inputChunk));
	this->setChunkExist(attrId, inputChunk->getId());
	return inputChunk;
}

void array::flush()
{
	// TODO::array::flush()

	for (auto attr : *this->getDesc()->attrDescs_)
	{
		auto cit = this->getChunkIterator(attr->id_);
		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				this->freeChunk(attr->id_, cit->seqPos());
			}
			++(*cit);
		}
		this->chunks_[attr->id_].clear();
	}

	this->chunks_.clear();
}

pChunk array::makeChunk(const chunkDesc& desc)
{
	return this->makeChunk(desc.attrDesc_->id_, desc.id_);
}

void array::makeChunks(const attributeId attrId, const bitmap& input)
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

pChunkDesc array::getChunkDesc(const attributeId attrId, const chunkId cId)
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

pChunk array::getChunk(const attributeId attrId, const chunkId cId)
{
	assert(this->chunks_.find(attrId) != this->chunks_.end());
	return this->chunks_[attrId][cId];
}

arrayId array::getId()
{
	return this->desc_->id_;
}

void array::setId(const arrayId id)
{
	this->desc_->id_ = id;
}

chunkId array::getChunkId(pChunkDesc cDesc)
{
	return this->getChunkIdFromItemCoor(cDesc->sp_);
}

chunkId array::getChunkIdFromItemCoor(const coor& itemCoor)
{
	coor chunkCoor = itemCoor;
	for (dimensionId d = this->desc_->dimDescs_->size() - 1; d != -1; --d)
	{
		chunkCoor[d] /= this->desc_->dimDescs_->at(d)->chunkSize_;
	}
	return this->getChunkIdFromChunkCoor(chunkCoor);
}

chunkId array::getChunkIdFromChunkCoor(const coor& chunkCoor)
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
void array::freeChunk(const attributeId attrId, const chunkId cId)
{
	assert(this->chunks_.find(attrId) != this->chunks_.end());
	if (this->chunks_[attrId][cId] != nullptr)
	{
		this->chunks_[attrId][cId]->flush();
	}
	this->setChunkNull(attrId, cId);
	//this->chunks_[cId] = nullptr;
	//this->overallChunkBitmap_->setNull(cId);
}
cpBitmap array::getChunkBitmap() const
{
	return this->overallChunkBitmap_;
}
void array::copyChunkBitmap(cpBitmap chunkBitmap)
{
	this->overallChunkBitmap_ = std::make_shared<bitmap>(*chunkBitmap);
}
void array::replaceChunkBitmap(pBitmap chunkBitmap)
{
	this->overallChunkBitmap_ = chunkBitmap;
}
void array::mergeChunkBitmap(pBitmap chunkBitmap)
{
	this->overallChunkBitmap_->andMerge(*chunkBitmap);
}
void array::print()
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