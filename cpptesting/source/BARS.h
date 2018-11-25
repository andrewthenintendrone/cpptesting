#pragma once
#include <string>
#include <fstream>
#include <vector>

class BARS
{
public:

	BARS(std::string filename);

	void extract();

	void close();

private:

	std::string m_filename;

	std::streamoff m_fileLength;

	std::ifstream m_inFile;
	std::ofstream m_currentOutFile;

	std::vector<unsigned int> m_fwavOffsets;
};