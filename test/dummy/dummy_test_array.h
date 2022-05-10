#pragma once
#ifndef _MSDB_TEST_DUMMY_H_
#define _MSDB_TEST_DUMMY_H_
#include <pch_test.h>
#include <array/arrayDesc.h>
#include <array/blockDesc.h>
#include <array/chunkDesc.h>
#include <dummy_array_util.h>

namespace msdb
{
namespace test
{
//core::pDimensionDescs dimensionDescBuilder(
//	std::vector<std::string> dimNames, core::dimension& dims, 
//	core::dimension& chunkDims, core::dimension& blockDims
//);
//core::pAttributeDescs attributeDescBuilder(
//	std::vector<std::string> attrNames,
//	std::vector<core::dataType> attrTypes
//);


core::pArrayDesc getDummyArrayDesc_SIMPLE_2D();

template<typename Ty_>
core::pBlockDesc getDummyBlockDesc_SIMPLE_2D()
{
	core::blockId bid = 0;
	core::dimension dims(32, 32);
	core::coordinates sp({ 0, 0 }), ep = dims;
	core::blockSize size = sizeof(Ty_) * dims.size();
	core::blockSize offset = 0;

	core::eleType eType = getEleType<Ty_>();

	return std::make_shared<core::blockDesc>(bid, eType, dims, sp, ep, size, offset);
}

template <typename Ty_>
core::pChunkDesc getDummyChunkDesc_SIMPLE_2D(core::dimension& dims)
{
	core::chunkId cid = 0;
	core::pAttributeDescs attrDescs = dummy::attributeDescBuilder({"ATTR_1"}, {core::concreteTy<char>()});
	core::coordinates sp({0, 0});
	core::chunkSize size = sizeof(Ty_) * dims.area();

	return std::make_shared<core::chunkDesc>(cid, attrDescs->at(0), dims, dims, sp, dims, size);
}
}		// test
}		// msdb
#endif	// _MSDB_TEST_DUMMY_H_