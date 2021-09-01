#pragma once
#ifndef _MSDB_WAVELET_H_
#define _MSDB_WAVELET_H_

#include <pch.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class wavelet;
using pWavelet = std::shared_ptr<wavelet>;

// Wavelet Family
enum class waveletType
{
	None,
	Haar,
	HaarSimple
};

class wavelet
{
public:
	const double* h_0;
	const double* g_0;
	const double* g_1;
	const double* h_1;

	waveletType t_;			// Wavelet Family
	size_t c_;				// Wavelet length
	size_t offset_;

public:
	wavelet();
	wavelet(const char* name, size_t k = 0);
	wavelet(waveletType t, size_t k = 0);
	wavelet(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
			size_t c, size_t offset, waveletType t = waveletType::None);
	~wavelet();

private:
	static waveletType getWaveletType(const char* name);
	void init(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
			  size_t c, size_t offset, waveletType t);
};

//int waveletEncode(const wavelet* w, const double* data, double* output,
//					size_t length, std::vector<int>* dims, size_t level = 1);

template <typename _Dty>
void getWaveletLevelDims(std::vector<_Dty>& output, const std::vector<_Dty>& dims,
						 const size_t level)
{
	assert(level + 1 != 0);

	double factor = pow(1 / 2, level);
	for (auto d : dims)
	{
		output.push_back(ceil(d * factor));
	}
}

template <typename _Dty>
size_t getDataLength(const size_t dSize, const _Dty* sP, const _Dty* eP)
{
	size_t length = 1;
	for (size_t d = 0; d < dSize; d++)
	{
		length *= (eP[d] - sP[d]);
	}
	return length;
}
}		// core
}		// msdb
#endif