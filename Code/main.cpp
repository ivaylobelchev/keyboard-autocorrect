/*
*	Ivaylo Georgiev Belchev
*	fn 62479
*	Software Engineering, FMI, Sofia University
*/

#include "menu/menu.hpp"

int main()
{
	std::cout << "Welcome!\n\n";

	// Enter input file
	std::cerr << "Loading dictionary...\n\n";
	//std::ifstream myFile ("../Dictionary//short_dictionary.json");
	std::ifstream myFile("../Dictionary/dictionary.json");
	if (!myFile.is_open())
	{
		std::cerr << "Error loading file!\n";
		std::cout << "Closing program!\nGoodbye!";
		return 0;
	}
	root = new trie;

	// Inputs the words from the dictionary.
	std::string temp;
	bool bracketOpened = false;
	size_t wordCounter = 0;
	while (std::getline(myFile, temp))
	{
		if (!bracketOpened)
		{
			for (size_t i = 0; i < temp.length(); ++i)
			{
				if (temp[i] == '{')
				{
					bracketOpened = true;
					break;
				}
			}
			continue;
		}

		std::string word;
		bool invertedComma = false;
		for (size_t i = 0; i < temp.length(); ++i)
		{
			if (temp[i] == '}')
			{
				bracketOpened = false;
				break;
			}
			if (!invertedComma)
			{
				if (temp[i] == '"') invertedComma = true;
				continue;
			}
			if (temp[i] == '"')
			{
				invertedComma = false;
				break;
			}
			word.push_back(temp[i]);
		}

		if (!bracketOpened)
		{
			bracketOpened = true;
			std::cout << "\rAll words added!        \n\n";
			break;
		}
		if (invertedComma)
		{
			std::cerr << "Error loading one word from dictionary! Inverted comma mismatch! Program continues...\n";
			continue;
		}

		addWord(word);
		++wordCounter;
		if (wordCounter == 1000) std::cout << wordCounter << " words added...";
		if (wordCounter % 1000 == 0) std::cout << '\r' << wordCounter << " words added...";
	}
	if (!bracketOpened)
	{
		std::cerr << "Error loading words from dictionary! Missing opening bracket!\n";
		std::cout << "Closing program!\nGoodbye!";
		return 0;
	}
	myFile.close();

	std::cout << wordCounter << " words in total have been added, starting with " << root->children.size() << " different characters.\n\n";
	// The included dictionary includes prefixes and suffixes and that's why my dictionary contains 27 different characters.

	// Calls the menu
	while (modes(false)) {}

	return 0;
}