#pragma once
#include <filesystem>
#include <fstream>
#include <random>
#include <string>


// Creates a unique subdirectory under the system temp path on construction and removes
// it (recursively) on destruction.
class TempDirectory
{
private:
	std::filesystem::path path;


public:
	TempDirectory()
	{
		std::random_device rd;

		auto unique = "DF2D_Test_" + std::to_string(rd()) + "_" + std::to_string(rd());

		path = std::filesystem::temp_directory_path() / unique;

		std::filesystem::create_directories(path);
	}

	~TempDirectory()
	{
		std::error_code ec;

		std::filesystem::remove_all(path, ec);
	}

	TempDirectory(const TempDirectory&) = delete;

	TempDirectory& operator=(const TempDirectory&) = delete;


	const std::filesystem::path& Path() const
	{
		return path;
	}

	std::filesystem::path WriteFile(const std::string& name, const std::string& content) const
	{
		auto filePath = path / name;

		std::ofstream file(filePath);
		file << content;
		file.close();

		return filePath;
	}
};