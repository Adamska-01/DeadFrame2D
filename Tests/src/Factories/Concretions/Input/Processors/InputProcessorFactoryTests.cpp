#include "Factories/Concretions/Input/Processors/InputProcessorFactory.h"
#include "Factories/Products/Input/Processors/ClampProcessor.h"
#include "Factories/Products/Input/Processors/DeadzoneProcessor.h"
#include "Factories/Products/Input/Processors/InvertProcessor.h"
#include "Factories/Products/Input/Processors/NormalizeProcessor.h"
#include "Factories/Products/Input/Processors/ScaleProcessor.h"
#include <doctest.h>


using namespace DF2D::Factories;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("InputProcessorFactory");


TEST_CASE("Builds a DeadzoneProcessor from matching settings")
{
	auto model = InputProcessor
	{
		.kind = ProcessorType::DEADZONE,
		.settings = DeadzoneProcessorSettings{ .threshold = 0.3f }
	};

	auto processor = InputProcessorFactory().CreateProduct(model);

	REQUIRE(processor != nullptr);
	CHECK(processor->GetType() == ProcessorType::DEADZONE);
	CHECK(dynamic_cast<DeadzoneProcessor*>(processor.get())->GetThreshold() == doctest::Approx(0.3f));
}


TEST_CASE("Builds a ScaleProcessor from matching settings")
{
	auto model = InputProcessor
	{
		.kind = ProcessorType::SCALE,
		.settings = ScaleProcessorSettings{ .factor = 4.0f }
	};

	auto processor = InputProcessorFactory().CreateProduct(model);

	REQUIRE(processor != nullptr);
	CHECK(processor->GetType() == ProcessorType::SCALE);
	CHECK(dynamic_cast<ScaleProcessor*>(processor.get())->GetFactor() == doctest::Approx(4.0f));
}


TEST_CASE("Builds a ClampProcessor from matching settings")
{
	auto model = InputProcessor
	{
		.kind = ProcessorType::CLAMP,
		.settings = ClampProcessorSettings{ .min = -2.0f, .max = 2.0f }
	};

	auto processor = InputProcessorFactory().CreateProduct(model);

	REQUIRE(processor != nullptr);
	CHECK(processor->GetType() == ProcessorType::CLAMP);

	auto* clamp = dynamic_cast<ClampProcessor*>(processor.get());

	CHECK(clamp->GetMin() == doctest::Approx(-2.0f));
	CHECK(clamp->GetMax() == doctest::Approx(2.0f));
}


TEST_CASE("Builds an InvertProcessor")
{
	auto model = InputProcessor
	{
		.kind = ProcessorType::INVERT,
		.settings = InvertProcessorSettings{}
	};

	auto processor = InputProcessorFactory().CreateProduct(model);

	REQUIRE(processor != nullptr);
	CHECK(processor->GetType() == ProcessorType::INVERT);
	CHECK(dynamic_cast<InvertProcessor*>(processor.get()) != nullptr);
}


TEST_CASE("Builds a NormalizeProcessor")
{
	auto model = InputProcessor
	{
		.kind = ProcessorType::NORMALIZE,
		.settings = NormalizeProcessorSettings{}
	};

	auto processor = InputProcessorFactory().CreateProduct(model);

	REQUIRE(processor != nullptr);
	CHECK(processor->GetType() == ProcessorType::NORMALIZE);
	CHECK(dynamic_cast<NormalizeProcessor*>(processor.get()) != nullptr);
}


TEST_CASE("Falls back to default settings when the variant holds a mismatched alternative")
{
	// The variant's default alternative is ClampProcessorSettings, so a
	// DEADZONE model with settings left untouched exercises the factory's
	// std::holds_alternative fallback branch.
	auto model = InputProcessor{ .kind = ProcessorType::DEADZONE };

	auto processor = InputProcessorFactory().CreateProduct(model);

	REQUIRE(processor != nullptr);

	auto* deadzone = dynamic_cast<DeadzoneProcessor*>(processor.get());

	REQUIRE(deadzone != nullptr);
	CHECK(deadzone->GetThreshold() == doctest::Approx(0.15f));
}


TEST_SUITE_END();