#include <iostream>
#include "Model.h"
#include <iomanip>

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		Model model = Model::createModelFromSprite(argv[1], 1.0f);
		std::string filename = argv[1];
		filename = filename.substr(0, filename.length() - 4) + ".obj";
		model.writeOBJ(filename.c_str());
	}

	std::cin.get();

	return 0;
}