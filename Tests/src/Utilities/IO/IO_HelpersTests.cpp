#include "Helpers/TempDirectory.h"
#include "Utilities/IO/IO_Helpers.h"
#include <algorithm>
#include <doctest.h>


using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("IO_Helpers");


TEST_CASE("GetFilesInFolder returns only the regular files, ignoring subdirectories")
{
	TempDirectory dir;
	dir.WriteFile("a.txt", "a");
	dir.WriteFile("b.txt", "b");
	std::filesystem::create_directory(dir.Path() / "subdir");

	auto files = IO_Helpers::GetFilesInFolder(dir.Path().string());

	CHECK(files.size() == 2);

	auto hasSuffix = [&](const std::string& suffix)
	{
		return std::any_of(files.begin(), files.end(), [&](const std::string& f) { return f.find(suffix) != std::string::npos; });
	};

	CHECK(hasSuffix("a.txt"));
	CHECK(hasSuffix("b.txt"));
}

TEST_CASE("GetFilesInFolder returns an empty vector for an empty folder")
{
	TempDirectory dir;

	auto files = IO_Helpers::GetFilesInFolder(dir.Path().string());

	CHECK(files.empty());
}

TEST_CASE("GetFilesInFolder throws std::filesystem::filesystem_error for a missing folder")
{
	TempDirectory dir;
	auto missing = (dir.Path() / "does_not_exist").string();

	CHECK_THROWS_AS(IO_Helpers::GetFilesInFolder(missing), std::filesystem::filesystem_error);
}


TEST_CASE("LoadTextFile reads the full contents of a multi-line file")
{
	TempDirectory dir;
	auto filePath = dir.WriteFile("text.txt", "line one\nline two").string();

	auto content = IO_Helpers::LoadTextFile(filePath);

	CHECK(content == "line one\nline two");
}

TEST_CASE("LoadTextFile throws std::runtime_error for a missing file")
{
	TempDirectory dir;
	auto missing = (dir.Path() / "does_not_exist.txt").string();

	CHECK_THROWS_AS(IO_Helpers::LoadTextFile(missing), std::runtime_error);
}


TEST_CASE("LoadTextFileLines splits on newlines")
{
	TempDirectory dir;
	auto filePath = dir.WriteFile("lines.txt", "one\ntwo\nthree").string();

	auto lines = IO_Helpers::LoadTextFileLines(filePath);

	REQUIRE(lines.size() == 3);
	CHECK(lines[0] == "one");
	CHECK(lines[1] == "two");
	CHECK(lines[2] == "three");
}

TEST_CASE("LoadTextFileLines handles a trailing newline the same as none")
{
	TempDirectory dir;
	auto withTrailing = dir.WriteFile("trailing.txt", "one\ntwo\n").string();
	auto withoutTrailing = dir.WriteFile("no_trailing.txt", "one\ntwo").string();

	auto trailingLines = IO_Helpers::LoadTextFileLines(withTrailing);
	auto noTrailingLines = IO_Helpers::LoadTextFileLines(withoutTrailing);

	CHECK(trailingLines == noTrailingLines);
}

TEST_CASE("LoadTextFileLines returns an empty vector for an empty file")
{
	TempDirectory dir;
	auto filePath = dir.WriteFile("empty.txt", "").string();

	auto lines = IO_Helpers::LoadTextFileLines(filePath);

	CHECK(lines.empty());
}

TEST_CASE("LoadTextFileLines throws std::runtime_error for a missing file")
{
	TempDirectory dir;
	auto missing = (dir.Path() / "does_not_exist.txt").string();

	CHECK_THROWS_AS(IO_Helpers::LoadTextFileLines(missing), std::runtime_error);
}


TEST_SUITE_END();