#pragma once
#include <string>


struct ResourceMount
{
	std::string alias;

	std::filesystem::path path;


	ResourceMount(const std::string& a, const std::filesystem::path& p)
		: alias(a), path(p)
	{
		PathMountResolver::Mount(*this);
	}
};