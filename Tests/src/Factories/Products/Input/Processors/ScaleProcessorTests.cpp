#include "Factories/Products/Input/Processors/ScaleProcessor.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Factories;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("ScaleProcessor");


TEST_CASE("GetType returns SCALE")
{
	auto processor = ScaleProcessor();

	CHECK(processor.GetType() == ProcessorType::SCALE);
}


TEST_CASE("Default factor is identity")
{
	auto processor = ScaleProcessor();
	auto value = 3.0f;

	processor.Process(value);

	CHECK(value == doctest::Approx(3.0f));
}


TEST_CASE("Process scales a float by the factor")
{
	auto processor = ScaleProcessor(2.0f);
	auto value = 3.0f;

	processor.Process(value);

	CHECK(value == doctest::Approx(6.0f));
}


TEST_CASE("Process scales both components of a vector by the factor")
{
	auto processor = ScaleProcessor(0.5f);
	auto value = Vector2F(4.0f, -8.0f);

	processor.Process(value);

	CHECK(value == Vector2F(2.0f, -4.0f));
}


TEST_CASE("GetFactor and SetFactor round-trip")
{
	auto processor = ScaleProcessor();

	processor.SetFactor(5.0f);

	CHECK(processor.GetFactor() == doctest::Approx(5.0f));
}


TEST_SUITE_END();