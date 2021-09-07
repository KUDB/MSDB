#include <pch.h>
#include <array/dimensionDesc.h>
#include <util/math.h>
#include <xml/xmlFile.h>

namespace msdb
{
namespace core
{
dimensionDesc::dimensionDesc(dimensionId id, std::string name,
							 dimension_type start, dimension_type end, 
							 position_t chunkSize, position_t blockSize)
	: id_(id), name_(name), start_(start), end_(end), 
	chunkSize_(chunkSize), blockSize_(blockSize)
{
}

dimensionDesc::dimensionDesc(const dimensionDesc& mit)
	: id_(mit.id_), name_(mit.name_), start_(mit.start_), end_(mit.end_),
	chunkSize_(mit.chunkSize_), blockSize_(mit.blockSize_)
{
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
dimensionDescs::dimensionDescs()
{
}
dimensionDescs::dimensionDescs(const dimensionDescs& mit)
{
	for(auto desc : mit)
	{
		this->push_back(std::make_shared<dimensionDesc>(*desc));
	}
}
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
}		// core
}		// msdb