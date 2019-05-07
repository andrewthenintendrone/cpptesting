#include "CharacterMarkovChain.h"
#include <iostream>

void CharacterMarkovChain::buildMarkovChain(std::string baseData)
{
	// cycle through every character in the string
	for (int i = 0; i < baseData.size() - 1; i++)
	{
		char currentChar = baseData[i];
		char nextChar = baseData[i + 1];

		bool currentExists = false;

		// ensure both states exist in the vector
		for (auto& iter : m_states)
		{
			if (iter.currentState == currentChar)
			{
				currentExists = true;
				iter.probabilities[nextChar]++;
				iter.totalProbabilities++;
			}
		}

		if (!currentExists)
		{
			MarkovState m(currentChar);
			m.probabilities[nextChar] = 1;
			m.totalProbabilities = 1;
			m_states.push_back(m);
		}

		
	}
}

void CharacterMarkovChain::generateText()
{
	MarkovState currentState = m_states[0];

	for (int i = 0; i < 200; i++)
	{
		std::cout << currentState.currentState;

		char nextChar = currentState.getRandomNextChar();

		// make that state the current state
		for (auto& iter : m_states)
		{
			if (iter.currentState == nextChar)
			{
				currentState = iter;
				break;
			}
		}
	}
}
