#include <pch.h>
#include <array/dimensionDesc.h>
#include <util/math.h>
#include <xml/xmlFile.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// dimensionDescs
//
// ***************************
// Constructor
dimensionDescs::dimensionDescs()
{
}
dimensionDescs::dimensionDescs(const dimensionDescs& mit)
{
	for (auto desc : mit)
	{
		this->push_back(std::make_shared<dimensionDesc>(*desc));
	}
}
// ***************************
dimension dimensionDescs::getDims()
{
	dimension dims(this->size());
	for (dimensionId d = 0; d < this->size(); ++d)
	{
		dims[d] = this->at(d)->getLength();
	}
	return dims;
}
dimension dimensionDescs::getChunkDims()
{
	dimension dims(this->size());
	for (dimensionId d = 0; d < this->size(); ++d)
	{
		dims[d] = this->at(d)->chunkSize_;
	}
	return dims;
}

dimension dimensionDescs::getChunkSpace()
{
	return this->getDims() / this->getChunkDims();
}

dimension dimensionDescs::getBlockDims()
{
	dimension blockDims(this->size());
	for (dimensionId d = 0; d < this->size(); ++d)
	{
		blockDims[d] = this->at(d)->blockSize_;
	}
	return blockDims;
}

dimension dimensionDescs::getBlockSpace()
{
	return dimension(this->getChunkDims()) / this->getBlockDims();
}

// ***************************
// Friends
bool operator== (const dimensionDescs& lhs_, const dimensionDescs& rhs_)
{
	if (lhs_.size() != rhs_.size())		return false;

	auto size = lhs_.size();
	for (int i = 0; i < size; ++i)
	{
		_MSDB_TRY_BEGIN
		{
			// shared_ptr obj
			if (lhs_.at(i) != rhs_.at(i))
			{
				if (lhs_.at(i) == nullptr || rhs_.at(i) == nullptr)	return false;
				if (*(lhs_.at(i)) != *(rhs_.at(i)))					return false;
			}
		}
		_MSDB_CATCH(std::out_of_range e)
		{
			return false;
		}
		_MSDB_CATCH_END
	}

	return true;
}
bool operator!= (const dimensionDescs& lhs_, const dimensionDescs& rhs_)
{
	if (lhs_ == rhs_)
		return false;

	return true;
}


//////////////////////////////
// dimensionDesc
//
dimensionDesc::dimensionDesc()
	: id_(0), name_(""), start_(0), end_(0), chunkSize_(0), blockSize_(0)
{
}
dimensionDesc::dimensionDesc(const dimensionId id, const std::string name,
							 const dimension_type start, const dimension_type end,
							 const position_t chunkSize, const position_t blockSize)
	: id_(id), name_(name), start_(start), end_(end), 
	chunkSize_(chunkSize), blockSize_(blockSize)
{
}

dimensionDesc::dimensionDesc(const dimensionDesc& mit)
	: id_(mit.id_), name_(mit.name_), start_(mit.start_), end_(mit.end_),
	chunkSize_(mit.chunkSize_), blockSize_(mit.blockSize_)
{
}

dimensionDesc::dimensionDesc(dimensionDesc&& src) noexcept
	: dimensionDesc()
{
	swap(*this, src);
}

position_t msdb::core::dimensionDesc::getLength()
{
	return this->end_ - this->start_;
}
size_t dimensionDesc::getChunkNum()
{
	if(this->getLength() % this->chunkSize_)
	{
		return this->getLength() / this->chunkSize_ + 1;
	}
	return this->getLength() / this->chunkSize_;
}

std::string dimensionDesc::toString()
{
	std::stringstream ss;
	ss << "id: " << this->id_ << ", name: " << this->name_ << ", (" << this->start_ << "~" << this->end_ << "), cs: " << this->chunkSize_ << ", bs: " << this->blockSize_;
	return ss.str();
}

/**
 * Save/load in XML file
 */
tinyxml2::XMLElement* dimensionDesc::convertToXMLDoc(tinyxml2::XMLElement* node)
{
	node->SetAttribute(_MSDB_STR_DIM_ID_, this->id_);
	node->SetAttribute(_MSDB_STR_DIM_NAME_, this->name_.c_str());
	node->SetAttribute(_MSDB_STR_DIM_START_, this->start_);
	node->SetAttribute(_MSDB_STR_DIM_END_, this->end_);
	node->SetAttribute(_MSDB_STR_DIM_CHUNK_SIZE_, this->chunkSize_);
	node->SetAttribute(_MSDB_STR_DIM_BLOCK_SIZE_, this->blockSize_);

	return node;
}
pDimensionDesc dimensionDesc::buildDescFromXML(tinyxml2::XMLElement* node)
{
	auto id = node->IntAttribute(_MSDB_STR_DIM_ID_);
	auto name = xmlErrorHandler(node->Attribute(_MSDB_STR_DIM_NAME_));
	auto start = node->Int64Attribute(_MSDB_STR_DIM_START_);
	auto end = node->Int64Attribute(_MSDB_STR_DIM_END_);
	auto chunkSize = node->Int64Attribute(_MSDB_STR_DIM_CHUNK_SIZE_);
	auto blockSize = node->Int64Attribute(_MSDB_STR_DIM_BLOCK_SIZE_);

	return std::make_shared<dimensionDesc>(id, name, start, end, chunkSize, blockSize);
}


//////////////////////////////
// Operators
// ***************************
// Assign
dimensionDesc& dimensionDesc::operator=(const dimensionDesc& src)
{
	if (this == &src)
	{
		return *this;
	}

	dimensionDesc temp(src);
	swap(*this, temp);
	return *this;
}

dimensionDesc& dimensionDesc::operator=(dimensionDesc&& src) noexcept
{
	dimensionDesc temp(std::move(src));
	swap(*this, temp);
	return *this;
}

// ***************************
// Friends
void swap(dimensionDesc& lhs, dimensionDesc& rhs)
{
	using std::swap;

	swap(lhs.id_, rhs.id_);
	swap(lhs.name_, rhs.name_);

	swap(lhs.start_, rhs.start_);
	swap(lhs.end_, rhs.end_);

	swap(lhs.chunkSize_, rhs.chunkSize_);
	swap(lhs.blockSize_, rhs.blockSize_);
}
bool operator== (const dimensionDesc& lhs_, const dimensionDesc& rhs_)
{
	if (lhs_.id_ != rhs_.id_) return false;
	if (lhs_.name_ != rhs_.name_) return false;

	if (lhs_.start_ != rhs_.start_) return false;
	if (lhs_.end_ != rhs_.end_) return false;

	if (lhs_.chunkSize_ != rhs_.chunkSize_) return false;
	if (lhs_.blockSize_ != rhs_.blockSize_) return false;

	return true;
}
bool operator!= (const dimensionDesc& lhs_, const dimensionDesc& rhs_)
{
	if (lhs_ == rhs_)
		return false;

	return true;
}
}		// core
}		// msdb