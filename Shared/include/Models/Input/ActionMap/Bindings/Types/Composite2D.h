#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct Composite2D
	{
		int up = -1;

		int down = -1;

		int left = -1;

		int right = -1;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Composite2D, up, down, left, right)
	};
}