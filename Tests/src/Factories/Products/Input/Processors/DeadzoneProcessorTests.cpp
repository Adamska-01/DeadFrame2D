#include "Factories/Products/Input/Processors/DeadzoneProcessor.h"
#include <cmath>
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Factories;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("DeadzoneProcessor");


TEST_CASE("GetType returns DEADZONE")
{
	auto processor = DeadzoneProcessor();

	CHECK(processor.GetType() == ProcessorType::DEADZONE);
}


TEST_CASE("Process zeroes a float inside the threshold")
{
	auto processor = DeadzoneProcessor(0.2f);
	auto value = 0.1f;

	processor.Process(value);

	CHECK(value == doctest::Approx(0.0f));
}


TEST_CASE("Process rescales a positive float outside the threshold")
{
	auto processor = DeadzoneProcessor(0.2f);
	auto value = 0.6f;

	processor.Process(value);

	CHECK(value == doctest::Approx((0.6f - 0.2f) / (1.0f - 0.2f)));
}


TEST_CASE("Process rescales a negative float outside the threshold")
{
	auto processor = DeadzoneProcessor(0.2f);
	auto value = -0.6f;

	processor.Process(value);

	CHECK(value == doctest::Approx((-0.6f + 0.2f) / (1.0f - 0.2f)));
}


TEST_CASE("Process is continuous at the threshold boundary")
{
	auto processor = DeadzoneProcessor(0.2f);
	auto value = 0.2f;

	processor.Process(value);

	CHECK(value == doctest::Approx(0.0f));
}


TEST_CASE("Process zeroes each vector component independently inside the threshold")
{
	auto processor = DeadzoneProcessor(0.2f);
	auto value = Vector2F(0.1f, 0.6f);

	processor.Process(value);

	CHECK(value.x == doctest::Approx(0.0f));
	CHECK(value.y == doctest::Approx((0.6f - 0.2f) / (1.0f - 0.2f)));
}


TEST_CASE("Constructor clamps a threshold of 1.0 to avoid division by zero")
{
	auto processor = DeadzoneProcessor(1.0f);
	auto value = 0.9999f;

	CHECK(processor.GetThreshold() < 1.0f);
	CHECK_FALSE(std::isinf(value));

	processor.Process(value);

	CHECK_FALSE(std::isinf(value));
	CHECK_FALSE(std::isnan(value));
}


TEST_CASE("SetThreshold clamps a threshold of 1.0 to avoid division by zero")
{
	auto processor = DeadzoneProcessor(0.2f);

	processor.SetThreshold(1.0f);

	CHECK(processor.GetThreshold() < 1.0f);
}


TEST_CASE("SetThreshold clamps a negative threshold to zero")
{
	auto processor = DeadzoneProcessor(0.2f);

	processor.SetThreshold(-0.5f);

	CHECK(processor.GetThreshold() == doctest::Approx(0.0f));
}


TEST_CASE("GetThreshold reflects the constructor argument when in range")
{
	auto processor = DeadzoneProcessor(0.35f);

	CHECK(processor.GetThreshold() == doctest::Approx(0.35f));
}


TEST_SUITE_END();