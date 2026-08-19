#include "Helpers/TempDirectory.h"
#include "Utilities/IO/FileSystem/PathMountResolver.h"
#include "Utilities/IO/FileSystem/ResolvedPath.h"
#include "Utilities/IO/FileSystem/ResourceMount.h"
#include <doctest.h>


using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("ResolvedPath");


TEST_CASE("operator std::string resolves to the real mounted path")
{
	TempDirectory dir;
	dir.WriteFile("file.txt", "data");
	ResourceMount("ResolvedPathTests.asString", dir.Path());

	auto path = ResolvedPath("ResolvedPathTests.asString", "file.txt");
	std::string resolved = path;

	CHECK(resolved == (dir.Path() / "file.txt").string());
}

TEST_CASE("operator std::string_view resolves to the real mounted path")
{
	TempDirectory dir;
	dir.WriteFile("file.txt", "data");
	ResourceMount("ResolvedPathTests.asStringView", dir.Path());

	auto path = ResolvedPath("ResolvedPathTests.asStringView", "file.txt");
	std::string_view resolved = path;

	CHECK(resolved == (dir.Path() / "file.txt").string());
}

TEST_CASE("Resolution is cached after first access, even if the mount table changes afterward")
{
	TempDirectory dirA;
	dirA.WriteFile("file.txt", "a");
	ResourceMount("ResolvedPathTests.caching", dirA.Path());

	auto path = ResolvedPath("ResolvedPathTests.caching", "file.txt");
	std::string firstResolved = path;

	CHECK(firstResolved == (dirA.Path() / "file.txt").string());

	// Mount overwrites by alias - re-mounting the same alias to a different directory
	// is enough to move where a *fresh* resolution would point, without needing to
	// clear anything first.
	TempDirectory dirB;
	dirB.WriteFile("file.txt", "b");
	ResourceMount("ResolvedPathTests.caching", dirB.Path());

	// `path` already has a cached resolution; StoreCache()'s `if (!cached.empty()) return;`
	// guard means it never re-resolves, even though the mount table now points elsewhere.
	// This pins that current (documented, not "fixed") caching behavior.
	std::string secondResolved = path;

	CHECK(secondResolved == firstResolved);
	CHECK(secondResolved != (dirB.Path() / "file.txt").string());
}

TEST_CASE("Constructing a ResolvedPath for an unknown alias does not throw; conversion does")
{
	CHECK_NOTHROW(ResolvedPath("ResolvedPathTests.neverMounted", "file.txt"));

	auto path = ResolvedPath("ResolvedPathTests.neverMounted", "file.txt");

	CHECK_THROWS_AS((void)static_cast<std::string>(path), std::runtime_error);
}


TEST_SUITE_END();