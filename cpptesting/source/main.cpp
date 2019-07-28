#include <iostream>
//#include "Model.h"
#include <experimental/filesystem>
#include "bprmFile.h"

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
	for (auto& p : fs::directory_iterator("C:\\Users\\Andrew\\Downloads\\splatoon2\\update\\out\\Param\\TeamColor"))
	{
		if (p.path().has_extension() && p.path().extension() == ".bprm")
		{
			bprmFile bprm;
			bprm.readFile(p.path().string());
			bprm.printColors();
		}
	}

	std::cin.get();

	return 0;
}