#pragma once

#include <map>

struct trie
{
	bool isWord;

	std::map<char, trie*> children;
	// Using std::map to save space.

	trie()
	{
		isWord = false;
		// If I were using an array, here I'd have to set all to nullptr.
	}
};

trie* root;