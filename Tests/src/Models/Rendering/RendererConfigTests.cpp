#include <doctest.h>
#include "Models/Rendering/RendererConfig.h"
#include <nlohmann/json.hpp>


using namespace DF2D::Models;


TEST_SUITE_BEGIN("RendererConfig");


TEST_CASE("to_json omits targetFramerate when not set")
{
	RendererConfig config;
	config.width = 800;
	config.height = 600;

	nlohmann::json j = config;

	CHECK_FALSE(j.contains("targetFramerate"));
}


TEST_CASE("to_json includes targetFramerate when set")
{
	RendererConfig config;
	config.targetFramerate = 60;

	nlohmann::json j = config;

	REQUIRE(j.contains("targetFramerate"));
	CHECK(j["targetFramerate"] == 60);
}


TEST_CASE("from_json leaves targetFramerate empty when absent")
{
	nlohmann::json j = { {"width", 800}, {"height", 600} };

	RendererConfig config = j.get<RendererConfig>();

	CHECK_FALSE(config.targetFramerate.has_value());
}


TEST_CASE("from_json parses targetFramerate when present")
{
	nlohmann::json j = { {"width", 800}, {"height", 600}, {"targetFramerate", 144} };

	RendererConfig config = j.get<RendererConfig>();

	REQUIRE(config.targetFramerate.has_value());
	CHECK(*config.targetFramerate == 144);
}


TEST_CASE("from_json throws when required fields are missing")
{
	nlohmann::json j = { {"width", 800} };

	CHECK_THROWS(j.get<RendererConfig>());
}


TEST_SUITE_END();