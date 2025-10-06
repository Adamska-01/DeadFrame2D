#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>


/**
 * Utility functions related to filesystem operations.
 */
namespace DeadFrame2D::Utilities
{
	/**
	 * Retrieves all regular file paths from a given folder.
	 * 
	 * @param folderPath Path to the folder to scan.
	 * @return A vector containing the full paths of all regular files in the folder.
	 */
	inline std::vector<std::string> GetFilesInFolder(std::string_view folderPath)
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

	/**
	 * @brief Loads the entire contents of a text file into a single string.
	 *
	 * @param filePath The path to the text file.
	 * @return The contents of the file as a single string.
	 * @throws std::runtime_error If the file cannot be opened.
	 */
	inline std::string LoadTextFile(const std::string& filePath)
	{
		std::ifstream file(filePath);

		if (!file)
		{
			throw std::runtime_error("Failed to open file: " + filePath);
		}

		std::ostringstream ss;
		
		ss << file.rdbuf();

		return ss.str();
	}

	/**
	 * @brief Loads the contents of a text file and returns each line as a string in a vector.
	 *
	 * @param filePath The path to the text file.
	 * @return A vector containing one string per line in the file.
	 * @throws std::runtime_error If the file cannot be opened.
	 */
	inline std::vector<std::string> LoadTextFileLines(const std::string& filePath)
	{
		std::ifstream file(filePath);

		if (!file)
		{
			throw std::runtime_error("Failed to open file: " + filePath);
		}

		std::vector<std::string> lines;
		std::string line;

		while (std::getline(file, line))
		{
			lines.push_back(line);
		}

		return lines;
	}
}