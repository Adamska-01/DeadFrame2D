#pragma once
#include "Models/Input/ActionMap/InputActionMap.h"
#include <nlohmann/json.hpp>
#include <vector>


namespace DF2D::Models
{
	struct InputActionMapBucket
	{
		std::vector<InputActionMap> actionMaps;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(InputActionMapBucket, actionMaps)
	};
}