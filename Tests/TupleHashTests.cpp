#include <doctest.h>
#include <tuple>
#include <Utilities/Hashing/TupleHash.h>


TEST_CASE("TupleHash - hash_combine produces different values")
{
	std::size_t seed = 0;
	DF2D::Utilities::hash_combine(seed, 42);
	DF2D::Utilities::hash_combine(seed, 123);

	REQUIRE(seed != 0);
}

TEST_CASE("TupleHash - same tuple produces same hash")
{
	auto tuple1 = std::make_tuple(1, 2, 3);
	auto tuple2 = std::make_tuple(1, 2, 3);

	DF2D::Utilities::TupleHash hasher;

	CHECK(hasher(tuple1) == hasher(tuple2));
}

TEST_CASE("TupleHash - different tuples produce different hashes")
{
	auto tuple1 = std::make_tuple(1, 2, 3);
	auto tuple2 = std::make_tuple(1, 2, 4);

	DF2D::Utilities::TupleHash hasher;

	CHECK_FALSE(hasher(tuple1) == hasher(tuple2));
}