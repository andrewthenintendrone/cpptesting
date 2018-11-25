#include "FFM.h"
#include "Endian.h"

template <typename T>
T read(std::ifstream& stream)
{
	T value;
	stream.read(reinterpret_cast<char*>(&value), sizeof(T));
	return value;
}

// constructor with filename
FFM::FFM(const char* filename)
{
	loadFFM(filename);
}

// load and read from the ffm file
void FFM::loadFFM(const char* filename)
{
	// open file in binary mode (don't create it)
	std::ifstream file;
	file.open(filename, std::ios::_Nocreate | std::ios::binary);

	// make sure the file actually opened
	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	// skip the first 8 bytes of the header
	file.seekg(0x8, std::ios::beg);

	// read boneCount
	uint32_t boneCount = read<uint32_t>(file);

	if (boneCount > 0)
	{
		std::cout << "found " << boneCount << " bones. skipping them." << std::endl;
		// skip the bone data
		file.seekg(0x66 * boneCount, std::ios::cur);
	}

	// read segment id
	uint32_t segmentID;
	segmentID = swapEndian<uint32_t>(read<uint32_t>(file));
	if (segmentID == 0x01044800)
	{
		std::cout << "reading faces" << std::endl;
	}

	// faceArray
	uint32_t indexCount = read<uint32_t>(file);

	for (size_t i = 0; i < indexCount; i += 3)
	{
		m_faceArray.push_back(glm::vec3(read<uint16_t>(file) + 1, read<uint16_t>(file) + 1, read<uint16_t>(file) + 1));
	}

	segmentID = swapEndian<uint32_t>(read<uint32_t>(file));
	if (segmentID == 0x0101d200)
	{
		std::cout << "reading positions" << std::endl;
	}

	// positionArray
	indexCount = read<uint32_t>(file);

	for (size_t i = 0; i < indexCount; i++)
	{
		m_vertexArray.push_back(read<glm::vec3>(file));
	}

	segmentID = swapEndian<uint32_t>(read<uint32_t>(file));
	if (segmentID == 0x0102d200)
	{
		std::cout << "reading normals" << std::endl;
	}

	// normalArray
	indexCount = read<uint32_t>(file);

	for (size_t i = 0; i < indexCount; i++)
	{
		m_normalArray.push_back(read<glm::vec3>(file));
	}

	segmentID = swapEndian<uint32_t>(read<uint32_t>(file));
	if (segmentID == 0x0108d200)
	{
		std::cout << "reading tangents" << std::endl;
	}

	// tangentArray
	indexCount = read<uint32_t>(file);

	for (size_t i = 0; i < indexCount; i++)
	{
		m_tangentArray.push_back(read<glm::vec3>(file));
	}

	segmentID = swapEndian<uint32_t>(read<uint32_t>(file));
	if (segmentID == 0x0109D200)
	{
		std::cout << "reading bitangents" << std::endl;
	}

	// bitangentArray array
	indexCount = read<uint32_t>(file);

	for (size_t i = 0; i < indexCount; i++)
	{
		m_bitangentArray.push_back(read<glm::vec3>(file));
	}

	segmentID = swapEndian<uint32_t>(read<uint32_t>(file));
	if (segmentID == 0x01039200)
	{
		std::cout << "reading texture coordinates" << std::endl;
	}
	else
	{
		// extra bone stuff?
		indexCount = read<uint32_t>(file);

		file.seekg(0x10 * indexCount, std::ios::cur);

		segmentID = swapEndian<uint32_t>(read<uint32_t>(file));
		if (segmentID == 0x01039200)
		{
			std::cout << "reading texture coordinates" << std::endl;
		}
	}

	// texture coordinates
	indexCount = read<uint32_t>(file);

	for (size_t i = 0; i < indexCount; i++)
	{
		m_texCoordArray.push_back(read<glm::vec2>(file));
	}

	file.close();
}

void FFM::writeFile(const char* filename)
{
	std::ofstream file;
	file.open(filename, std::ios::trunc);

	// header
	file << "o model\n";

	// vertex positions
	for (size_t i = 0; i < m_vertexArray.size(); i++)
	{
		file << "v " << m_vertexArray[i].x << " " << m_vertexArray[i].y << " " << m_vertexArray[i].z << std::endl;
	}

	// texture coordinates (y axis is flipped)
	for (size_t i = 0; i < m_texCoordArray.size(); i++)
	{
		file << "vt " << m_texCoordArray[i].x << " " << (1 - m_texCoordArray[i].y) << std::endl;
	}

	// normals
	for (size_t i = 0; i < m_normalArray.size(); i++)
	{
		file << "vn " << m_normalArray[i].x << " " << m_normalArray[i].y << " " << m_normalArray[i].z << std::endl;
	}

	// faces
	for (size_t i = 0; i < m_faceArray.size(); i++)
	{
		file << "f " << m_faceArray[i].x << "/" << m_faceArray[i].x << "/" << m_faceArray[i].x << " ";
		file << m_faceArray[i].y << "/" << m_faceArray[i].y << "/" << m_faceArray[i].y << " ";
		file << m_faceArray[i].z << "/" << m_faceArray[i].z << "/" << m_faceArray[i].z << std::endl;
	}

	file.close();
}
