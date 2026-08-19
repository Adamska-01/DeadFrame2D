#include "Factories/Products/Input/Processors/ClampProcessor.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Factories;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("ClampProcessor");


TEST_CASE("GetType returns CLAMP")
{
	auto processor = ClampProcessor();

	CHECK(processor.GetType() == ProcessorType::CLAMP);
}


TEST_CASE("Process leaves an in-range float unchanged")
{
	auto processor = ClampProcessor(0.0f, 1.0f);
	auto value = 0.5f;

	processor.Process(value);

	CHECK(value == doctest::Approx(0.5f));
}


TEST_CASE("Process clamps a float above the max")
{
	auto processor = ClampProcessor(0.0f, 1.0f);
	auto value = 5.0f;

	processor.Process(value);

	CHECK(value == doctest::Approx(1.0f));
}


TEST_CASE("Process clamps a float below the min")
{
	auto processor = ClampProcessor(0.0f, 1.0f);
	auto value = -5.0f;

	processor.Process(value);

	CHECK(value == doctest::Approx(0.0f));
}


TEST_CASE("Process clamps each vector component independently")
{
	auto processor = ClampProcessor(-1.0f, 1.0f);
	auto value = Vector2F(5.0f, -5.0f);

	processor.Process(value);

	CHECK(value == Vector2F(1.0f, -1.0f));
}


TEST_CASE("GetMin/GetMax and SetMin/SetMax round-trip")
{
	auto processor = ClampProcessor();

	processor.SetMin(-2.0f);
	processor.SetMax(2.0f);

	CHECK(processor.GetMin() == doctest::Approx(-2.0f));
	CHECK(processor.GetMax() == doctest::Approx(2.0f));
}


TEST_SUITE_END();