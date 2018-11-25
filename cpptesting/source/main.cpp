#include "Model.h"
#include <string>
#include <experimental\filesystem>
namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
	for(int i = 1; i < argc; i++)
	{
		Model model;
		model.loadRaw(argv[i], 0, 0);
		model.writeOBJ(std::string("C:\\Users\\Andrew\\Desktop\\model.obj").c_str());
	}

	system("pause");
	return 0;
}