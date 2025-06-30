#pragma once
#include "InputDeviceType.h"
#include <SDL_stdinc.h>


struct InputBinding
{
	InputDeviceType inputDeviceType;

	Uint8 inputKey;
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