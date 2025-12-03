#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	struct Composite1D
	{
		int negative = -1;

		int positive = -1;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Composite1D, negative, positive)
	};
}