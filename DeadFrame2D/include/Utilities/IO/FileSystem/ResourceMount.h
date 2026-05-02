#pragma once
#include <string>


namespace DF2D::Utilities
{
	struct ResourceMount
	{
		std::string alias;

		std::filesystem::path path;


		ResourceMount(const std::string& alias, const std::filesystem::path& path)
			: alias(alias), path(path)
		{
			PathMountResolver::Mount(*this);
		}
	};
}