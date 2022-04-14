#pragma once
#ifndef _MSDB_OP_ZFP_ENCODE_ACTION_H_
#define _MSDB_OP_ZFP_ENCODE_ACTION_H_

#include <pch.h>
#include "zfp.h"

namespace msdb
{
namespace core
{
int compress(double* array, int nx, int ny, int nz, double tolerance, int decompress);
}		// core
}		// msdb
#endif	// _MSDB_OP_ZFP_ENCODE_ACTION_H_