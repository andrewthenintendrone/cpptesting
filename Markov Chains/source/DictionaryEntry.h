#pragma once
#include <string>
#include <unordered_map>

struct DictionaryEntry
{
	DictionaryEntry() { }

	void addNextWord(const std::string& nw)
	{
		nextWords[nw]++;
	}

	// gets a random next word weighted by the number of occurences
	std::string getRandomNextWord()
	{
		// add up every occurence of words after this one
		size_t totalPossibleNextWords = 0;

		for (auto iter = nextWords.begin(); iter != nextWords.end(); iter++)
		{
			totalPossibleNextWords += iter->second;
		}

		// pick a random index out of all the words that followed
		size_t randomIndex = rand() % totalPossibleNextWords;

		size_t currentCheckIndex = 0;

		for (auto iter = nextWords.begin(); iter != nextWords.end(); iter++)
		{
			if (currentCheckIndex >= randomIndex)
			{
				return iter->first;
			}

			currentCheckIndex += iter->second;
		}

		return nextWords.begin()->first;
	}

	std::unordered_map<std::string, int> nextWords;
};