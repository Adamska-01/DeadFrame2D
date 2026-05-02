#pragma once
#include "Utilities/IO/FileSystem/PathMountResolver.h"
#include <string>


namespace DF2D::Utilities
{
	struct ResolvedPath
	{
	private:
		mutable std::string cached = "";


		inline void StoreCache() const
		{
			if (!cached.empty())
				return;

			cached = PathMountResolver::Resolve(*this).string();
		}


	public:
		std::string alias;

		std::string fileName;


		ResolvedPath(const std::string& alias, const std::string& fileName)
			: alias(alias), fileName(fileName)
		{
		}


		inline operator std::string() const
		{
			StoreCache();

			return cached;
		}

		operator std::string_view() const
		{
			StoreCache();

			return cached;
		}
	};
}