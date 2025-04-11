#pragma once
#include <filesystem>
#include <string>
#include <vector>


/// <summary>
/// Utility functions related to filesystem operations.
/// </summary>
namespace FileSystemUtils
{
	/// <summary>
	/// Retrieves all regular file paths from a given folder.
	/// </summary>
	/// <param name="folderPath">Path to the folder to scan.</param>
	/// <returns>A vector containing the full paths of all regular files in the folder.</returns>
	std::vector<std::string> GetFilesInFolder(std::string_view folderPath)
	{
		std::vector<std::string> filePaths;

		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(folderPath))) 
		{
			if (!entry.is_regular_file())
				continue;

			filePaths.push_back(entry.path().string());
		}

		return filePaths;
	}
}