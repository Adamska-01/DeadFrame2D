#include <doctest.h>
#include <string>
#include <Utilities/Hashing/PairHash.h>
#include <utility>


TEST_CASE("PairHash - same pair produces same hash")
{
	auto pair1 = std::make_pair(1, 2);
	auto pair2 = std::make_pair(1, 2);

	DF2D::Utilities::PairHash hasher;

	CHECK(hasher(pair1) == hasher(pair2));
}

TEST_CASE("PairHash - different pairs produce different hashes")
{
	auto pair1 = std::make_pair(1, 2);
	auto pair2 = std::make_pair(1, 3);

	DF2D::Utilities::PairHash hasher;

	CHECK_FALSE(hasher(pair1) == hasher(pair2));
}

TEST_CASE("PairHash - works with different types")
{
	auto intPair = std::make_pair(1, 2);
	auto stringPair = std::make_pair(std::string("a"), std::string("b"));

	DF2D::Utilities::PairHash hasher;

	auto hashInt = hasher(intPair);
	auto hashString = hasher(stringPair);

	REQUIRE(hashInt != 0);
	REQUIRE(hashString != 0);
}