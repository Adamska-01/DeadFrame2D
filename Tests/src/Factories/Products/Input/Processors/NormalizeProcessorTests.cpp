#include "Factories/Products/Input/Processors/NormalizeProcessor.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Factories;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("NormalizeProcessor");


TEST_CASE("GetType returns NORMALIZE")
{
	auto processor = NormalizeProcessor();

	CHECK(processor.GetType() == ProcessorType::NORMALIZE);
}


TEST_CASE("Process leaves a float value unchanged")
{
	auto processor = NormalizeProcessor();
	auto value = 3.5f;

	processor.Process(value);

	CHECK(value == doctest::Approx(3.5f));
}


TEST_CASE("Process reduces a vector to unit length, preserving direction")
{
	auto processor = NormalizeProcessor();
	auto value = Vector2F(3.0f, 4.0f);

	processor.Process(value);

	CHECK(value.Magnitude() == doctest::Approx(1.0f));
	CHECK(value.x == doctest::Approx(0.6f));
	CHECK(value.y == doctest::Approx(0.8f));
}


TEST_CASE("Process leaves the zero vector unchanged instead of dividing by zero")
{
	auto processor = NormalizeProcessor();
	auto value = Vector2F::Zero;

	processor.Process(value);

	CHECK(value == Vector2F::Zero);
}


TEST_SUITE_END();