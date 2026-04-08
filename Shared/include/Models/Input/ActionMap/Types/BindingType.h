#pragma once
#include <nlohmann/json.hpp>


namespace Shared::Models
{
	enum class BindingType
	{
		SIMPLE,

		COMPOSITE_2D,

		COMPOSITE_1D,
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(BindingType,
	{
		{ BindingType::SIMPLE, "Simple" },
		{ BindingType::COMPOSITE_2D, "Composite2D" },
		{ BindingType::COMPOSITE_1D, "Composite1D" },
	});
}