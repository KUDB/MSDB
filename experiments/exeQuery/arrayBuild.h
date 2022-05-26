#pragma once
#ifndef _MSDB_EXPERIMENTS_ARRAYBUILD_H_
#define _MSDB_EXPERIMENTS_ARRAYBUILD_H_

#include <pch_experiments.h>
#include <index/attributeIndex.h>

namespace msdb
{
namespace dummy
{
namespace data_star1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_star1024x1024
namespace data_saturn1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_saturn1024x1024
namespace data_solar1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_solar1024x1024
namespace data_mars4096x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_mars4096x2048
namespace data_mercury20480x10240
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_mercury20480x10240
namespace data_lunar102400x40960
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_lunar102400x40960
namespace data_star64x64x64
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}		// data_star64x64x64
}		// dummy
}		// msdb
#endif	// _MSDB_EXPERIMENTS_ARRAYBUILD_H_