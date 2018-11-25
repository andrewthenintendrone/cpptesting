#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <string>
#include <fstream>

struct Vertex
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
};

class Model
{
public:

	Model() {};
	Model(std::vector<glm::vec3> verts, std::vector<int> indices);

	void loadRaw(const std::string& filename, int startAddress, int endAddress);
	void loadOBJ(const std::string& filename);


	void writeOBJ(const std::string& filename);
	void writePLY(const std::string& filename);

protected:

	std::vector<glm::vec3> m_verts;
	std::vector<glm::vec3> m_colors;
	std::vector<int> m_indices;
	std::ifstream file;
	uint32_t m_totalVerts = 0;
};