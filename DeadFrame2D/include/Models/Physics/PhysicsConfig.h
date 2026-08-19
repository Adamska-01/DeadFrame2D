#pragma once
#include <nlohmann/json.hpp>
#include <stdexcept>


namespace DF2D::Models
{
	struct PhysicsConfig
	{
		float gravityX = 0.0f;

		float gravityY = 9.81f;

		int velocityIterations = 8;

		int positionIterations = 3;

		float pixelPerMeter = 40.0f;

		float meterPerPixel = 0.0f; // Derived value

		bool debugDrawEnabled = true;
	};


	inline void to_json(nlohmann::json& j, const PhysicsConfig& binding)
	{
		j = nlohmann::json{
			{"gravityX", binding.gravityX},
			{"gravityY", binding.gravityY},
			{"velocityIterations", binding.velocityIterations},
			{"positionIterations", binding.positionIterations},
			{"pixelPerMeter", binding.pixelPerMeter},
			{"debugDrawEnabled", binding.debugDrawEnabled},
		};
	}

	// Custom deserialization (Derive "meterPerPixel")
	inline void from_json(const nlohmann::json& j, PhysicsConfig& binding)
	{
		j.at("gravityX").get_to(binding.gravityX);
		j.at("gravityY").get_to(binding.gravityY);
		j.at("velocityIterations").get_to(binding.velocityIterations);
		j.at("positionIterations").get_to(binding.positionIterations);
		j.at("pixelPerMeter").get_to(binding.pixelPerMeter);

		binding.debugDrawEnabled = j.value("debugDrawEnabled", true);

		if (binding.pixelPerMeter <= 0.0f)
			throw std::runtime_error("PhysicsConfig: pixelPerMeter must be a positive number");

		// Compute derived property automatically
		binding.meterPerPixel = 1.0f / binding.pixelPerMeter;
	}
}