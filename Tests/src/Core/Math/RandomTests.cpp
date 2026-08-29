#include "Core/Math/Random.h"
#include <doctest.h>
#include <vector>


using namespace DF2D::Core;


TEST_SUITE_BEGIN("Random");


TEST_CASE("Random constructed with the same seed produces an identical sequence")
{
	auto first = Random(12345u);
	auto second = Random(12345u);

	for (auto i = 0; i < 32; ++i)
		CHECK(first.NextUInt() == second.NextUInt());
}


TEST_CASE("Random constructed with different seeds produces different sequences")
{
	auto first = Random(1u);
	auto second = Random(2u);

	auto differences = 0;

	for (auto i = 0; i < 32; ++i)
	{
		if (first.NextUInt() != second.NextUInt())
			++differences;
	}

	CHECK(differences > 0);
}


TEST_CASE("Random constructed with seed zero still produces a varying sequence")
{
	auto random = Random(0u);

	const auto first = random.NextUInt();
	const auto second = random.NextUInt();

	CHECK(first != 0u);
	CHECK(first != second);
}


TEST_CASE("Value returns results within the half-open range zero to one")
{
	auto random = Random(777u);

	for (auto i = 0; i < 4096; ++i)
	{
		const auto value = random.Value();

		CHECK(value >= 0.0f);
		CHECK(value < 1.0f);
	}
}


TEST_CASE("Range with float bounds never returns a value outside those bounds")
{
	auto random = Random(31337u);

	for (auto i = 0; i < 4096; ++i)
	{
		const auto value = random.Range(-5.0f, 12.5f);

		CHECK(value >= -5.0f);
		CHECK(value <= 12.5f);
	}
}


TEST_CASE("Range returns the bound itself when the minimum equals the maximum")
{
	auto random = Random(9u);

	CHECK(random.Range(4.0f, 4.0f) == doctest::Approx(4.0f));
	CHECK(random.Range(7, 7) == 7);
}


TEST_CASE("Range with integer bounds eventually returns both endpoints")
{
	auto random = Random(4242u);

	auto sawMin = false;
	auto sawMax = false;

	for (auto i = 0; i < 4096; ++i)
	{
		const auto value = random.Range(0, 3);

		CHECK(value >= 0);
		CHECK(value <= 3);

		sawMin = sawMin || value == 0;
		sawMax = sawMax || value == 3;
	}

	CHECK(sawMin);
	CHECK(sawMax);
}


TEST_CASE("Range with inverted bounds does not throw and stays between them")
{
	auto random = Random(55u);

	for (auto i = 0; i < 512; ++i)
	{
		const auto value = random.Range(10.0f, -10.0f);

		CHECK(value >= -10.0f);
		CHECK(value <= 10.0f);
	}

	CHECK(random.Range(9, 2) >= 2);
}


TEST_CASE("InsideUnitCircle returns points whose magnitude never exceeds one")
{
	auto random = Random(2024u);

	for (auto i = 0; i < 2048; ++i)
	{
		const auto point = random.InsideUnitCircle();

		CHECK(point.Magnitude() <= 1.0f + 0.0001f);
	}
}


TEST_CASE("SetSeed restarts the sequence deterministically")
{
	auto random = Random(1u);

	std::vector<uint32_t> firstRun;

	for (auto i = 0; i < 8; ++i)
		firstRun.push_back(random.NextUInt());

	random.SetSeed(1u);

	for (auto i = 0; i < 8; ++i)
		CHECK(random.NextUInt() == firstRun[i]);
}


TEST_SUITE_END();
