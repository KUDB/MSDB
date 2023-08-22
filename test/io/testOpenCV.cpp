#include <pch_test.h>
#include <opencv2/highgui.hpp>
#include <string>

using namespace msdb::core;
using namespace cv;

TEST(io_file, opencv_load)
{
	{
		errno = 0;
		std::string filename = "C:/Workspace/21_MSDB/x64/Debug/Lenna.png";
		FILE* f = fopen(filename.c_str(), "rb");
		std::cout << errno << std::endl;
		if (!f)
		{
			EXPECT_TRUE(f);
		}
	}
	{
		errno = 0;
		std::string filename = "E:/Datasets/cifar10/test/cifar_x_test_0.jpg";
		FILE* f = fopen(filename.c_str(), "rb");
		std::cout << errno << std::endl;
		if (!f)
		{
			EXPECT_TRUE(f);
		}
	}
	{
		errno = 0;
		std::string filename = "E:/Datasets/cifar10/test/cifar_x_test_1.jpg";
		FILE* f = fopen(filename.c_str(), "rb");
		std::cout << errno << std::endl;
		if (!f)
		{
			EXPECT_TRUE(f);
		}
	}

	{
		Mat test_color_img = imread("Lenna.png", IMREAD_COLOR);
		EXPECT_FALSE(test_color_img.empty());
	}
	{
		Mat test_color_img = imread("E:/Datasets/cifar10/test/cifar_x_test_0.png", IMREAD_COLOR);
		EXPECT_FALSE(test_color_img.empty());
	}
}