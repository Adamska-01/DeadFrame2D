#pragma once
#include <nlohmann/json.hpp>


namespace DeadFrame2D::Models
{
	struct Composite1D
	{
		int negative = -1;

		int positive = -1;


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Composite1D, negative, positive)
	};
}