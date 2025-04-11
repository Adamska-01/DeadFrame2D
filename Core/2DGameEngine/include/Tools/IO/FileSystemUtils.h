#pragma once
#include <filesystem>
#include <string>
#include <vector>

/**
 * Utility functions related to filesystem operations.
 */
namespace FileSystemUtils
{
	/**
	 * Retrieves all regular file paths from a given folder.
	 * 
	 * @param folderPath Path to the folder to scan.
	 * @return A vector containing the full paths of all regular files in the folder.
	 */
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