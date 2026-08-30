#pragma once
#include "Core/Math/Color.h"
#include <string>


namespace DF2D::Utilities::StyleValues
{
	/** @brief Formats a color as the "#rrggbbaa" literal style syntax expects. */
	inline std::string ToColor(const Core::Color& color)
	{
		constexpr char digits[] = "0123456789abcdef";

		auto literal = std::string("#");

		for (auto channel : { color.r, color.g, color.b, color.a })
		{
			literal += digits[(channel >> 4) & 0x0F];
			literal += digits[channel & 0x0F];
		}

		return literal;
	}

	/** @brief Formats a pixel length, trimming the trailing zeros std::to_string leaves behind. */
	inline std::string ToPixels(float value)
	{
		auto text = std::to_string(value);

		auto lastDigit = text.find_last_not_of('0');

		if (lastDigit != std::string::npos && text[lastDigit] == '.')
		{
			lastDigit--;
		}

		text.erase(lastDigit + 1);

		return text + "px";
	}

	/** @brief Formats a 0-1 ratio as a percentage length. */
	inline std::string ToPercent(float normalized)
	{
		return std::to_string(normalized * 100.0f) + "%";
	}
}