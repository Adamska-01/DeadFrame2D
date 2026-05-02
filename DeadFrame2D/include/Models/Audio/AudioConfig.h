#pragma once
#include <nlohmann/json.hpp>


namespace DF2D::Models
{
	struct AudioConfig
	{
		int maxSFXChannelAllocation = 32;

		float musicVolume = 1.0f;

		float sfxVolume = 1.0f;

		float masterVolume = 1.0f;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(
			AudioConfig,
			maxSFXChannelAllocation,
			musicVolume,
			sfxVolume,
			masterVolume)
	};
}