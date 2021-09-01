#include <pch.h>
#include <compression/haar.h>

namespace msdb
{
namespace core
{
	namespace haar
	{
		static const double h2[2] =
		{
			1 / sqrt(2), 1 / sqrt(2)
		};
		static const double g2[2] =
		{
			1 / sqrt(2), -1 / sqrt(2)
		};

		void getWavelet(const double** h_0, const double** g_0,
						const double** h_1, const double** g_1,
						size_t* c, size_t* offset)
		{
			*h_0 = haar::h2;
			*h_1 = haar::h2;
			*g_0 = haar::g2;
			*g_1 = haar::g2;

			*c = 2;
			*offset = 1;
		}
	}

	namespace haar_simple
	{
		static const double h2[2] =
		{
			0.5, 0.5
		};
		static const double g2[2] =
		{
			0.5, -0.5
		};
		static const double ih2[2] =
		{
			1, 1
		};
		static const double ig2[2] =
		{
			1, -1
		};

		void getWavelet(const double** h_0, const double** g_0,
						const double** h_1, const double** g_1,
						size_t* c, size_t* offset)
		{
			*h_0 = haar_simple::h2;
			*h_1 = haar_simple::ih2;
			*g_0 = haar_simple::g2;
			*g_1 = haar_simple::ig2;

			*c = 2;
			*offset = 1;
		}
	}
}		// core
}		// msdb