#include "Helpers/TempDirectory.h"
#include "Utilities/IO/FileSystem/PathMountResolver.h"
#include "Utilities/IO/FileSystem/ResolvedPath.h"
#include "Utilities/IO/FileSystem/ResourceMount.h"
#include <doctest.h>


using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("ResourceMount");


TEST_CASE("Constructing a ResourceMount stores its alias and path")
{
	auto mount = ResourceMount("ResourceMountTests.stored", "some/path");

	CHECK(mount.alias == "ResourceMountTests.stored");
	CHECK(mount.path == std::filesystem::path("some/path"));
}

TEST_CASE("Constructing a ResourceMount registers it with PathMountResolver")
{
	// ResourceMount has no direct accessor into the mount table; registration is
	// observed indirectly through a subsequent Resolve() call - that's the seam available.
	// The alias is unique to this test case so it can't collide with any other test's
	// mount or with a real production alias (e.g. "shared_fonts") in the same process.
	TempDirectory dir;
	dir.WriteFile("file.txt", "data");

	auto mount = ResourceMount("ResourceMountTests.registered", dir.Path());

	auto resolved = PathMountResolver::Resolve(ResolvedPath("ResourceMountTests.registered", "file.txt"));

	CHECK(resolved == dir.Path() / "file.txt");
}


TEST_SUITE_END();