#pragma once
#include <nlohmann/json.hpp>


namespace DF2D::Models
{
	enum class ValueType
	{
		ANY,

		BOOL,

		FLOAT,

		VECTOR2
	};


	NLOHMANN_JSON_SERIALIZE_ENUM(ValueType,
	{
		{ ValueType::ANY, "Any" },
		{ ValueType::BOOL, "Bool" },
		{ ValueType::FLOAT, "Float" },
		{ ValueType::VECTOR2, "Vector2" }
	});
}