#include <pch_test.h>
#include <opencv2/highgui.hpp>

using namespace msdb::core;
using namespace cv;

TEST(oepncv, opencv_load)
{
	Mat color_img = imread("Lenna.png", IMREAD_COLOR);
}