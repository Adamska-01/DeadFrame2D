#include <doctest.h>
#include "Models/Physics/PhysicsConfig.h"
#include <nlohmann/json.hpp>


using namespace DF2D::Models;


TEST_SUITE_BEGIN("PhysicsConfig");


TEST_CASE("from_json derives meterPerPixel from pixelPerMeter")
{
	nlohmann::json j =
	{
		{"gravityX", 0.0f},
		{"gravityY", 9.81f},
		{"velocityIterations", 8},
		{"positionIterations", 3},
		{"pixelPerMeter", 40.0f}
	};

	PhysicsConfig config = j.get<PhysicsConfig>();

	CHECK(config.meterPerPixel == doctest::Approx(1.0f / 40.0f));
}


TEST_CASE("from_json defaults debugDrawEnabled to true when absent")
{
	nlohmann::json j =
	{
		{"gravityX", 0.0f},
		{"gravityY", 9.81f},
		{"velocityIterations", 8},
		{"positionIterations", 3},
		{"pixelPerMeter", 40.0f}
	};

	PhysicsConfig config = j.get<PhysicsConfig>();

	CHECK(config.debugDrawEnabled == true);
}


TEST_CASE("from_json honors explicit debugDrawEnabled")
{
	nlohmann::json j =
	{
		{"gravityX", 0.0f},
		{"gravityY", 9.81f},
		{"velocityIterations", 8},
		{"positionIterations", 3},
		{"pixelPerMeter", 40.0f},
		{"debugDrawEnabled", false}
	};

	PhysicsConfig config = j.get<PhysicsConfig>();

	CHECK(config.debugDrawEnabled == false);
}


TEST_CASE("from_json throws on zero pixelPerMeter instead of producing inf")
{
	nlohmann::json j =
	{
		{"gravityX", 0.0f},
		{"gravityY", 9.81f},
		{"velocityIterations", 8},
		{"positionIterations", 3},
		{"pixelPerMeter", 0.0f}
	};

	CHECK_THROWS_AS(j.get<PhysicsConfig>(), std::runtime_error);
}


TEST_CASE("to_json does not emit the derived meterPerPixel field")
{
	PhysicsConfig config;
	config.pixelPerMeter = 40.0f;
	config.meterPerPixel = 1.0f / 40.0f;

	nlohmann::json j = config;

	CHECK_FALSE(j.contains("meterPerPixel"));
}


TEST_SUITE_END();