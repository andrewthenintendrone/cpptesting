#pragma once
#include <string>
#include <fstream>
#include "NutexImageFormat.h"

struct NutexbProperties
{
	uint32_t padding;
	uint32_t height = 0;
	uint32_t width = 0;
	uint32_t depth = 0;
	NutexImageFormat format;
	uint8_t unk = 0;
	uint16_t padding2 = 0;
	int32_t unk2 = 0;
	uint32_t mipCount = 0;
	int32_t alignment = 0;
	uint32_t arrayCount = 0; // 6 for cubemaps
	int32_t imageSize = 0;
};

class Nutexb
{
public:

	Nutexb() {};
	Nutexb(const std::string& filename);
	Nutexb(char* filename);

	void open(const std::string& filename);

	~Nutexb();

	void readProperties();

private:

	std::ifstream m_file;

	std::streampos m_filesize;

	NutexbProperties m_properties;
};