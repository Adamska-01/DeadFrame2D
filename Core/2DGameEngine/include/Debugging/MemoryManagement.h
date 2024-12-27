#pragma once
#include <fstream>


static int tot = 0;
static bool loaded = false;

std::ofstream file;


inline void* operator new(size_t size, const char* filename, int line)
{
	if (!loaded)
	{
		file.open("Assets/MemoryLog.txt");

		loaded = true;
	}

	tot += size;

	void* ptr = new char[size];

	file << "size = " << size << " bytes " << "filename = " << filename << "line = " << line << std::endl;
	file << "total heap allocation = " << tot << " bytes " << std::endl;

	file.close();

	return ptr;
}

inline void* operator new[](size_t size, const char* filename, int line)
{
	if (!loaded)
	{
		file.open("Assets/MemoryLog.txt");

		loaded = true;
	}

	tot += size;
	
	void* ptr = new char[size];
	
	file << "size = " << size << " bytes " << "filename = " << filename << "line = " << line << std::endl;
	file << "total heap allocation = " << tot << " bytes " << std::endl;

	file.close();

	return ptr;
}

// Exploit the preprocessor
#define new new(__FILE__, __LINE__)