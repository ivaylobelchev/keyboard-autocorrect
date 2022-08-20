#pragma once

#include "../functions/functions.hpp"

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
			autocompleteOneWord();
		}
		else if (input.find("-o", 0) != std::string::npos)
		{
			input.erase(0, 2);

			std::cout << "This feature is not avalible yet!\n";
		}
	}
	else if (input.find("display -i ", 0) != std::string::npos)
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