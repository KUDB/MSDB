#include "gtest/gtest.h"
#include <io/file.h>

using namespace msdb::core;


TEST(io_file, getFileExtension)
{
	EXPECT_EQ(getFileExtension("C:/Users/Pictures/msdb_build_complete.png"), std::string("png"));
	EXPECT_EQ(getFileExtension("msdb_build_complete.xlsx"), std::string("xlsx"));

	// Uppercase -> Lowercase
	EXPECT_EQ(getFileExtension("C:/Users/Pictures/msdb_build_complete.PNG"), std::string("png"));
	
	// Find last dot
	EXPECT_EQ(getFileExtension("C:.Users.Pictures.msdb_build_complete.png"), std::string("png"));
	
	// Extension longer than 9 word is ignored
	EXPECT_EQ(getFileExtension("msdb_build_com.pletexlsx"), std::string("pletexlsx"));
	EXPECT_EQ(getFileExtension("msdb_build_co.mpletexlsx"), std::string(""));
	
	// No extension
	EXPECT_EQ(getFileExtension("msdb_build_completexlsx"), std::string(""));
	EXPECT_EQ(getFileExtension("C:/Users/Pictures/msdb_build_complete"), std::string(""));
}

TEST(io_file, imageFile)
{
	EXPECT_TRUE(isImageFile("msdb_build_complete.jpg"));
	EXPECT_TRUE(isImageFile("Pictures/msdb_build_complete.jpg"));
	EXPECT_TRUE(isImageFile("C:/Users/Pictures/msdb_build_complete.PNG"));
	EXPECT_TRUE(isImageFile("C:/Users/Pictures/msdb_build_complete.png"));

	EXPECT_FALSE(isImageFile("msdb_build_complete.xlsx"));
	EXPECT_FALSE(isImageFile("Pictures/msdb_build_complete.exe"));
	EXPECT_FALSE(isImageFile("C:/Users/Pictures/msdb_build_complete.pdf"));
	EXPECT_FALSE(isImageFile("C:/Users/Pictures/msdb_build_complete.pptx"));
}
