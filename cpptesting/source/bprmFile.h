#pragma once
#include <glm\vec3.hpp>
#include <fstream>
#include <string>
#include "Color.h"

// splatoon 2 bprm file
class bprmFile
{
public:

	void readFile(const std::string& filename);

	void changeColors(Color colora, Color colorb, Color colorn);

	void printColors();

private:

	std::string m_filename;

	glm::vec3 m_teamColorA;
	glm::vec3 m_teamColorB;
	glm::vec3 m_teamColorN;
};