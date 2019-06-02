#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include "DictionaryEntry.h"

class MarkovChain
{
public:

	MarkovChain() {};

	void buildDictionary(const char* filename);

	void writeToFile(const char* filename);

	void generateText(unsigned int numberOfWords);

private:

	std::ifstream m_file;

	std::string readWord();
	std::string peekWord();

	std::unordered_map<std::string, DictionaryEntry> m_dictionary;
};