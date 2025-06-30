#pragma once
#include <fstream>


static int tot = 0;
static bool loaded = false;

std::ofstream file;

const std::string MEMORY_LOG_PATH = "Logs/MemoryLog.txt";


inline void* operator new(size_t size, const char* filename, int line)
{
	if (!loaded)
	{
		file.open(MEMORY_LOG_PATH);

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
		file.open(MEMORY_LOG_PATH);

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