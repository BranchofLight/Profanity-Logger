#include <iostream>
#include <stdio.h>
#include <string>

int subStrCount(std::string& searchStr, std::string& libStr);

int main()
{
	const int wordsSize = 2;
	std::string words[wordsSize] = {"fuck", "shit"};

	char c;

	int buffSize = 8000; // Same as BUFFLEN in profanity-logger.h
	char buffer[buffSize]; 
	int buffIndex = 0;
	bool isReal = false;

	FILE* file;
	file = fopen("/dev/Profanity Logger", "r");

	if (file)
	{
		isReal = true;
		while ((c = getc(file)) != EOF)
		{
			buffer[buffIndex++] = c;
		}

		fclose(file);
	}
	else
		printf("File could not be opened.\n");

	if (isReal)
	{
		std::string buffStr(buffer);
		int count = 0;
		for (int i = 0; i < wordsSize; ++i)
		{
			count = subStrCount(words[i], buffStr);
			std::cout << words[i] << " has been said " << count;
			if (count == 1)
				std::cout << " time.\n";
			else
				std::cout << " times.\n";
		}
	}
	
	return 0;
}

int subStrCount(std::string& searchStr, std::string& libStr)
{
	const int block = searchStr.size();
	int count = 0;
	int pos   = 0;
	
	// Set pos equal to the location of the find before incrementing by block
	while ( (pos = libStr.find(searchStr, pos)) != std::string::npos)
	{
		++count;
		pos += block;
	}

	return count;
}
		
