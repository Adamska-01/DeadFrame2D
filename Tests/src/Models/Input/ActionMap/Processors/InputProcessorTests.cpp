#include <doctest.h>
#include "Models/Input/ActionMap/Processors/InputProcessor.h"
#include <nlohmann/json.hpp>


using namespace DF2D::Models;


TEST_SUITE_BEGIN("InputProcessor");


TEST_CASE("Deadzone processor round-trips through JSON")
{
	InputProcessor processor;
	processor.kind = ProcessorType::DEADZONE;
	processor.settings = DeadzoneProcessorSettings{ 0.25f };

	nlohmann::json j = processor;
	InputProcessor parsed = j.get<InputProcessor>();

	CHECK(parsed.kind == ProcessorType::DEADZONE);
	REQUIRE(std::holds_alternative<DeadzoneProcessorSettings>(parsed.settings));
	CHECK(std::get<DeadzoneProcessorSettings>(parsed.settings).threshold == doctest::Approx(0.25f));
}


TEST_CASE("Scale processor round-trips through JSON")
{
	InputProcessor processor;
	processor.kind = ProcessorType::SCALE;
	processor.settings = ScaleProcessorSettings{ 2.0f };

	nlohmann::json j = processor;
	InputProcessor parsed = j.get<InputProcessor>();

	CHECK(parsed.kind == ProcessorType::SCALE);
	REQUIRE(std::holds_alternative<ScaleProcessorSettings>(parsed.settings));
	CHECK(std::get<ScaleProcessorSettings>(parsed.settings).factor == doctest::Approx(2.0f));
}


TEST_CASE("Clamp processor round-trips through JSON")
{
	InputProcessor processor;
	processor.kind = ProcessorType::CLAMP;
	processor.settings = ClampProcessorSettings{ -1.0f, 1.0f };

	nlohmann::json j = processor;
	InputProcessor parsed = j.get<InputProcessor>();

	CHECK(parsed.kind == ProcessorType::CLAMP);
	REQUIRE(std::holds_alternative<ClampProcessorSettings>(parsed.settings));

	auto settings = std::get<ClampProcessorSettings>(parsed.settings);

	CHECK(settings.min == doctest::Approx(-1.0f));
	CHECK(settings.max == doctest::Approx(1.0f));
}


TEST_CASE("Invert processor round-trips through JSON with empty settings")
{
	InputProcessor processor;
	processor.kind = ProcessorType::INVERT;
	processor.settings = InvertProcessorSettings{};

	nlohmann::json j = processor;
	InputProcessor parsed = j.get<InputProcessor>();

	CHECK(parsed.kind == ProcessorType::INVERT);
	CHECK(std::holds_alternative<InvertProcessorSettings>(parsed.settings));
}


TEST_CASE("Normalize processor round-trips through JSON with empty settings")
{
	InputProcessor processor;
	processor.kind = ProcessorType::NORMALIZE;
	processor.settings = NormalizeProcessorSettings{};

	nlohmann::json j = processor;
	InputProcessor parsed = j.get<InputProcessor>();

	CHECK(parsed.kind == ProcessorType::NORMALIZE);
	CHECK(std::holds_alternative<NormalizeProcessorSettings>(parsed.settings));
}


TEST_SUITE_END();