#include <iostream>
#include <string>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb\stb_image_write.h>


#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
	//std::string filename = fs::path(argv[0]).parent_path().string() + "\\ExtractedRomFS\\wuhu_ground.trk";

	//std::ifstream file;
	//file.open(filename, std::ios::binary | std::ios::_Nocreate | std::ios::ate);

	//if (!file.is_open())
	//{
	//	std::cout << "Failed to open " << filename << " for reading." << std::endl;
	//}
	//else
	//{
	//	std::streamoff fileSize = file.tellg();

	//	file.seekg(0, std::ios::beg);

	//	// find closest square to filesize
	//	int imageWidthHeight = std::sqrt(fileSize);
	//	int imageSize = imageWidthHeight * imageWidthHeight;

	//	stbi_uc* imageBuffer = new stbi_uc[imageSize];
	//	file.read(reinterpret_cast<char*>(imageBuffer), imageSize);

	//	std::string outputFilename = "C:\\Users\\Andrew\\Desktop\\out.png";

	//	stbi_write_png(outputFilename.c_str(), imageWidthHeight, imageWidthHeight, 1, imageBuffer, imageWidthHeight);

	//	delete[] imageBuffer;

	//	file.close();
	//}



	std::cin.get();

	return 0;
}