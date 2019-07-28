#include "Model.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include "Endian.h"
#include "BinaryReader.h"
#include "Image.h"

bool validVector(glm::vec2& v)
{
	// not a number
	if (std::isnan(v.x) || std::isnan(v.y))
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

	return true;
}

bool validVector(glm::vec4& v)
{
	// not a number
	if (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w))
	{
		return false;
	}

	return true;
}

Model::Model(std::vector<glm::vec3> verts, std::vector<glm::vec2> uvs, std::vector<int> indices) : m_verts(verts), m_uvs(uvs), m_indices(indices)
{

}

// returns a bitmask of the surrounding pixels (used for sprite to model)
uint8_t getCorners(Image* sprite, int x, int y)
{
	uint8_t corners = 0;

	// top left
	if (x != 0 && y != sprite->getHeight()- 1 && sprite->getPixel(x - 1, y + 1, WRAP).a > 128)
	{
		corners |= 128;
	}

	// top
	if (y != sprite->getHeight() - 1 && sprite->getPixel(x, y + 1, WRAP).a > 128)
	{
		corners |= 64;
	}

	// top right
	if (x != sprite->getWidth() - 1 && y != sprite->getHeight() - 1 && sprite->getPixel(x + 1, y + 1, WRAP).a > 128)
	{
		corners |= 32;
	}

	// left
	if (x != 0 && sprite->getPixel(x - 1, y, WRAP).a > 128)
	{
		corners |= 16;
	}

	// right
	if (x != sprite->getWidth() - 1 && sprite->getPixel(x + 1, y, WRAP).a > 128)
	{
		corners |= 8;
	}

	// bottom left
	if (x != 0 && y == 0 && sprite->getPixel(x - 1, y - 1, WRAP).a > 128)
	{
		corners |= 4;
	}

	// bottom
	if (y != 0 && sprite->getPixel(x, y - 1, WRAP).a > 128)
	{
		corners |= 2;
	}

	// bottom right
	if (x != sprite->getWidth() - 1 && y != 0 && sprite->getPixel(x + 1, y - 1, WRAP).a > 128)
	{
		corners |= 1;
	}

	return corners;
}

Model Model::createModelFromSprite(const std::string& filename, float meshHeight)
{
	// create model
	Model model;

	// open sprite
	Image img(filename.c_str());

	int opaquePixelCount = 0;

	float longerSide = std::max(img.getWidth(), img.getHeight());

	// add top verts
	for (int y = 0, i = 0; y < img.getHeight(); y++)
	{
		for (int x = 0; x < img.getWidth(); x++, i++)
		{
			// if the current pixel is opaque
			if (img.getPixel(x, y, WRAP).a > 128)
			{
				opaquePixelCount++;

				// create verts
				model.m_verts.push_back(glm::vec3(x - 0.5f, meshHeight * 0.5f, y - 0.5f) / longerSide);
				model.m_verts.push_back(glm::vec3(x + 0.5f, meshHeight * 0.5f, y - 0.5f) / longerSide);
				model.m_verts.push_back(glm::vec3(x - 0.5f, meshHeight * 0.5f, y + 0.5f) / longerSide);
				model.m_verts.push_back(glm::vec3(x + 0.5f, meshHeight * 0.5f, y + 0.5f) / longerSide);

				// create uvs
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
			}
		}
	}

	// copy for bottom verts
	for (int y = 0, i = 0; y < img.getHeight(); y++)
	{
		for (int x = 0; x < img.getWidth(); x++, i++)
		{
			// if the current pixel is opaque
			if (img.getPixel(x, y, WRAP).a > 128)
			{
				// create verts
				model.m_verts.push_back(glm::vec3(x - 0.5f, -meshHeight * 0.5f, y - 0.5f) / longerSide);
				model.m_verts.push_back(glm::vec3(x + 0.5f, -meshHeight * 0.5f, y - 0.5f) / longerSide);
				model.m_verts.push_back(glm::vec3(x - 0.5f, -meshHeight * 0.5f, y + 0.5f) / longerSide);
				model.m_verts.push_back(glm::vec3(x + 0.5f, -meshHeight * 0.5f, y + 0.5f) / longerSide);

				// create uvs
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
				model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
			}
		}
	}

	// create top indices
	for (int i = 0; i < opaquePixelCount; i++)
	{
		int a = i * 4;
		int b = i * 4 + 1;
		int c = i * 4 + 2;
		int d = i * 4 + 3;

		model.m_indices.push_back(c);
		model.m_indices.push_back(b);
		model.m_indices.push_back(a);

		model.m_indices.push_back(c);
		model.m_indices.push_back(d);
		model.m_indices.push_back(b);
	}

	// create bottom indices
	for (int i = 0; i < opaquePixelCount; i++)
	{
		int a = opaquePixelCount * 4 + i * 4;
		int b = opaquePixelCount * 4 + i * 4 + 1;
		int c = opaquePixelCount * 4 + i * 4 + 2;
		int d = opaquePixelCount * 4 + i * 4 + 3;

		model.m_indices.push_back(a);
		model.m_indices.push_back(b);
		model.m_indices.push_back(c);

		model.m_indices.push_back(b);
		model.m_indices.push_back(d);
		model.m_indices.push_back(c);
	}

	// create edge indices
	for (int y = 0, i = opaquePixelCount * 8; y < img.getHeight(); y++)
	{
		for (int x = 0; x < img.getWidth(); x++)
		{
			// only fill in corners for opaque pixels
			if (img.getPixel(x, y, WRAP).a > 128)
			{
				uint8_t corners = getCorners(&img, x, y);

				// top edge
				if ((corners & 64) != 64)
				{
					model.m_verts.push_back(glm::vec3(x - 0.5f, meshHeight * 0.5f, y + 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x + 0.5f, meshHeight * 0.5f, y + 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x - 0.5f, -meshHeight * 0.5f, y + 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x + 0.5f, -meshHeight * 0.5f, y + 0.5f) / longerSide);

					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));

					model.m_indices.push_back(i + 2);
					model.m_indices.push_back(i + 1);
					model.m_indices.push_back(i);

					model.m_indices.push_back(i + 2);
					model.m_indices.push_back(i + 3);
					model.m_indices.push_back(i + 1);

					i += 4;
				}

				// bottom edge
				if ((corners & 2) != 2)
				{
					model.m_verts.push_back(glm::vec3(x - 0.5f, meshHeight * 0.5f, y - 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x + 0.5f, meshHeight * 0.5f, y - 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x - 0.5f, -meshHeight * 0.5f, y - 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x + 0.5f, -meshHeight * 0.5f, y - 0.5f) / longerSide);

					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));

					model.m_indices.push_back(i);
					model.m_indices.push_back(i + 1);
					model.m_indices.push_back(i + 2);

					model.m_indices.push_back(i + 1);
					model.m_indices.push_back(i + 3);
					model.m_indices.push_back(i + 2);

					i += 4;
				}

				// left edge
				if ((corners & 16) != 16)
				{
					model.m_verts.push_back(glm::vec3(x - 0.5f, meshHeight * 0.5f, y + 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x - 0.5f, meshHeight * 0.5f, y - 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x - 0.5f, -meshHeight * 0.5f, y + 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x - 0.5f, -meshHeight * 0.5f, y - 0.5f) / longerSide);

					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));

					model.m_indices.push_back(i);
					model.m_indices.push_back(i + 1);
					model.m_indices.push_back(i + 2);

					model.m_indices.push_back(i + 1);
					model.m_indices.push_back(i + 3);
					model.m_indices.push_back(i + 2);

					i += 4;
				}

				// right edge
				if ((corners & 8) != 8)
				{
					model.m_verts.push_back(glm::vec3(x + 0.5f, meshHeight * 0.5f, y - 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x + 0.5f, meshHeight * 0.5f, y + 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x + 0.5f, -meshHeight * 0.5f, y - 0.5f) / longerSide);
					model.m_verts.push_back(glm::vec3(x + 0.5f, -meshHeight * 0.5f, y + 0.5f) / longerSide);

					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));
					model.m_uvs.push_back(glm::vec2((float)x / (float)(img.getWidth() - 1.0f), 1.0f - (float)y / (float)(img.getHeight() - 1.0f)));

					model.m_indices.push_back(i);
					model.m_indices.push_back(i + 1);
					model.m_indices.push_back(i + 2);

					model.m_indices.push_back(i + 1);
					model.m_indices.push_back(i + 3);
					model.m_indices.push_back(i + 2);

					i += 4;
				}
			}
		}
	}

	// finally center mesh vertices
	for (int i = 0; i < model.m_verts.size(); i++)
	{
		model.m_verts[i] -= glm::vec3(img.getWidth() * 0.5f / longerSide, 0.0f, img.getHeight() * 0.5f / longerSide);
	}

	return model;
}

Model Model::createModelFromSprite2(const std::string& filename, float meshHeight)
{
	// create model
	Model model;

	// open sprite
	Image img(filename.c_str());

	float width = img.getWidth();
	float height = img.getHeight();

	// create grid
	for (int y = 0; y < height + 1; y++)
	{
		for (int x = 0; x < width + 1; x++)
		{
			model.m_verts.push_back(glm::vec3(x, 0, y));
			model.m_uvs.push_back(glm::vec2((float)x / (float)width, 1.0f - (float)y / (float)height));
		}
	}



	// create indices
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int i = (width + 1) * y + x;
			int i2 = i + 1;
			int i3 = i + width + 1;
			int i4 = i3 + 1;

			model.m_indices.push_back(i);
			model.m_indices.push_back(i2);
			model.m_indices.push_back(i3);

			model.m_indices.push_back(i3);
			model.m_indices.push_back(i2);
			model.m_indices.push_back(i4);
		}
	}

	return model;
}

void Model::loadRaw(const std::string& filename, int startAddress /* 0 */, int endAddress /* 0 */)
{
	BinaryReader reader(filename);

	// if endAddress is 0 set it to fileSize
	if (endAddress == 0)
	{
		endAddress = (int)reader.getFileSize();
	}

	int vecCount = 50;

	// read data as verts 
	while (reader.getFileOffset() < endAddress - sizeof(glm::vec3))
	{
		glm::vec3 currentVector;

		currentVector.x = reader.readSingle();
		currentVector.y = reader.readSingle();
		currentVector.z = reader.readSingle();

		if (validVector(currentVector))
		{
			if (glm::length(currentVector) >= 0.99f && glm::length(currentVector) <= 1.01f)
			{
				if (vecCount > 0)
				{
					std::cout << "normal vector at " << reader.getFileOffset() << std::endl;
					vecCount--;
				}
			}
			m_verts.push_back(currentVector);
		}
	}
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
	std::ifstream file;

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

	// write uvs (if they exist)
	for (int i = 0; i < m_uvs.size(); i++)
	{
		file << "vt " << std::setprecision(6) << m_uvs[i].x << " " << m_uvs[i].y << std::endl;
	}

	// write indices
	for (int i = 0; i < m_indices.size(); i += 3)
	{
		file << "f ";
		file << m_indices[i] + 1 << "/" << m_indices[i] + 1 << " ";
		file << m_indices[i + 1] + 1 << "/" << m_indices[i + 1] + 1 << " ";
		file << m_indices[i + 2] + 1 << "/" << m_indices[i + 2] + 1 << std::endl;
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
