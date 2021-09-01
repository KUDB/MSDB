#include <pch.h>
#include <util/coordinate.h>
#include <compression/wavelet.h>
#include <compression/haar.h>

namespace msdb
{
namespace core
{
	wavelet::wavelet()
		: h_0(NULL), g_0(NULL), h_1(NULL), g_1(NULL), c_(0), offset_(0), t_(waveletType::None)
	{

	}

	wavelet::wavelet(const char* name, size_t k)
		: wavelet(this->getWaveletType(name), k)
	{

	}

	wavelet::wavelet(waveletType t, size_t k)
	{
		switch (t)
		{
		case waveletType::HaarSimple:
		{
			const double* h_0, * g_0, * h_1, * g_1;
			size_t c, offset;

			haar_simple::getWavelet(&h_0, &g_0, &h_1, &g_1, &c, &offset);
			this->init(h_0, g_0, h_1, g_1, c, offset, t);
			break;
		}
		case waveletType::Haar:
		default:
		{
			const double* h_0, * g_0, * h_1, * g_1;
			size_t c, offset;

			haar::getWavelet(&h_0, &g_0, &h_1, &g_1, &c, &offset);
			this->init(h_0, g_0, h_1, g_1, c, offset, t);
			break;
		}
		}
	}

	//:h_0(h_0), g_0(g_0), h_1(h_1), g_1(g_1), c(c), offset(offset), t(t)
	wavelet::wavelet(const double* h_0, const double* g_0, const double* h_1, const double* g_1,
					 size_t c, size_t offset, waveletType t)
	{
		this->init(h_0, g_0, h_1, g_1, c, offset, t);
	}

	wavelet::~wavelet()
	{

	}

	waveletType wavelet::getWaveletType(const char* name)
	{
		waveletType t = waveletType::None;

		if (strcmp(name, "Haar") == 0)
		{
			t = waveletType::Haar;
		} else if (strcmp(name, "HaarSimple") == 0)
		{
			t = waveletType::HaarSimple;
		} 

		return t;
	}

	void wavelet::init(const double* h_0, const double* g_0,
					   const double* h_1, const double* g_1,
					   size_t c, size_t offset, waveletType t)
	{
		this->h_0 = h_0;
		this->g_0 = g_0;
		this->h_1 = h_1;
		this->g_1 = g_1;

		this->c_ = c;
		this->offset_ = offset;
		this->t_ = t;
	}

	//int waveletEncode(const wavelet* w, const double* data, double* output,
	//					size_t length, std::vector<int>* dims, size_t level)
	//{
	//	// copy input data
	//	double* temp = new double[length];
	//	memcpy(temp, data, sizeof(double) * length);
	//	// init output stream as a zero.
	//	memset(output, 0, sizeof(double) * length);
	//	std::vector<int> boundary(*dims), sP(dims->size(), 0), eP(*dims);

	//	coorRangeIterator<int, double> iit(temp, eP.size(), dims->data(), sP.data(), eP.data());
	//	coorRangeIterator<int, double> oit(output, eP.size(), dims->data(), sP.data(), eP.data());

	//	size_t seq = 0;
	//	size_t levelDataLength = 1;
	//	for (int i = 0; i < dims->size(); i++)
	//	{
	//		levelDataLength *= (static_cast<size_t>(eP[i]) - static_cast<size_t>(sP[i]) + 1);
	//	}

	//	for (int d = dims->size() - 1; d >= 0; d--)
	//	{
	//		std::cout << "[" << d << "] ------------------------------" << std::endl;
	//		size_t half = eP[d] >> 1;
	//		size_t rows = levelDataLength / (static_cast<size_t>(eP[d]) - static_cast<size_t>(sP[d]) + 1);

	//		iit.setBasisDim(d);
	//		iit.moveToStart();
	//		oit.setBasisDim(d);
	//		oit.moveToStart();

	//		for (int r = 0; r < rows; r++)
	//		{
	//			for (int i = sP[d]; i < eP[d]; i += 2)
	//			{
	//				double h = 0, g = 0;

	//				for (int j = 0; (j < w->c_) && (i + j < eP[d]); j++)
	//				{
	//					h += w->h_0[j] * iit[j];
	//					g += w->g_0[j] * iit[j];
	//				}

	//				oit[0] = h;
	//				oit[half] = g;

	//				iit += 2;
	//				++oit;
	//			}
	//			oit += half;	// go to next row
	//		}

	//		for (int i = 0; i < length; i++)
	//		{
	//			std::cout << output[i] << ", ";

	//			if (length % eP[d] == 0)
	//			{
	//				std::cout << "/";
	//			}
	//		}
	//		std::cout << std::endl;

	//		memcpy(temp, output, sizeof(double) * length);
	//	}

	//	delete[] temp;

	//	return 0;
	//}
}		// core
}		// msdb