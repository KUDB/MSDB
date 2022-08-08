#include <pch.h>
#include <op/wavelet_encode/fast_wavelet_decode_plan.h>
#include <op/wavelet_encode/fast_wavelet_decode_action.h>

namespace msdb
{
namespace core
{
fast_wavelet_decode_plan::fast_wavelet_decode_plan()
{
}
const char* fast_wavelet_decode_plan::name()
{
	return "fast_wavelet_decode";
}
pAction fast_wavelet_decode_plan::makeAction()
{
	auto wdAction = std::make_shared<fast_wavelet_decode_action>();
	return wdAction;
}
}		// core
}		// msdb