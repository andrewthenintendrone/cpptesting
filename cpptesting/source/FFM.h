#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <glm\matrix.hpp>
#include <vector>

class FFM
{
public:

	FFM() {};
	FFM(const char* filename);

	void loadFFM(const char* filename);

	void writeFile(const char* filename);

private:

	// array of indices used for faces
	std::vector<glm::ivec3> m_faceArray;
	// array of vertex positions
	std::vector<glm::vec3> m_vertexArray;
	// array of vertex normals
	std::vector<glm::vec3> m_normalArray;
	// array of vertex tangents
	std::vector<glm::vec3> m_tangentArray;
	// array of vertex bitangents
	std::vector<glm::vec3> m_bitangentArray;
	// array of texture coordinates
	std::vector<glm::vec2> m_texCoordArray;
};