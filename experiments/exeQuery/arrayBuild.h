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
}
namespace data_saturn1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}
namespace data_solar1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}
namespace data_mars4096x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}
namespace data_mercury20480x10240
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}
namespace data_lunar102400x40960
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType);
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType);
}
}
}
#endif	// _MSDB_EXPERIMENTS_ARRAYBUILD_H_