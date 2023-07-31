#include <pch.h>
#include <io/io_util.h>
#include <opencv2/core.hpp>         // Mat class, C++ data structure
#include <opencv2/videoio.hpp>      // File I/O
#include <opencv2/highgui.hpp>

namespace msdb
{
namespace core
{
std::stringstream::pos_type size_of_stream(const std::stringstream& ss)
{
    std::streambuf* buf = ss.rdbuf();

    // Get the current position so we can restore it later
    std::stringstream::pos_type original = buf->pubseekoff(0, ss.cur, ss.out);

    // Seek to end and get the position
    std::stringstream::pos_type end = buf->pubseekoff(0, ss.end, ss.out);

    // Restore the position
    buf->pubseekpos(original, ss.out);

    return end;
}
void readImageFile(const std::string filePath, void** data, size_t& bytes, size_t& length)
{
    cv::Mat imgMat;

    /*
     * IMREAD_GRAYSCALE, IMREAD_COLOR, IMREAD_ANYCOLOR
     * IMREAD_ANYDEPTH - 16/32 bits, default: 8bit
     * IMREAD_UNCHANGED
     */
    imgMat = cv::imread(filePath, cv::IMREAD_UNCHANGED);
    int numChannels = imgMat.channels();
    length = imgMat.rows * imgMat.cols;
    bytes = length * numChannels;

    *data = (void*)new uchar[bytes];
    memcpy(*data, imgMat.data, bytes);

    // TODO::Change void** to shared_ptr<>
    //data = std::static_pointer_cast<void>(std::make_shared<uchar[]>(new uchar[bytes]));
    //memcpy(&data, imgMat.data, bytes);

    // Here is an example of the data structure of cv::Mat
    //uchar* imgData = imgMat.data;
    //for (int row = 0; row < imgMat.rows; ++row)
    //{
    //    for (int col = 0; col < imgMat.cols; ++col)
    //    {
    //        uchar b = imgData[row * imgMat.cols * 3 + col * 3];
    //        uchar g = imgData[row * imgMat.cols * 3 + col * 3 + 1];
    //        uchar r = imgData[row * imgMat.cols * 3 + col * 3 + 2];
    //    }
    //}
}
}		// core
}		// msdb