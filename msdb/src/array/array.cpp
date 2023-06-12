#include <pch.h>
#include <array/array.h>
#include <util/math.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
array::array(pArrayDesc desc)
	: globalChunkBitmap_()
{
	this->desc_ = desc;
	this->globalChunkBitmap_ = std::make_shared<bitmap>(desc_->dimDescs_->getChunkSpace().area(), false);
	for (auto attr : *desc_->attrDescs_)
	{
		this->attrChunkBitmaps_[attr->id_] = std::make_shared<bitmap>(desc_->dimDescs_->getChunkSpace().area(), false);
		this->chunks_[attr->id_] = chunkContainer();
	}
	//initChunkFactories();
}
array::~array()
{
	//BOOST_LOG_TRIVIAL(debug) << "~array(): " << this->desc_->name_;
	this->flush();
	this->chunks_.clear();
	this->globalChunkBitmap_ = nullptr;
	for (auto b : this->attrChunkBitmaps_)
	{
		b.second = nullptr;
	}
	this->desc_ = nullptr;
	this->cFactories_.clear();
}
pArrayDesc array::getDesc()
{
	return this->desc_;
}

const pArrayDesc array::getDesc() const
{
	return this->desc_;
}

pChunkIterator array::getChunkIterator(const attributeId attrId, const iterateMode itMode)
{
	if (this->chunks_.find(attrId) != this->chunks_.end())
	{
		return std::make_shared<chunkIterator>(this->desc_->dimDescs_->getChunkSpace(),
											   &(this->chunks_.at(attrId)), this->attrChunkBitmaps_.at(attrId),
											   itMode);
	}
	//_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NO_ATTRIBUTE, "Fail to get chunkIterator (attrId: " + std::to_string(attrId) + ")"));


	//return std::make_shared<chunkIterator>(this->desc_->dimDescs_->getChunkSpace(),
	//									   nullptr, this->globalChunkBitmap_,
	//									   itMode);
	
	return std::make_shared<chunkIterator>(this->desc_->dimDescs_->getChunkSpace(),
										   nullptr, std::make_shared<bitmap>(this->globalChunkBitmap_->getCapacity(), false),
										   itMode);
}

const pConstChunkIterator array::getChunkIterator(const attributeId attrId, const iterateMode itMode) const
{
	if (this->chunks_.find(attrId) != this->chunks_.end())
	{
		return std::make_shared<constChunkIterator>(this->desc_->dimDescs_->getChunkSpace(),
			&(this->chunks_.at(attrId)), this->attrChunkBitmaps_.at(attrId),
			itMode);
	}

	return std::make_shared<constChunkIterator>(this->desc_->dimDescs_->getChunkSpace(),
		nullptr, std::make_shared<bitmap>(this->globalChunkBitmap_->getCapacity(), false),
		itMode);
}
pChunkFactory array::getChunkFactory(const attributeId& attrId)
{
	assert(attrId < this->cFactories_.size());

	return this->cFactories_[attrId];
}
//array::size_type array::getNumChunks(const attributeId attrId)
//{
//	return this->chunks_[attrId].size();
//}
coor array::itemCoorToChunkCoor(const coor& itemCoor) const
{
	coor chunkCoor(this->desc_->dimDescs_->size());
	for (dimensionId d = 0; d < this->desc_->dimDescs_->size(); d++)
	{
		chunkCoor[d] = floor(itemCoor[d] / (*this->desc_->dimDescs_)[d]->chunkSize_);
	}
	return chunkCoor;
}
pChunk array::insertChunk(pChunk inputChunk)
{
	const auto attrId = inputChunk->getDesc()->attrDesc_->id_;
	assert(attrId < this->desc_->attrDescs_->size());
	
	this->chunks_[attrId].insert(chunkPair(inputChunk->getId(), inputChunk));
	this->setChunkExist(attrId, inputChunk->getId());
	return inputChunk;
}

void array::flush()
{
	try
	{
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
	}
	catch (msdb_exception e)
	{
		BOOST_LOG_TRIVIAL(error) << "Fail array::flush()";
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
	catch (std::exception e)
	{
		BOOST_LOG_TRIVIAL(error) << "Fail array::flush()";
		BOOST_LOG_TRIVIAL(error) << e.what();
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(error) << "Fail array::flush()";
	}
	
	this->chunks_.clear();
}

pChunk array::makeChunk(const attributeId attrId, const chunkId cId)
{
	return this->makeChunk(this->getChunkDesc(attrId, cId));
	// TODO:: this->insertChunk(chunkObj);
}

pChunk array::makeChunk(pChunkDesc desc)
{
	auto chunkObj = this->getChunkFactory(desc->attrDesc_->id_)->requestNewChunk(desc);
	this->insertChunk(chunkObj);

	return chunkObj;
}

void array::makeChunks(const attributeId attrId, const bitmap& input)
{
	chunkId capacity = this->getChunkIterator(attrId)->getCapacity();
	for(chunkId cid = 0; cid < capacity; ++cid)
	{
		if(input.isExist(cid) && !this->attrChunkBitmaps_[attrId]->isExist(cid))
		{
			auto chunkObj = this->makeChunk(attrId, cid);
			this->insertChunk(chunkObj);
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

arrayId array::getId() const
{
	return this->desc_->id_;
}

void array::setId(const arrayId id)
{
	this->desc_->id_ = id;
}

chunkId array::getChunkId(pChunkDesc cDesc) const
{
	return this->itemCoorToChunkId(cDesc->sp_);
}

chunkId array::itemCoorToChunkId(const coor& itemCoor) const
{
	coor chunkCoor = itemCoor;
	for (dimensionId d = this->desc_->dimDescs_->size() - 1; d != -1; --d)
	{
		chunkCoor[d] /= this->desc_->dimDescs_->at(d)->chunkSize_;
	}
	return this->chunkCoorToChunkId(chunkCoor);
}

chunkId array::chunkCoorToChunkId(const coor& chunkCoor) const
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
	_MSDB_TRY_BEGIN
	{
		if (this->chunks_[attrId][cId] != nullptr)
		{
			this->chunks_[attrId][cId]->flush();
			this->chunks_[attrId][cId] = nullptr;
		}
		this->setChunkNull(attrId, cId);
	}_MSDB_CATCH_ALL
	{
		BOOST_LOG_TRIVIAL(error) << "freeChunk(" << attrId << ", " << cId << ") error";
	}

	//this->chunks_[cId] = nullptr;dkddkd
	//this->globalChunkBitmap_->setNull(cId);
}
cpBitmap array::getChunkBitmap() const
{
	return this->globalChunkBitmap_;
}
void array::copyChunkBitmap(cpBitmap chunkBitmap)
{
	this->globalChunkBitmap_ = std::make_shared<bitmap>(*chunkBitmap);

	auto attrDesc = this->getDesc()->getAttrDescs();
	for (auto attr : *attrDesc)
	{
		this->copyAttrChunkBitmap(attr->getId(), chunkBitmap, true);
	}
}
void array::replaceChunkBitmap(pBitmap chunkBitmap)
{
	this->globalChunkBitmap_ = chunkBitmap;
	// Copy for attr chunkBitmap
	auto attrDesc = this->getDesc()->getAttrDescs();
	for (auto attr : *attrDesc)
	{
		this->copyAttrChunkBitmap(attr->getId(), chunkBitmap, true);
	}
}
void array::mergeChunkBitmap(cpBitmap chunkBitmap)
{
	this->globalChunkBitmap_->andMerge(*chunkBitmap);

	auto attrDesc = this->getDesc()->getAttrDescs();
	for (auto attr : *attrDesc)
	{
		this->mergeAttrChunkBitmap(attr->getId(), chunkBitmap, true);
	}
}

void array::copyAttrChunkBitmap(const attributeId attrId, cpBitmap chunkBitmap, bool globalUpdated)
{
	this->attrChunkBitmaps_[attrId] = nullptr;
	this->attrChunkBitmaps_[attrId] = std::make_shared<bitmap>(*chunkBitmap);
	if (!globalUpdated)
	{
		this->mergeChunkBitmap(chunkBitmap);
	}
}
void array::replaceAttrChunkBitmap(const attributeId attrId, pBitmap chunkBitmap, bool globalUpdated)
{
	this->attrChunkBitmaps_[attrId] = nullptr;
	this->attrChunkBitmaps_[attrId] = chunkBitmap;
	if (!globalUpdated)
	{
		this->mergeChunkBitmap(chunkBitmap);
	}
}
void array::mergeAttrChunkBitmap(const attributeId attrId, cpBitmap chunkBitmap, bool globalUpdated)
{
	this->attrChunkBitmaps_[attrId]->andMerge(*chunkBitmap);
	if (!globalUpdated)
	{
		this->mergeChunkBitmap(chunkBitmap);
	}
}

void array::print() const
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

void array::shallowChunkCopy(array& inArr, const bool takeOwnership)
{
	this->copyChunkBitmap(inArr.getChunkBitmap());

	auto attrDescs = inArr.getDesc()->getAttrDescs();
	for (auto attrDesc : *attrDescs)
	{
		auto chunkItr = inArr.getChunkIterator(attrDesc->getId(), iterateMode::EXIST);

		while (!chunkItr->isEnd())
		{
			assert(chunkItr->isExist() == true);

			auto inChunk = (**chunkItr);
			auto outChunk = this->makeChunk(std::make_shared<chunkDesc>(*inChunk->getDesc()));
			outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
			outChunk->makeBlocks();
			outChunk->bufferRef(inChunk, takeOwnership);

			++(*chunkItr);
		}
	}
}

void array::deleteAttribute(const attributeId attrId)
{
	auto attrDescs = this->getDesc()->getAttrDescs();
	auto numAttributes = attrDescs->size();
	assert(attrId < numAttributes);


	for (attributeId id = attrId + 1; id < numAttributes; ++id)
	{
		this->chunks_[id - 1] = this->chunks_[id];
		this->attrChunkBitmaps_[id - 1] = this->attrChunkBitmaps_[id];
		this->cFactories_[id - 1] = this->cFactories_[id];

		attrDescs->at(id - 1) = attrDescs->at(id);
		attrDescs->at(id - 1)->setId(id - 1);
	}

	this->chunks_.erase(numAttributes - 1);
	this->attrChunkBitmaps_.erase(numAttributes - 1);
	this->cFactories_.pop_back();
	attrDescs->pop_back();
}

// TODO::Array Comparison
bool operator==(const array& lhs, const array& rhs)
{
	auto lArrayDesc = lhs.getDesc();
	auto rArrayDesc = rhs.getDesc();

	if (*lArrayDesc != *rArrayDesc)	return false;

	auto lAttrDesc = lArrayDesc->getAttrDescs();
	auto rAttrDesc = rArrayDesc->getAttrDescs();
	auto attrSize = lAttrDesc->size();

	for (attributeId attrId = 0; attrId <= attrSize; ++attrId)
	{
		auto lcit = lhs.getChunkIterator(attrId, iterateMode::ALL);
		auto rcit = rhs.getChunkIterator(attrId, iterateMode::ALL);

		if (lcit->getCapacity() != rcit->getCapacity())	return false;

		while (true)
		{
			if (lcit->isEnd() != rcit->isEnd())	return false;
			if (lcit->isEnd())	break;

			if (***lcit != ***rcit)	return false;

			++(*lcit);
			++(*rcit);
		}
	}
}
}		// core
}		// msdb