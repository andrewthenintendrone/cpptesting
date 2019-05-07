#include <iostream>
#include <vector>
#include <string>

#include "Characters.h"
#include "KartParts.h"

#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

std::string parseMariokartFilename(const std::string& filename)
{
	std::string parsed = filename;
	std::cout << parsed << std::endl;

	// remove ^l

	std::cout << "Remove ^l" << std::endl;

	size_t foundPos = parsed.find("^l");

	if (foundPos != std::string::npos)
	{
		parsed = parsed.erase(foundPos, 2);
	}

	std::cout << parsed << std::endl;

	// replace vehicle name

	std::cout << "Replace Vehicle Name" << std::endl;

	for (auto iter = KartParts.begin(); iter != KartParts.end(); iter++)
	{
		foundPos = parsed.find(iter->m_internalName);

		if (foundPos != std::string::npos)
		{
			parsed.replace(foundPos, iter->m_internalName.length(), iter->m_name);
			break;
		}
	}

	std::cout << parsed << std::endl;

	// replace character name

	std::cout << "Replace Character Name" << std::endl;

	for (auto iter = Characters.begin(); iter != Characters.end(); iter++)
	{
		foundPos = parsed.find(iter->m_internalName2);

		if (foundPos != std::string::npos)
		{
			parsed.replace(foundPos, iter->m_internalName2.length(), iter->m_name);
			break;
		}
	}

	std::cout << parsed << std::endl;

	return parsed;
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		std::string filename = argv[1];

		parseMariokartFilename(filename);
	}

	std::cin.get();

	return 0;
}