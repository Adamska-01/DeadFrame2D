#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>	
#include <time.h>
#include <stdlib.h>


namespace UsefullFunctions
{
	static bool init;

	inline float Clamp(float value, float min, float max) 
	{
		if (value > max)
			return max;

		if (value < min)
			return min;

		return value;
	}

	inline void LoadMapFromFile(std::string FilePath, std::vector<std::string>& grid)
	{
		char tile;

		//x and y size of the file
		int sizeX = 0;
		int sizeY = 0;


		//create and open .txt file
		std::ifstream mapFile;
		mapFile.open(FilePath);

		//get rows
		std::string line;
		while (std::getline(mapFile, line))
			sizeY++;


		//get columns
		std::stringstream s(line);
		char value;
		//while there's something in the line, ignore ','  and increase the number of columns
		while (s >> value)	if (value != ',') sizeX++;


		grid.resize(sizeY);


		//start from beginning of the file 
		mapFile.clear();
		mapFile.seekg(0);


		//populate the array
		for (int y = 0; y < sizeY; y++)
			for (int x = 0; x < sizeX; x++)
			{
				mapFile.get(tile);

				grid[y] += tile;

				mapFile.ignore();
			}


		mapFile.close();
	}

	//returns the number of character in a row (if character is 0)
	inline int CountCharactersinARow(std::vector<std::string>& grid, int row, int column)
	{
		int counter = 1;
		for (int i = column + 1; i < grid[0].length(); i++)
		{
			if (grid[row][i] == '0')
				++counter;
			else
				break;
		}

		return counter;
	}

	//returns the number of character in a column (if character is 0)
	inline int CountCharactersinAColumn(std::vector<std::string>& grid, int row, int column)
	{
		auto counter = 1;

		for (auto i = row + 1; i < grid.size(); i++)
		{
			if (grid[i][column] == '0')
				++counter;
			else
				break;
		}

		return counter;
	}

	inline float RandomFloat(float min, float max) 
	{
		if (!init)
		{
			srand((unsigned int)time(NULL));
			init = true;
		}

		auto random = ((float)rand()) / (float)RAND_MAX;
		auto diff = max - min;
		auto r = random * diff;

		return min + r;
	}

	inline int RandomInt(int min, int max)
	{
		if (!init)
		{
			srand((unsigned int)time(NULL));
			init = true;
		}

		return rand() % (max - min + 1) + min;
	}
}