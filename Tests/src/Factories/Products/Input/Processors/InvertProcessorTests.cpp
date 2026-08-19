#include "Factories/Products/Input/Processors/InvertProcessor.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Factories;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("InvertProcessor");


TEST_CASE("GetType returns INVERT")
{
	auto processor = InvertProcessor();

	CHECK(processor.GetType() == ProcessorType::INVERT);
}


TEST_CASE("Process negates a float value")
{
	auto processor = InvertProcessor();
	auto value = 3.5f;

	processor.Process(value);

	CHECK(value == doctest::Approx(-3.5f));
}


TEST_CASE("Process negates both vector components")
{
	auto processor = InvertProcessor();
	auto value = Vector2F(3.0f, -4.0f);

	processor.Process(value);

	CHECK(value == Vector2F(-3.0f, 4.0f));
}


TEST_CASE("Process is its own inverse")
{
	auto processor = InvertProcessor();
	auto value = Vector2F(3.0f, -4.0f);

	processor.Process(value);
	processor.Process(value);

	CHECK(value == Vector2F(3.0f, -4.0f));
}


TEST_SUITE_END();