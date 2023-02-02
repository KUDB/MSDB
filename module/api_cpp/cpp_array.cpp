#include <pch_op.h>
#include <array/arrayMgr.h>
#include <query/query.h>
#include <api_cpp/cpp_array.h>

namespace msdb
{
Array::Array(Context context, core::arrayId arrId)
	: context_(context)
{
	this->arrDesc_ = core::arrayMgr::instance()->getArrayDesc(arrId);
}

Array::Array(Context context, std::string arrName)
	: context_(context)
{
	this->arrDesc_ = core::arrayMgr::instance()->getArrayDesc(
		core::arrayMgr::instance()->getArrayId(arrName));
}

ResultArray::ResultArray(Context context, core::pQuery qry)
	: context_(context), arrDesc_(qry->getArrayDesc()), qry_(qry)
{
}

std::shared_ptr<std::vector<core::coor>> ResultArray::getDimBuffer()
{
	return this->qry_->getDimBuffer();
}

void ResultArray::close()
{
	this->arrDesc_ = nullptr;
	this->qry_ = nullptr;
}
void printResultArray(ResultArray& ra)
{
	auto dimBuffer = ra.getDimBuffer();
	// TODO:: Attribute Id, type
	auto attr_01_buffer = ra.getAttrBuffer<uint8_t>(0);

	for (int i = 0; i < dimBuffer->size(); ++i)
	{
		BOOST_LOG_TRIVIAL(info) << dimBuffer->at(i).toString() << ": " << static_cast<int>(attr_01_buffer->at(i)) << std::endl;
		std::cout << dimBuffer->at(i).toString() << ": " << static_cast<int>(attr_01_buffer->at(i)) << std::endl;
	}
	ra.close();
	BOOST_LOG_TRIVIAL(info) << std::endl;
	std::cout << std::endl;
}

DefDimension::DefDimension(std::string name, uint64_t start, uint64_t end, uint64_t chunkSize, uint64_t blockSize)
	: dimDesc_(std::make_shared<core::dimensionDesc>(
	0, name, start, end, chunkSize, blockSize))
{
}
std::shared_ptr<core::dimensionDesc> DefDimension::getDesc()
{
	return this->dimDesc_;
}

// attribute id will be specified in 'build' operator of cpp_api
DefAttribute::DefAttribute(const std::string name, const core::dataType type,
						   const core::materializedType matType,
						   const std::map<std::string, std::string> optionalParams)
	: attrDesc_(std::make_shared<core::attributeDesc>(0, name, type, matType, core::encodingType::NONE, optionalParams))
{}

DefAttribute::DefAttribute(const std::string name, const core::dataType type,
						   const core::encodingType compType,
						   const std::map<std::string, std::string> optionalParams)
	: attrDesc_(std::make_shared<core::attributeDesc>(0, name, type, core::materializedType::FLATTEN, compType, optionalParams))
{}

DefAttribute::DefAttribute(const std::string name, const core::dataType type,
						   const core::materializedType matType, const core::encodingType compType,
						   const std::map<std::string, std::string> optionalParams)
	: attrDesc_(std::make_shared<core::attributeDesc>(0, name, type, matType, compType, optionalParams))
{}

std::shared_ptr<core::attributeDesc> DefAttribute::getDesc()
{
	return this->attrDesc_;
}
DefAttribute::operator core::attributeDesc()
{
	return *this->attrDesc_;
}
}		// msdb