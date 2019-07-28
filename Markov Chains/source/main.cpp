#include <iostream>
#include "MarkovChain.h"
#include <chrono>

int main(int argc, char* argv[])
{
	srand((int)std::chrono::system_clock::now().time_since_epoch().count());

	MarkovChain chain;

	if (argc > 1)
	{
		chain.buildDictionary(argv[1]);
		chain.generateText(2000);
	}

	//system("pause");
	std::cin.get();

	return 0;
}
