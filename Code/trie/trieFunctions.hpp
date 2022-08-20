#pragma once

#include <string>
#include "trie.hpp"

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