#include "BFSTM.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Endian.h"

BFSTM::BFSTM(const char* filename)
{
	readData(filename);
}

void BFSTM::readData(const char* filename)
{
	std::ifstream file;

	file.open(filename, std::ios::_Nocreate | std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	char magic[5] = "    ";

	// first attempt to find "INFO"
	while (!file.eof())
	{
		file.read(&magic[0], sizeof(magic) - 1);

		if (std::string(magic) == "INFO")
		{
			//std::cout << "Found INFO magic at 0x" << std::hex << ((int)file.tellg() - 4) << std::endl;

			// skip the next 8 bytes of the INFO structure
			file.seekg(8, std::ios::cur);

			// read the stream info offset
			int32_t streamInfoOffset;
			file.read(reinterpret_cast<char*>(&streamInfoOffset), sizeof(streamInfoOffset));

			//std::cout << "Stream info offset: " << std::dec << streamInfoOffset << std::endl;

			// jump to stream info offset
			file.seekg((int)streamInfoOffset - 8, std::ios::cur);

			// print sound encoding
			uint8_t soundEncoding = 0;
			file.read(reinterpret_cast<char*>(&soundEncoding), sizeof(soundEncoding));
			//printSoundEncoding(soundEncoding);

			// does the audio loop
			uint8_t loops = 0;
			file.read(reinterpret_cast<char*>(&loops), sizeof(loops));
			/*if (loops == 1)
			{
				std::cout << "Audio loops\n";
			}
			else
			{
				std::cout << "Audio does not loop\n";
			}*/

			// print the number of channels
			uint8_t channelCount = 0;
			file.read(reinterpret_cast<char*>(&channelCount), sizeof(channelCount));
			//std::cout << "Number of channels: " << (int)channelCount << std::endl;

			// print the number of regions
			uint8_t regionCount;
			file.read(reinterpret_cast<char*>(&regionCount), sizeof(regionCount));
			//std::cout << "Number of regions: " << (int)regionCount << std::endl;

			// print the sample rate
			uint32_t sampleRate;
			file.read(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
			//std::cout << "Sample rate: " << sampleRate << std::endl;

			// print the start point of the loop
			uint32_t loopA;
			file.read(reinterpret_cast<char*>(&loopA), sizeof(loopA));
			//std::cout << "Loop start point: " << loopA << std::endl;

			// print the end point of the loop
			uint32_t loopB;
			file.read(reinterpret_cast<char*>(&loopB), sizeof(loopB));
			//std::cout << "Loop end point: " << loopB << std::endl;

			std::cout << "new MusicLoopPoints(" << loopA << ", " << loopB << ")," << std::endl;

			break;
		}
		else
		{
			file.seekg((int)file.tellg() - 3, std::ios::beg);
		}
	}


	file.close();
}

void BFSTM::printSoundEncoding(uint8_t soundEncoding)
{
	std::cout << "Sound Encoding Type: ";

	switch (soundEncoding)
	{
	case 0:
		std::cout << "PCM8";
		break;
	case 1:
		std::cout << "PCM16";
		break;
	case 2:
		std::cout << "DSP ADPCM";
		break;
	case 3:
		std::cout << "IMA ADPCM";
		break;
	default:
		std::cout << "Unknown";
		break;
	}

	std::cout << std::endl;
}
