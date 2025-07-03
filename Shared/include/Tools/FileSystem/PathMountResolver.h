#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>


class ResourceMount;


class PathMountResolver
{
private:
	PathMountResolver() = delete;

	~PathMountResolver() = delete;

	PathMountResolver(const PathMountResolver&) = delete;

	PathMountResolver(PathMountResolver&&) = delete;

	PathMountResolver& operator=(const PathMountResolver&) = delete;

	PathMountResolver& operator=(PathMountResolver&&) = delete;


	inline static std::filesystem::path prefix = "";
	
	inline static std::unordered_map<std::string, std::filesystem::path> mounts = {};


public:
	static void Mount(const ResourceMount& resourceMount);

	static std::filesystem::path Resolve(const std::string& alias, const std::string& fileName);


	static void SetMountPrefix(std::string mountPrefix);
};