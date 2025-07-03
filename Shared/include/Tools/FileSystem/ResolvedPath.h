#pragma once
#include "Tools/FileSystem/PathMountResolver.h"
#include <string>


struct ResolvedPath
{
	std::string alias;

	std::string fileName;


	inline operator std::string() const
	{
		return PathMountResolver::Resolve(alias, fileName).string();
	}

	operator std::string_view() const
	{
		static thread_local std::string cached;

		cached = PathMountResolver::Resolve(alias, fileName).string();

		return cached;
	}
};