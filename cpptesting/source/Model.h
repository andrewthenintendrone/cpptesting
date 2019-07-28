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
	Model(std::vector<glm::vec3> verts, std::vector<glm::vec2> uvs, std::vector<int> indices);

	static Model createModelFromSprite(const std::string& filename, float meshHeight);
	static Model createModelFromSprite2(const std::string& filename, float meshHeight);

	void loadRaw(const std::string& filename, int startAddress = 0, int endAddress = 0);
	void loadOBJ(const std::string& filename);

	void writeOBJ(const std::string& filename);
	void writePLY(const std::string& filename);

protected:

	std::vector<glm::vec3> m_verts;
	std::vector<glm::vec2> m_uvs;
	std::vector<int> m_indices;

	std::vector<glm::vec3> m_colors;
	uint32_t m_totalVerts = 0;
};