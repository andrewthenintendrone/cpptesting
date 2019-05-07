#include "MarkovChain.h"
#include <sstream>
#include <algorithm>
#include <iostream>

std::string toLower(const std::string& str)
{
	std::string value;

	for (auto c : str)
	{
		value += tolower(c);
	}

	return value;
}

void MarkovChain::buildDictionary(const char* filename)
{
	// open file to read in text
	m_file.open(filename, std::ios::_Nocreate, std::ios::binary);

	if (!m_file.is_open())
	{
		printf("Failed to open %s", filename);
		return;
	}

	std::string currentWord, nextWord;

	while (!m_file.eof())
	{
		currentWord = toLower(readWord());

		if (currentWord != "")
		{
			nextWord = toLower(peekWord());

			if (nextWord != "")
			{
				bool exists = false;
				bool nextExists = false;

				if (m_dictionary.find(currentWord) != m_dictionary.end())
				{
					// current word already exists in the dictionary
					exists = true;

					// check to make sure nextWord doesn't already exist as a next word for this word
					if (m_dictionary.find(currentWord)->second.nextWords.find(nextWord) != m_dictionary.find(currentWord)->second.nextWords.end())
					{
						// next word already exists for this word
						nextExists = true;
						m_dictionary.find(currentWord)->second.nextWords[nextWord]++;
					}

					// the next word has never been seen before after an instance of the current word
					if (!nextExists)
					{
						// add it as the first occurance
						m_dictionary.find(currentWord)->second.nextWords[nextWord] = 1;
					}
				}

				if (!exists)
				{
					// add the current word along with the next word
					m_dictionary[currentWord] = DictionaryEntry(currentWord, nextWord);
				}
			}
		}
	}

	m_file.close();
}

// writes dictionary to file
void MarkovChain::writeToFile(const char* filename)
{
	std::ofstream file;
	file.open(filename, std::ios::trunc);

	if (!file.is_open())
	{
		printf("Failed to open %s\n", filename);
		return;
	}

	for (auto iter = m_dictionary.begin(); iter != m_dictionary.end(); iter++)
	{
		file << iter->first.c_str() << ": ";

		for (auto iter2 = iter->second.nextWords.begin(); iter2 != iter->second.nextWords.end(); iter2++)
		{
			file << iter2->first << "(" << std::to_string(iter2->second) << "), ";
		}

		file << "\n";
	}

	file.close();
}

// generates text using the pre calculated dictionary
void MarkovChain::generateText(int numberOfWords)
{
	std::string currentWord = m_dictionary.begin()->first;

	for (size_t i = 0; i < numberOfWords; i++)
	{
		// get a random next word
		printf(currentWord.c_str());
		printf(" ");
		currentWord = m_dictionary[currentWord].getRandomNextWord();
	}

	printf("\n");
}

// read a word from the file without changing the position in the file
std::string MarkovChain::peekWord()
{
	// store current position
	std::streamoff position = m_file.tellg();

	if (m_file.eof())
	{
		return "";
	}

	// read next word into a string
	std::string word = readWord();

	// return to position
	m_file.seekg(position);

	return word;
}

// read a word from the file
std::string MarkovChain::readWord()
{
	/*std::string word = "";
	char c;

	while (true)
	{
		m_file.read(&c, 1);

		if (m_file.eof())
		{
			break;
		}

		if (c == '\n' || c == ' ')
		{
			break;
		}

		word += c;
	}*/

	std::string word;

	m_file >> word;

	return word;
}
