#include <iostream>
#include <fstream>
#include <string>
#include <experimental\filesystem>

namespace fs = std::experimental::filesystem;

// backs up the file
void backup(const std::string& filename)
{
	// make a backup ("Unity.exe.bak")
	fs::copy_file(fs::path(filename), fs::path(filename + ".bak"), fs::copy_options::overwrite_existing);
}

// reads the file returns the unmodified buffer
uint8_t* getFileBuffer(const std::string& filename, int& filesize)
{
	// open the file
	std::ifstream file;
	file.open(filename, std::ios::binary | std::ios::_Nocreate | std::ios::ate);

	// ensure file opened properly
	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << " for reading. Make sure unity isn't open.\n";

		return new uint8_t;
	}
	else
	{
		// store file size
		filesize = (int)file.tellg();

		// return to start of file
		file.seekg(0, std::ios::beg);

		// read entire file into a big buffer
		uint8_t* unityBuffer = new uint8_t[filesize];
		file.read(reinterpret_cast<char*>(unityBuffer), filesize);

		// close the file
		file.close();

		return unityBuffer;
	}
}

// modifies the buffer to enable dark mode
void modifyBuffer(uint8_t* buffer, int& filesize)
{
	// find and replace the magic numbers in the buffer
	uint8_t expectedBuffer1[15] = { 0x84, 0xC0, 0x75, 0x08, 0x33, 0xC0, 0x48, 0x83, 0xC4, 0x20, 0x5B, 0xC3, 0x8B, 0x03, 0x48 };
	uint8_t expectedBuffer2[15] = { 0x84, 0xC0, 0x75, 0x08, 0x33, 0xC0, 0x48, 0x83, 0xC4, 0x30, 0x5B, 0xC3, 0x8B, 0x03, 0x48 };

	// don't run off the end
	for (int currentOffset = 0; currentOffset < filesize - 15; currentOffset++)
	{
		bool foundMagic1 = true;
		bool foundMagic2 = true;

		for (size_t i = 0; i < 15; i++)
		{
			if (buffer[currentOffset + i] != expectedBuffer1[i])
			{
				foundMagic1 = false;
			}
			if (buffer[currentOffset + i] != expectedBuffer2[i])
			{
				foundMagic2 = false;
			}

			if (!(foundMagic1 || foundMagic2))
			{
				break;
			}
		}

		// change the third byte from 0x75 to 0x74
		if (foundMagic1 || foundMagic2)
		{
			buffer[currentOffset + 2] = 0x74;
			break;
		}
	}
}

// writes the buffer to a file and then deletes it
void writeBuffer(uint8_t* buffer, int& filesize, const std::string& filename)
{
	// write the entire buffer to the new file
	std::ofstream newUnity;
	newUnity.open(filename, std::ios::binary | std::ios::trunc);

	// ensure it opened
	if (!newUnity.is_open())
	{
		std::cout << "Failed to open " << filename << " for writing. Make sure unity isn't open.\n";

		delete[] buffer;
	}
	else
	{
		// write the buffer
		newUnity.write(reinterpret_cast<char*>(buffer), filesize);

		// free the buffer and close the file
		delete[] buffer;

		newUnity.close();
	}
}

int main(int argc, char* argv[])
{
	uint8_t* buffer = nullptr;
	std::string filename;
	int filesize = 0;

	// attempt to determine filename

	if (argc == 1)
	{
		// attempt to find "Unity.exe" in the same directory
		if (fs::exists(fs::path(argv[0]).parent_path().string() + "/Unity.exe"))
		{
			filename = fs::path(argv[0]).parent_path().string() + "/Unity.exe";
		}
		else
		{
			std::cout << "Unable to find \"Unity.exe\" in the current directory." << std::endl;
			std::cin.get();
			return 1;
		}
	}
	else if (argc > 1)
	{
		// check that the file name is "Unity.exe"
		if (std::string(argv[1]).find("Unity.exe") == std::string::npos)
		{
			std::cout << "Unable to find the string \"Unity.exe\" in the file name." << std::endl;
			std::cin.get();
			return 2;
		}

		filename = argv[1];
	}

	// check that the file exists
	if (!fs::exists(fs::path(filename)))
	{
		std::cout << "File " << filename << " does not exist." << std::endl;
		std::cin.get();
		return 3;
	}
	
	backup(filename);
	buffer = getFileBuffer(filename, filesize);
	modifyBuffer(buffer, filesize);
	writeBuffer(buffer, filesize, filename);

	std::cout << "Patch successful" << std::endl;

	std::cin.get();
	return 0;
}