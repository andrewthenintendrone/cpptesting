#include <iostream>
#include <iomanip>
#include <fstream>
#include "Image.h"
#include "Model.h"
#include <string>
#include "Array2D.h"

bool ColorSort(Color& a, Color& b)
{
	if (a.g <= b.g)
	{
		return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			Image img(argv[i]);

			std::string filename = argv[i];

			filename = filename.substr(0, filename.length() - 4) + "_sobel.png";

			img.gaussianBlur().sobelEdgeDetection().save(filename.c_str());
		}
	}


	std::cin.get();

	return 0;
}