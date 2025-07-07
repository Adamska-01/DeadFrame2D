#pragma once
#include "Tools/FileSystem/PathMountResolver.h"
#include <string>


struct ResolvedPath
{
private:
	mutable std::string cached = "";


public:
	std::string alias;

	std::string fileName;


	ResolvedPath(const std::string& alias, const std::string& fileName)
		: alias(alias), fileName(fileName)
	{
	}


	inline operator std::string() const
	{
		if (!cached.empty())
			return cached;

		cached = PathMountResolver::Resolve(*this).string();
	
		return cached;
	}

	operator std::string_view() const
	{
		if (!cached.empty())
			return cached;

		cached = PathMountResolver::Resolve(*this).string();

		return cached;
	}
};