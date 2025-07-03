#include "Tools/FileSystem/PathMountResolver.h"
#include "Tools/FileSystem/ResourceMount.h"


void PathMountResolver::Mount(const ResourceMount& resourceMount)
{
	mounts[resourceMount.alias] = resourceMount.path;
}

std::filesystem::path PathMountResolver::Resolve(const std::string& alias, const std::string& fileName)
{
	auto it = mounts.find(alias);

	if (it == mounts.end())
	{
		throw std::runtime_error("Unknown mount alias: " + alias);
	}

	// Try with prefix first
	auto resolvedPathWithPrefix = prefix / it->second / fileName;
	
	if (std::filesystem::exists(resolvedPathWithPrefix))
		return resolvedPathWithPrefix;

	// Try without prefix
	auto resolvedPathWithoutPrefix = it->second / fileName;
	
	if (std::filesystem::exists(resolvedPathWithoutPrefix))
		return resolvedPathWithoutPrefix;

	// Neither path exists
	throw std::runtime_error("File not found with or without prefix: " + resolvedPathWithPrefix.string() + " / " + resolvedPathWithoutPrefix.string());
}

void PathMountResolver::SetMountPrefix(std::string mountPrefix)
{
	prefix = std::filesystem::path(mountPrefix);
}