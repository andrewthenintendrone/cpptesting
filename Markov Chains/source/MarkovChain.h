#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <random>

struct DictionaryEntry
{
	DictionaryEntry()
	{

	}

	DictionaryEntry(const std::string& w, const std::string& nw)
	{
		nextWords[nw] = 1;
	}

	std::unordered_map<std::string, int> nextWords;

	// gets a random next word weighted by the number of occurences
	std::string getRandomNextWord()
	{
		size_t totalPossibleNextWords = 1;

		for (auto iter = nextWords.begin(); iter != nextWords.end(); iter++)
		{
			totalPossibleNextWords += iter->second;
		}

		size_t randomIndex = rand() % totalPossibleNextWords;

		size_t currentCheckIndex = 0;

		for (auto iter = nextWords.begin(); iter != nextWords.end(); iter++)
		{
			currentCheckIndex += iter->second;

			if (currentCheckIndex >= randomIndex)
			{
				return iter->first;
			}
		}

		return nextWords.begin()->first;
	}
};

class MarkovChain
{
public:

	MarkovChain() {};

	void buildDictionary(const char* filename);

	void writeToFile(const char* filename);

	void generateText(int numberOfWords);

private:

	std::ifstream m_file;

	std::string peekWord();
	std::string readWord();

	std::unordered_map<std::string, DictionaryEntry> m_dictionary;
};