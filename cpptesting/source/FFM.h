#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <glm\matrix.hpp>
#include <vector>

// Candy crush friends FFM file
class FFM
{
public:

	FFM() {};
	FFM(const std::string& filename);

	void loadFFM(const std::string& filename);

	void writeFile(const std::string& filename);

private:

	// indices
	std::vector<glm::ivec3> m_faceArray;

	// verts
	std::vector<glm::vec3> m_vertexArray;

	// normals
	std::vector<glm::vec3> m_normalArray;

	// tangents
	std::vector<glm::vec3> m_tangentArray;

	// bitangents
	std::vector<glm::vec3> m_bitangentArray;

	// uvs
	std::vector<glm::vec2> m_texCoordArray;
};