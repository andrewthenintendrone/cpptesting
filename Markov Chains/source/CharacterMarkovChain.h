#pragma once

#include <vector>
#include <unordered_map>

struct MarkovState
{
	MarkovState() {}
	MarkovState(char c) { currentState = c; }

	char currentState;

	std::unordered_map<char, int> probabilities;

	int totalProbabilities = 0;

	// gets a random next character weighted by the number of occurences
	char getRandomNextChar()
	{
		// pick a random index out of all the words that followed
		size_t randomIndex = rand() % totalProbabilities;

		size_t currentCheckIndex = 0;

		for (auto iter = probabilities.begin(); iter != probabilities.end(); iter++)
		{
			currentCheckIndex += iter->second;

			if (currentCheckIndex >= randomIndex)
			{
				return iter->first;
			}
		}

		return probabilities.begin()->first;
	}
};

class CharacterMarkovChain
{
	public:

		CharacterMarkovChain() {}

		void buildMarkovChain(std::string baseData);

		void generateText();

	private:

		std::vector<MarkovState> m_states;
};