#include "bprmFile.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <experimental\filesystem>

namespace fs = std::experimental::filesystem;

std::string getColorHexCode(const glm::vec3& color)
{
	std::stringstream result;

	result << std::setfill('0') << std::setw(2) << std::hex << (int)(color.x * 255.0f);
	result << std::setfill('0') << std::setw(2) << std::hex << (int)(color.y * 255.0f);
	result << std::setfill('0') << std::setw(2) << std::hex << (int)(color.z * 255.0f);

	return std::string(result.str());
}

std::string getColorFloats(const glm::vec3& color)
{
	std::stringstream result;

	result << "[";
	result << color.x << ", ";
	result << color.y << ", ";
	result << color.z << ", ";
	result << "1.000000]";

	return std::string(result.str());
}

void bprmFile::readFile(const std::string& filename)
{
	m_filename = filename;

	std::ifstream file;
	file.open(m_filename, std::ios::_Nocreate | std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "can't open " << m_filename << std::endl;
	}

	// read color A
	file.seekg(0x90, std::ios::beg);
	file.read(reinterpret_cast<char*>(&m_teamColorA), sizeof(glm::vec3));

	// read color B
	file.seekg(0xa8, std::ios::beg);
	file.read(reinterpret_cast<char*>(&m_teamColorB), sizeof(glm::vec3));

	// read color N
	file.seekg(0xc0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&m_teamColorN), sizeof(glm::vec3));

	file.close();
}

void bprmFile::changeColors(Color colora, Color colorb, Color colorn)
{
	std::ifstream inFile;
	std::ofstream outFile;

	inFile.open(m_filename, std::ios::binary | std::ios::ate);

	if (!inFile.is_open())
	{
		std::cout << "Failed to open " << m_filename << std::endl;
		inFile.close();
		return;
	}

	std::streamsize fileSize = inFile.tellg();
	inFile.seekg(0, std::ios::beg);

	std::vector<char> buffer(fileSize);

	inFile.read(buffer.data(), fileSize);

	inFile.close();

	outFile.open("C:\\Users\\Andrew\\Desktop\\out\\" + fs::path(m_filename).filename().string(), std::ios::binary | std::ios::trunc);

	if (!outFile.is_open())
	{
		std::cout << "Failed to open " << "C:\\Users\\Andrew\\Desktop\\out\\" + fs::path(m_filename).filename().string() << std::endl;
		outFile.close();
		return;
	}

	// write buffer but change colors
	for (unsigned int i = 0; i < fileSize; i++)
	{
		// write color a
		if (i == 0x90)
		{
			outFile.write(reinterpret_cast<char*>(&colora.asVec3()), sizeof(glm::vec3));
			i += sizeof(glm::vec3) - 1;
		}
		// write color b
		else if(i == 0xa8)
		{
			outFile.write(reinterpret_cast<char*>(&colorb.asVec3()), sizeof(glm::vec3));
			i += sizeof(glm::vec3) - 1;
		}
		// write color n
		else if (i == 0xc0)
		{
			outFile.write(reinterpret_cast<char*>(&colorn.asVec3()), sizeof(glm::vec3));
			i += sizeof(glm::vec3) - 1;
		}
		else
		{
			outFile.write(&buffer[i], 1);
		}
	}

	outFile.close();
};

void bprmFile::printColors()
{
	std::ofstream file;
	file.open("C:\\Users\\Andrew\\Downloads\\splatoon2\\update\\out\\Param\\TeamColor\\team colors.txt", std::ios::app);

	if (!file.is_open())
	{
		std::cout << "Couldn't open C:\\Users\\Andrew\\Downloads\\splatoon2\\update\\out\\Param\\TeamColor\\team colors.txt" << std::endl;
	}

	// write filename stem
	std::cout << fs::path(m_filename).stem().string() << std::endl;
	file << fs::path(m_filename).stem().string() << std::endl;

	// write TeamColorA as floats
	std::cout << "TeamColorA: " << getColorFloats(m_teamColorA) << std::endl;
	file << "TeamColorA: " << getColorFloats(m_teamColorA) << std::endl;

	// write TeamColorB as floats
	std::cout << "TeamColorB: " << getColorFloats(m_teamColorB) << std::endl;
	file << "TeamColorB: " << getColorFloats(m_teamColorB) << std::endl;

	// write TeamColorN as floats
	std::cout << "TeamColorN: " << getColorFloats(m_teamColorN) << std::endl;
	file << "TeamColorN: " << getColorFloats(m_teamColorN) << std::endl;

	// write TeamColorA as hex code
	std::cout << "TeamColorA: " << getColorHexCode(m_teamColorA) << std::endl;
	file << "TeamColorA: " << getColorHexCode(m_teamColorA) << std::endl;

	// write TeamColorB as hex code
	std::cout << "TeamColorB: " << getColorHexCode(m_teamColorB) << std::endl;
	file << "TeamColorB: " << getColorHexCode(m_teamColorB) << std::endl;

	// write TeamColorN as hex code
	std::cout << "TeamColorN: " << getColorHexCode(m_teamColorN) << std::endl << std::endl;
	file << "TeamColorN: " << getColorHexCode(m_teamColorN) << std::endl << std::endl;

	file.close();
}
