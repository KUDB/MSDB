#pragma once
#ifndef _MSDB_FILE_H_
#define _MSDB_FILE_H_

#include <pch.h>
#include <util/singleton.h>

namespace msdb
{
namespace core
{
class file;
class fileManager;

class file
{
public:
	using byteSize = size_t;

public:
	file(char const* strPath, int flags);

	/**

	*/
	~file();

	int close();

	bool isClosed();

	byteSize read(void* data, byteSize size, byteSize offs);
	byteSize wirte(const void* data, byteSize size, byteSize offs);

private:
	std::fstream fs_;
	bool closed_;
	fileManager* fm_;		// pointer to filemanager
};


// TODO::Remove singleton, make all member functions to static function
class fileManager : public singleton<fileManager>
{
public:
	using files = std::vector<file>;
	using fileEntries = std::vector<std::filesystem::directory_entry>;

	bool openDir(const char* strPath);
	bool removeDir(const char* strPath);

	bool removeFile(char const* strPath);

	fileEntries getFileEntries(const char* strPath);

protected:
	bool init();

private:
	
};

/**
 * Find extension in string file path
 *
 * - WARNING: Extension longer than 9 words will be treated wrong extension.
 * 
 * Returns:
 * - Empty string if there is no exteion.
 * - Extension string without dot (type:std::string)
 **/
std::string getFileExtension(const std::string filePath);


/**
 * The following file formats are supported by OpenCV 'imread()'
 * 
 * Windows bitmaps - *.bmp, *.dib (always supported)
 * JPEG files - *.jpeg, *.jpg, *.jpe (see the Note section)
 * JPEG 2000 files - *.jp2 (see the Note section)
 * Portable Network Graphics - *.png (see the Note section)
 * WebP - *.webp (see the Note section)
 * Portable image format - *.pbm, *.pgm, *.ppm *.pxm, *.pnm (always supported)
 * PFM files - *.pfm (see the Note section)
 * Sun rasters - *.sr, *.ras (always supported)
 * TIFF files - *.tiff, *.tif (see the Note section)
 * OpenEXR Image files - *.exr (see the Note section)
 * Radiance HDR - *.hdr, *.pic (always supported)
 * Raster and Vector geospatial data supported by GDAL (see the Note section)
 * 
 * Note:
 * Use the IMREAD_UNCHANGED flag to keep the floating point values from PFM image.
 * By default number of pixels must be less than 2^30. Limit can be set using system variable OPENCV_IO_MAX_IMAGE_PIXELS
 * 
 * https://docs.opencv.org/4.7.0/d4/da8/group__imgcodecs.html
 */
const std::vector<std::string> imageFileExtension = {"bmp", "jpg", "jpeg", "png", "webp", "tiff", "tif"};

/**
 * The function determines the file by its file name (extension).
 * 
 * - WARNING: Wrong extension or filename will lead to wrong result.
 * - Only image file extensions written in 'imageFileExtension' list can be recognized.
 */
bool isImageFile(const std::string strFilePath);
}		// core
}		// msdb
#endif	// _MSDB_FILE_H_
