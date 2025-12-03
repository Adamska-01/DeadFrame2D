#pragma once
#include "Models/Input/InputDeviceType.h"
#include <stdint.h>


namespace Shared::Models
{
	struct InputBinding
	{
		InputDeviceType inputDeviceType;

		uint8_t inputKey;
	};


	// Serialization functions (nlohmann)
	inline void to_json(nlohmann::json& j, const InputBinding& binding)
	{
		j = nlohmann::json
		{
			{"inputDeviceType", binding.inputDeviceType},
			{"inputKey", binding.inputKey}
		};
	}

	inline void from_json(const nlohmann::json& j, InputBinding& binding) 
	{
		j.at("inputDeviceType").get_to(binding.inputDeviceType);
		j.at("inputKey").get_to(binding.inputKey);
	}
}