#pragma once
#ifndef _MSDB_DUMMY_EXE_BUILD_ASTRONOMY_ARRAY_3D_H_
#define _MSDB_DUMMY_EXE_BUILD_ASTRONOMY_ARRAY_3D_H_

#include <api_cpp/msdb.h>
#include <dummy.h>
#include <dummy_astronomy_array_3d.h>
#include <index/attributeIndex.h>

namespace msdb
{
namespace dummy
{
namespace data_star64x64x64
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_star64x64x64
namespace data_nexrad_16x1024x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_nexrad_16x1024x2048
namespace data_solar_sdo_1024x1024x512
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_solar_sdo_1024x1024x512
namespace data_nexrad_201711_64x24x1024x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType);
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_nexrad_201711_64x24x1024x2048
}		// dummy
}		// msdb
#endif	// _MSDB_DUMMY_EXE_BUILD_ASTRONOMY_ARRAY_3D_H_