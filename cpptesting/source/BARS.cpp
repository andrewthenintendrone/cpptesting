#include "BARS.h"
#include <iostream>
#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

// constructor
BARS::BARS(std::string filename)
{
	// load bars file
	m_filename = filename;

	m_inFile.open(m_filename, std::ios::binary | std::ios::_Nocreate | std::ios::ate);

	if (!m_inFile.is_open())
	{
		std::cout << "Failed to open" << m_filename << std::endl;
		return;
	}

	// store file length
	m_fileLength = m_inFile.tellg();

	// return to beginning of file
	m_inFile.seekg(0, std::ios::beg);
}

// extract fwav files
void BARS::extract()
{
	char magic[4] = "   ";

	// find all fwav offsets
	while (m_inFile.tellg() < m_fileLength - 4)
	{
		m_inFile.read(magic, 4);

		// store fwav offset
		if (magic[0] == 'F' && magic[1] == 'W' && magic[2] == 'A' && magic[3] == 'V')
		{
			m_fwavOffsets.push_back((unsigned int)m_inFile.tellg() - 4);
		}
	}

	std::cout << "Found " << m_fwavOffsets.size() << " fwavs\n";

	// create output folder if it doen't exist
	std::string filepath = fs::path(m_filename).parent_path().string() + "\\" + fs::path(m_filename).stem().string() + "\\";

	if (!fs::exists(filepath))
	{
		fs::create_directory(filepath);
	}

	// dump each one
	for (int i = 0; i < m_fwavOffsets.size(); i++)
	{
		// jump to the start of the fwav
		m_inFile.seekg(m_fwavOffsets[i], std::ios::beg);

		unsigned int bufferLength;

		// calculate buffer length (don't run off end)
		if (i < m_fwavOffsets.size() - 1)
		{
			bufferLength = m_fwavOffsets[i + 1] - m_fwavOffsets[i];
		}
		else
		{
			bufferLength = m_fileLength - m_fwavOffsets[i];
		}

		// create buffer
		std::vector<char> buffer(bufferLength);

		// read into buffer
		m_inFile.read(buffer.data(), bufferLength);

		// open new file
		m_currentOutFile.open(std::string(filepath + "file_" + std::to_string(i) + ".fwav"), std::ios::binary | std::ios::trunc);

		// check open
		if (!m_currentOutFile.is_open())
		{
			std::cout << "Failed to open" << filepath + "file_" + std::string(0) + ".fwav" << std::endl;
			return;
		}

		// write current buffer to current file
		m_currentOutFile.write(buffer.data(), bufferLength);

		// close current file
		m_currentOutFile.close();
	}
}

// close BARS file
void BARS::close()
{
	if (m_inFile.is_open())
	{
		m_inFile.close();
	}
}
