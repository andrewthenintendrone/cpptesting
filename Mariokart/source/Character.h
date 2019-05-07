#pragma once
#include <string>
#include <vector>

class Character
{
public:

	Character(const std::string& name, const std::string& internalName1, const std::string& internalName2) :
		m_name(name), m_internalName1(internalName1), m_internalName2(internalName2) {}

	std::string m_name;
	std::string m_internalName1;
	std::string m_internalName2;

private:

};