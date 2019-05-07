#pragma once
#include <string>

class KartPart
{
public:

	KartPart(const std::string& name, const std::string& internalName = "")
		: m_name(name), m_internalName(internalName) {}

	std::string m_name;
	std::string m_internalName;

private:
};