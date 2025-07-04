#pragma once
#include "Tools/FileSystem/PathMountResolver.h"
#include <string>


struct ResolvedPath
{
	std::string alias;

	std::string fileName;


	inline operator std::string() const
	{
		return PathMountResolver::Resolve(*this).string();
	}

	operator std::string_view() const
	{
		static thread_local std::string cached;

		cached = PathMountResolver::Resolve(*this).string();

		return cached;
	}
};