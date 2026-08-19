#include "Helpers/TempDirectory.h"
#include "Utilities/IO/FileSystem/PathMountResolver.h"
#include "Utilities/IO/FileSystem/ResolvedPath.h"
#include "Utilities/IO/FileSystem/ResourceMount.h"
#include <doctest.h>
#include <fstream>


using namespace DF2D::Utilities;


namespace
{
	struct PrefixGuard
	{
		~PrefixGuard()
		{
			PathMountResolver::SetMountPrefix("");
		}
	};

	struct CwdGuard
	{
		std::filesystem::path previous = std::filesystem::current_path();

		~CwdGuard()
		{
			std::filesystem::current_path(previous);
		}
	};
}


TEST_SUITE_BEGIN("PathMountResolver");


TEST_CASE("Mount registers an alias and Resolve finds a real file under it")
{
	TempDirectory dir;
	dir.WriteFile("sprite.png", "data");

	PathMountResolver::Mount(ResourceMount("PathMountResolverTests.sprites", dir.Path()));

	auto resolved = PathMountResolver::Resolve(ResolvedPath("PathMountResolverTests.sprites", "sprite.png"));

	CHECK(resolved == dir.Path() / "sprite.png");
}

TEST_CASE("Resolve throws for an unknown alias")
{
	CHECK_THROWS_AS(PathMountResolver::Resolve(ResolvedPath("PathMountResolverTests.never-mounted", "file.txt")), std::runtime_error);
}

TEST_CASE("Resolve throws for a known alias but a missing file")
{
	TempDirectory dir;

	PathMountResolver::Mount(ResourceMount("PathMountResolverTests.missingFile", dir.Path()));

	CHECK_THROWS_AS(PathMountResolver::Resolve(ResolvedPath("PathMountResolverTests.missingFile", "missing.png")), std::runtime_error);
}

TEST_CASE("SetMountPrefix prefers the prefixed path when it exists")
{
	PrefixGuard prefixGuard;

	TempDirectory dir;
	std::filesystem::create_directories(dir.Path() / "mounted");
	std::ofstream(dir.Path() / "mounted" / "file.txt") << "prefixed";

	PathMountResolver::SetMountPrefix(dir.Path().string());
	PathMountResolver::Mount(ResourceMount("PathMountResolverTests.prefixed", "mounted"));

	auto resolved = PathMountResolver::Resolve(ResolvedPath("PathMountResolverTests.prefixed", "file.txt"));

	CHECK(resolved == dir.Path() / "mounted" / "file.txt");
}

TEST_CASE("SetMountPrefix falls back to the unprefixed path when the prefixed one does not exist")
{
	// A relative mount path is needed to make the prefixed and unprefixed candidates
	// distinct paths (an absolute mount path would make `prefix / absoluteMount` collapse
	// to just `absoluteMount`, per std::filesystem::path's append semantics). Resolving a
	// relative mount means `exists()` checks against the process cwd, so this test scopes
	// its own cwd change to the temp directory and restores it afterward.
	PrefixGuard prefixGuard;
	CwdGuard cwdGuard;

	TempDirectory dir;
	dir.WriteFile("file.txt", "unprefixed");

	std::filesystem::current_path(dir.Path());

	PathMountResolver::SetMountPrefix("does_not_exist_prefix");
	PathMountResolver::Mount(ResourceMount("PathMountResolverTests.fallback", "."));

	auto resolved = PathMountResolver::Resolve(ResolvedPath("PathMountResolverTests.fallback", "file.txt"));

	CHECK(std::filesystem::equivalent(resolved, dir.Path() / "file.txt"));
}


TEST_SUITE_END();
