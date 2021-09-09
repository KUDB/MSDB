#include <iostream>
#include <filesystem>
#include <fstream>
#include <assert.h>

int main()
{
	std::ifstream in("../dummy/star1024x1024.txt", std::ios::binary);
	assert(in.is_open() == true && "Check file is opened");

	// Get file size
	in.seekg(0, std::ios::end);
	size_t fileLength = (size_t)in.tellg();
	in.seekg(0, std::ios::beg);

	std::cout << std::endl;

	return 0;
}