#include <pch_test.h>
#include <opencv2/highgui.hpp>

using namespace msdb::core;
using namespace cv;

TEST(io_file, opencv_load)
{
	Mat color_img = imread("Lenna.png", IMREAD_COLOR);
	Mat test_color_img = imread("Lennaa.png", IMREAD_COLOR);
	if (test_color_img.empty())
	{
		std::cout << "Read Fail" << std::endl;
	}
	else
	{
		std::cout << test_color_img.channels() << std::endl;
	}
}