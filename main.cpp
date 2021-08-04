/*
*	Ivaylo Georgiev Belchev
*	fn 62479
*	Software Engineering, FMI, Sofia University
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <vector>

struct trie
{
	bool isWord;
	//short usedTimes;

	std::map<char, trie*> children;
	// Using std::map to save space.

	trie()
	{
		isWord = false;
		//usedTimes = -1;
		// If I were using an array, here I'd have to set all to nullptr.
	}
};

trie* root;

void addWord(std::string word)
{
	trie* node = root;
	for (char letter : word)
	{
		if (node->children.find(letter) == node->children.end())
		{
			node->children[letter] = new trie();
		}
		node = node->children[letter];
	}
	node->isWord = true;
	/*if (node->usedTimes == -1)
		node->usedTimes = 0;*/
}

bool findWord(std::string word)
{
	trie* node = root;
	for (char letter : word)
	{
		if (node->children.find(letter) == node->children.end())
		{
			return false;
			// if the node has not been reached
		}
		node = node->children[letter];
	}
	return node->isWord;
	// is the node exists
}

bool makeWordLowercase(std::string& word)
{
	for (size_t i = 0; i < word.size(); ++i)
	{
		if (word[i] >= 'a' && word[i] <= 'z')
			continue;
		else if (word[i] >= 'A' && word[i] <= 'Z')
		{
			word[i] = word[i] + ('a' - 'A');
		}
		else
			return false;
		// Returns false if there are invalid characters.
	}
	return true;
}

bool isLastNode(trie* root)
{
	if (root->children.empty())
		return 1;
	return 0;
}

// This is a recursive function to print auto-suggestions for given node.
void suggestionsRecursive(trie* root, std::string currPrefix)
{
	if (root->isWord)
	{
		std::cout << currPrefix;
		std::cout << '\n';
		// Prints found word.
	}

	if (isLastNode(root))
		return;

	for (auto iter = root->children.begin(); iter != root->children.end(); ++iter)
	{
		if (&iter)
		{
			// append current character to currPrefix string 
			currPrefix.push_back(iter->first);

			// recur over the rest 
			if (root->children.find(iter->first) != root->children.end())
				suggestionsRecursive(root->children[iter->first], currPrefix);

			// remove last character after the recursion
			currPrefix.pop_back();
		}
	}
}

// print suggestions for given query prefix. 
int printAutoSuggestions(trie* root, const std::string query)
{
	trie* currentNode = root;

	// Check if prefix is present
	int level;
	int n = query.length();
	for (level = 0; level < n; level++)
	{
		// no string in the Trie has this prefix 
		if (!currentNode->children[query[level]])
			return 0;

		currentNode = currentNode->children[query[level]];
	}

	// If prefix is present as a word. 
	bool isWord = (currentNode->isWord == true);

	// If prefix has no children
	bool isLast = isLastNode(currentNode);

	// If prefix is present as a word and has no children
	if (isWord && isLast)
	{
		std::cout << query << '\n';
		return -1;
	}

	// If there are nodes below last matching character. 
	if (!isLast)
	{
		std::string prefix = query;
		suggestionsRecursive(currentNode, prefix);
		return 1;
	}
}


void autocompleteOneWord()
{
	//const int suggestions = -1; // NOT ACTIVE RN
	// -1 for all

	std::string word;
	std::getline(std::cin, word);

	if (!makeWordLowercase(word))
	{
		std::cout << "Invalid characters!\n";
		return;
	}

	int comp = printAutoSuggestions(root, word);

	if (comp == -1)
		std:: cout << "No other strings found with this prefix\n";

	else if (comp == 0)
		std::cout << "No string found with this prefix\n";

	//std::cout << "\nYour word is " << word << '\n';
}
 
// The same two functions as those above but with changes to accomodate this use
void suggestionsRecAutocorrect(trie * root, std::string currPrefix, std::vector<std::string>& words, size_t& queryLenght)
{ 
	if (root == nullptr) return;
	if (root->isWord)
	{
		if (currPrefix.size() == queryLenght)
			words.push_back(currPrefix);
	}

	if (isLastNode(root))
		return;

	for (auto iter = root->children.begin(); iter != root->children.end(); ++iter)
	{
		if (&iter)
		{
			currPrefix.push_back(iter->first);

			if (root->children.find(iter->first) != root->children.end())
				suggestionsRecAutocorrect(root->children[iter->first], currPrefix, words, queryLenght);

			currPrefix.pop_back();
		}
	}
}

 
int printAutocorrectSuggestions(trie* root, const std::string query, std::vector<std::string>& words, size_t& queryLenght)
{
	trie* currentNode = root;
 
	int level;
	int n = query.length();
	for (level = 0; level < n; level++)
	{
		if (!currentNode->children[query[level]])
			return 0;

		currentNode = currentNode->children[query[level]];
	}

	bool isWord = (currentNode->isWord == true);

	bool isLast = isLastNode(currentNode);

	if (isWord && isLast)
	{
		words.push_back(query);
		return -1;
	}

	if (!isLast)
	{
		std::string prefix = query;
		suggestionsRecAutocorrect(currentNode, prefix, words, queryLenght);
		return 1;
	}
}

// Return the number of matching characters
size_t matchingCharacters(std::string& original, std::string& compared)
{
	size_t counter = 0;
	for (size_t i = 0; i < original.size(); ++i)
	{
		if (original[i] == compared[i]) ++counter;
	}
	return counter;
}

// THIS FUNCTIONS IS MARKED AS EXPERIMENTAL
// This autocorrect solution is based on the autocomplete one.
// It only works to correct words with the same length.
void autocorrectOneWordNoAlg()
{
	std::string word;
	std::getline(std::cin, word);

	if (!makeWordLowercase(word))
	{
		std::cout << "Invalid characters!\n";
		return;
	}

	std::vector<std::string> words;

	std::string currentWord = word;
	size_t originalQueryLength = word.length();
	int comp = 0;
	while (comp == 0)
	{
		words.clear();
		comp = printAutocorrectSuggestions(root, currentWord, words, originalQueryLength);
		currentWord.pop_back();
		if (currentWord.empty())
		{
			std::cout << "Word not found!\n";
			return;
		}
	}

	// calculating letter difference and sorting by it

	std::priority_queue<std::pair<size_t, std::string>> pq;
	for (auto iter = words.begin(); iter != words.end(); ++iter)
	{
		pq.push(std::make_pair(matchingCharacters(word, (*iter)), (*iter)));
	}

	size_t best = pq.top().first;
	while (!pq.empty())
	{
		std::cout << '\"' << pq.top().second << "\" has " << word.size() - pq.top().first << " character difference.\n";
		pq.pop();
		if (pq.top().first != best) break;
	}
}

void autocorrectOneWordAlg()
{
	std::string originalWord;
	std::cin >> originalWord;
	std::string word = originalWord;

	if (!makeWordLowercase(word))
	{
		std::cout << "Invalid characters!\n";
		return;
	}

	if (findWord(word))
	{
		std::cout << "The word is correct!\n";
		return;
	}
	else
	{
		std::set<std::string> similarWords;

		// creating similar words to our input 
		std::string temp;
		size_t wordLength = word.length();
		for (size_t i = 0; i < wordLength; ++i)
		{
			// via changing one letter
			temp = word;
			for (char j = 'a'; j < 'z'; ++j)
			{
				temp[i] = j;
				//similarWords.push_back(temp);
				similarWords.insert(temp);
			}

			// via adding one letter except at the end
			temp.clear();
			temp = word.substr(0, i);
			temp.push_back('-');
			temp.append(word.substr(i, wordLength - i));
			for (char j = 'a'; j < 'z'; ++j)
			{
				temp[i] = j;
				//similarWords.push_back(temp); 
				similarWords.insert(temp);
			}

			// via removing one letter
			temp.clear();
			temp = word.substr(0, i);
			temp.append(word.substr(i + 1, wordLength - i - 1)); //might make trouble with i == last
			//similarWords.push_back(temp);
			similarWords.insert(temp);
		}

		// via adding one letter at the end
		temp = word;
		temp.push_back('0');
		for (char j = 'a'; j < 'z'; ++j)
		{
			temp[wordLength] = j;
			similarWords.insert(temp);
			//similarWords.push_back(temp);
		}

		std::cout << "Similar words:\n";
		bool thereAreSimilarWords = false;
		for (auto iter = similarWords.begin(); iter != similarWords.end(); ++iter)
		{
			if (findWord((*iter)))
			{
				std::cout << (*iter) << '\n';
				thereAreSimilarWords = true;
			}
		}
		if (!thereAreSimilarWords)
			std::cout << "none\n";
	}
}

// Displays files but CANNOT EDIT THEM.
void displayFile(std::string& fileName)
{
	std::string getcontent;
	std::fstream openfile(fileName, std::fstream::in);
	if (openfile.is_open())
	{
		while (!openfile.eof())
		{
			std::getline(openfile, getcontent);
			std::cout << getcontent << '\n';
		}
	}
	else
	{
		std::cout << "Error occured!\n";
	}
}

// A big part of this code is copied from above since I cannot have reused the functions there because they needed substantial changes.
std::string autocorrectOneWordForFile(std::string originalWord)
{
	std::string word = originalWord;

	// Each checked word is allowed to have a sign in the front and a sign in the back.
	// If more signs, that can be used appropriately, are pointed out, they can be added below easily.
	char signInFront = 0, signAtBack = 0;
	if (word[0] == '(' || word[0] == '\"' || word[0] == '\'')
	{
		signInFront = word[0];
		word = word.substr(1, word.size() - 1);
	}
	if (word[word.size() - 1] == ')' || word[word.size() - 1] == ';' || word[word.size() - 1] == '\"' || word[word.size() - 1] == '\'' || word[word.size() - 1] == ',' || word[word.size() - 1] == '.' || word[word.size() - 1] == '!' || word[word.size() - 1] == '?')
	{
		signAtBack = word[word.size() - 1];
		word = word.substr(0, word.size() - 1);
	}

	if (!makeWordLowercase(word))
	{
		std::cerr << "Invalid characters!\n";
		return originalWord;
	}

	if (findWord(word))
	{
		std::cout << "The word \"" << word << "\" is correct!\n";
		return originalWord;
	}
	else
	{
		// This set of words will contain all the variations of the lastWord input in hopes of finding a match in the dictionary for at least one of them.
		std::set<std::string> similarWords; 
		// I decided to use a set so that I don't have repeating words.

		// creating similar words to our input 
		std::string temp;
		size_t wordLength = word.length();
		for (size_t i = 0; i < wordLength; ++i)
		{
			// via changing one letter
			temp = word;
			for (char j = 'a'; j < 'z'; ++j)
			{
				temp[i] = j;
				//similarWords.push_back(temp);
				similarWords.insert(temp);
			}

			// via adding one letter except at the end
			temp.clear();
			temp = word.substr(0, i);
			temp.push_back('-');
			temp.append(word.substr(i, wordLength - i));
			for (char j = 'a'; j < 'z'; ++j)
			{
				temp[i] = j;
				//similarWords.push_back(temp); 
				similarWords.insert(temp);
			}

			// via removing one letter
			temp.clear();
			temp = word.substr(0, i);
			temp.append(word.substr(i + 1, wordLength - i - 1)); //might make trouble with i == last
			//similarWords.push_back(temp);
			similarWords.insert(temp);
		}

		// via adding one letter at the end
		temp = word;
		temp.push_back('0');
		for (char j = 'a'; j < 'z'; ++j)
		{
			temp[wordLength] = j;
			similarWords.insert(temp);
			//similarWords.push_back(temp);
		}

		// The user is presented with similar words that exist in the dictionary.
		std::cout << "Similar words:\n";
		bool thereAreSimilarWords = false;
		for (auto iter = similarWords.begin(); iter != similarWords.end(); ++iter)
		{
			if (findWord((*iter)))
			{
				std::cout << (*iter) << '\n';
				thereAreSimilarWords = true;
			}
		}
		if (!thereAreSimilarWords)
		{
			std::cout << "none\n";
			return originalWord;
		}
		else
		{
			// Asks the user to input his final choice of word. 
			// This word does not have to be from the list and this is a feature, not a bug.
			std::cout << "\nPlease enter your choice of word:\n";
			std::string choice;
			std::cin >> choice;
			if (choice == word) return originalWord;
			else
			{
				// Returns these signs to the string.
				if (signInFront) choice = choice.insert(0, std::string(1, signInFront));
				if (signAtBack) choice.push_back(signAtBack);
			
				if (choice.size() == originalWord.size())
				{
					// Return capital letters appropriately if the word had the same lenght.
					for (size_t i = 0; i < originalWord.size(); ++i)
					{
						if (originalWord[i] >= 'A' && originalWord[i] <= 'Z') choice[i] -= ('a' - 'A');
					}
				}
				else
				{
					short capitalLetters = 0;
					for (size_t i = 0; i < originalWord.size(); ++i)
					{
						if (originalWord[i] >= 'A' && originalWord[i] <= 'Z') ++capitalLetters;
					}
					if (capitalLetters == word.size())
					{
						// Returns ALL-CAPS
						for (size_t i = 0; i < choice.size(); ++i)
						{
							if (choice[i] >= 'a' && choice[i] <= 'z') choice[i] -= ('a' - 'A');
						}
					}
					else
					{
						// Returns first capital letter
						if (signInFront == 0 && choice[0] >= 'A' && choice[0] <= 'Z') choice[0] -= ('a' - 'A');
						else if (signInFront != 0 && choice[1] >= 'A' && choice[1] <= 'Z') choice[1] -= ('a' - 'A');
					}
				}
				return choice;
			}
		}
			
	}
}

void autocorrectInFile(std::string& filename)
{
	std::ofstream myFile(filename/*, std::fstream::out | std::ios::app*/);
	if (myFile.is_open())
	{
		std::cout << "Type in text that will be saved to a file and the last word before you click enter will be checked.\n";
		std::cout << "Write --close after finishing writing when you want to close the file and go back to the program.\n";

		std::string input;
		while (true)
		{
			std::getline(std::cin, input);
			while (input.empty())
				std::getline(std::cin, input);
			std::string lastWord;
			size_t lastWordLength = 0;



			int inputCounter = input.size() - 1;
			while (inputCounter >= 0 && input[inputCounter] != ' ')
			{
				--inputCounter;
			}
			while (++inputCounter < input.size())
			{
				lastWord.push_back(input[inputCounter]);
			}
			while (!input.empty() && input[input.size() - 1] != ' ') input.pop_back();
			myFile << input;
			if (lastWord == "--close")
			{
				myFile.close();
				return;
			}
			lastWord = autocorrectOneWordForFile(lastWord);

			myFile << lastWord;
		}
	}
	else
	{
		std::cout << "Error occured!\n";
	}
}

bool modes(bool detectedTwoDashes)
{
	if (!detectedTwoDashes)
	{
		std::cout << "\nEnter the preferred mode:\n";
		std::cout << "\t--autocorrect -word \t\tautocorrect only one word\n";
		//std::cout << "\t--autocorrect -word -experimental \tautocorrect only one word with a very dumb algorithm\n";
		std::cout << "\t--autocorrect -o \"text.txt\"\tautocorrect many words and type them in a document with said name\n";
		std::cout << "\t--autocomplete -word \t\tautocomplete only one word\n";
		//std::cout << "\t--autocomplete -o \"text.txt\"\tautocomplete many words and type them in a document with said name\n";
		std::cout << "\t--display -i \"text.txt\"\t\tvisualizes file\n";
		std::cout << "\t--help \t\t\t\treturns to this menu\n";
		std::cout << "\t--exit \t\t\t\texits the program\n\n";
	}

	std::string input;
	while (input.empty())
	{
		std::getline(std::cin, input);
	}

	if (!detectedTwoDashes)
	{
		while (input[0] == ' ')
			input.erase(0, 1);
		if (input[0] == '-' && input[1] == '-')
			input.erase(0, 2);
		else
			return modes(false);
	}

	if (input.find("autocorrect", 0) != std::string::npos)
	{
		while (input[0] == ' ')
			input.erase(0, 1);
		if (input.find("-word", 0) != std::string::npos)
		{
			input.erase(0, 5);
			if (input.find(" -experimental", 0) != std::string::npos)
			{
				autocorrectOneWordNoAlg();
			}
			else
			{
				autocorrectOneWordAlg();
			}
		}
		else if (input.find("-o", 0) != std::string::npos)
		{
			input.erase(0, 14);
			while (input[0] == ' ')
				input.erase(0, 1);
			std::string filename;
			if (input[0] == '\"')
			{
				size_t i = 1;
				while (input[i] != '\"')
				{
					filename.push_back(input[i++]);
				}
				//isplayFile(filename);
				autocorrectInFile(filename);
			}
			else
			{
				std::cout << "Incorrect input!\n";
			}
		}
			
	}
	else if (input.find("autocomplete", 0) != std::string::npos)
	{
		while (input[0] == ' ')
			input.erase(0, 1);
		if (input.find("-word", 0) != std::string::npos)
		{
			//input.erase(0, 5);
			autocompleteOneWord();
		}
		else if (input.find("-o", 0) != std::string::npos)
		{
			input.erase(0, 2);

			std::cout << "This feature is not avalible yet!\n";

			// get name of output file
		}
	}
	else if(input.find("display -i ", 0) != std::string::npos)
	{
		input.erase(0, 11);
		while (input[0] == ' ')
			input.erase(0, 1);
		std::string filename;
		if (input[0] == '\"')
		{
			size_t i = 1;
			while (input[i] != '\"')
			{
				filename.push_back(input[i++]);
			}
			displayFile(filename);
		}
		else
		{
			std::cout << "Incorrect input!\n";
		}
	}
	else if (input.find("exit", 0) != std::string::npos)
	{
		return false;
	}
	else // including --help
	{
		return modes(false);
	}

	return true;
}

int main()
{
	//std::cin.tie(NULL);
	//std::ios_base::sync_with_stdio(false);

	std::cout << "Welcome!\n\n";

	// Enter input file
	std::cerr << "Loading dictionary...\n\n";
	//std::ifstream myFile ("../short_dictionary.json");
	std::ifstream myFile("../dictionary.json");
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
		//std::cerr << '"' << word << "\" added!\n";
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