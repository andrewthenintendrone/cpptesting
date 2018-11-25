#include "Model.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include "Endian.h"

bool validVector(glm::vec2& v)
{
	// not a number
	if (std::isnan(v.x) || std::isnan(v.y))
	{
		return false;
	}
	// not normal
	if (!std::isnormal(v.x) || !std::isnormal(v.y))
	{
		return false;
	}

	return true;
}

bool validVector(glm::vec3& v)
{
	// not a number
	if (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z))
	{
		return false;
	}
	// not normal
	if (!std::isnormal(v.x) || !std::isnormal(v.y) || !std::isnormal(v.z))
	{
		return false;
	}

	return true;
}

bool validVector(glm::vec4& v)
{
	// not a number
	if (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w))
	{
		return false;
	}
	// not normal
	if (!std::isnormal(v.x) || !std::isnormal(v.y) || !std::isnormal(v.z) || !std::isnormal(v.w))
	{
		return false;
	}

	return true;
}

Model::Model(std::vector<glm::vec3> verts, std::vector<int> indices) : m_verts(verts), m_indices(indices)
{

}

void Model::loadRaw(const std::string& filename, int startAddress, int endAddress)
{
	file.open(filename, std::ios::binary | std::ios::_Nocreate | std::ios::ate);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	// store file size
	int fileSize = (int)file.tellg();

	// if endAddress is 0 set it to fileSize
	if (endAddress == 0)
	{
		endAddress = fileSize;
	}

	// return to start address
	file.seekg(startAddress, std::ios::beg);

	int normalCount = 0;
	int positionCount = 0;

	// read data as verts 
	while ((int)file.tellg() < endAddress - sizeof(glm::vec4))
	{
		glm::vec4 currentVector;

		file.read(reinterpret_cast<char*>(&currentVector), sizeof(glm::vec4));

		if (validVector(currentVector))
		{
			if (glm::length(currentVector) == 1.0f)
			{
				normalCount++;
			}
			else
			{
				positionCount++;
			}

			m_verts.push_back(currentVector);
		}
	}

	file.close();
}

size_t split(const std::string& txt, std::vector<std::string>& strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos)
	{
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

// read in obj file
void Model::loadOBJ(const std::string& filename)
{
	file.open(filename, std::ios::_Nocreate);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	std::string currentLine;
	std::vector<std::string> currentwords;

	while (std::getline(file, currentLine))
	{
		split(currentLine, currentwords, ' ');

		if (currentwords[0] == "v")
		{
			float x = std::stof(currentwords[1]);
			float y = std::stof(currentwords[2]);
			float z = std::stof(currentwords[3]);

			float r = std::stof(currentwords[4]);
			float g = std::stof(currentwords[5]);
			float b = std::stof(currentwords[6]);
			
			m_verts.push_back(glm::vec3(x, y, z));
			m_colors.push_back(glm::vec3(r, g, b));
		}
		else if (currentwords[0] == "f")
		{
			m_indices.push_back(std::stoi(currentwords[1]) - 1);
			m_indices.push_back(std::stoi(currentwords[2]) - 1);
			m_indices.push_back(std::stoi(currentwords[3]) - 1);
		}
	}

	file.close();
}

void Model::writeOBJ(const std::string& filename)
{
	std::ofstream file;

	file.open(filename, std::ios::trunc);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	// write obj header
	file << "o model" << std::endl;

	// write verts
	for (int i = 0; i < m_verts.size(); i++)
	{
		file << "v " << std::setprecision(6) << m_verts[i].x << " " << m_verts[i].y << " " << m_verts[i].z << std::endl;
	}

	// write indices
	for (int i = 0; i < m_indices.size(); i += 3)
	{
		file << "f " << m_indices[i] << " " << m_indices[i + 1] << " " << m_indices[i + 2] << std::endl;
	}

	file.close();
}

void Model::writePLY(const std::string& filename)
{
	std::ofstream file;

	file.open(filename, std::ios::trunc);

	if (!file.is_open())
	{
		std::cout << "Failed to open " << filename << std::endl;
		return;
	}

	// write ply header
	file << "ply\n";
	file << "format ascii 1.0\n";
	file << "element vertex " << m_verts.size() << std::endl;
	file << "property float x\n";
	file << "property float y\n";
	file << "property float z\n";
	file << "property uchar red\n";
	file << "property uchar green\n";
	file << "property uchar blue\n";
	file << "element face " << m_indices.size() / 3 << std::endl;
	file << "property list uchar int vertex_indices\n";
	file << "end_header\n";

	// write verts
	for (int i = 0; i < m_verts.size(); i++)
	{
		// position
		file << m_verts[i].x << " " << m_verts[i].y << " " << m_verts[i].z << " ";

		// color
		file << (int)(m_colors[i].r * 255) << " " << (int)(m_colors[i].g * 255) << " " << (int)(m_colors[i].b * 255) << std::endl;
	}

	// write indices
	for (int i = 0; i < m_indices.size(); i += 3)
	{
		file << "3 " << m_indices[i] << " " << m_indices[i + 1] << " " << m_indices[i + 2] << std::endl;
	}

	file.close();
}
